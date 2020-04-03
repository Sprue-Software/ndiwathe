/*
 * WF200_sta_mode_task.c
 *
 *  Created on: 4 Mar 2020
 *      Author: SMurva
 */


/*******************************************************************************
 * INCLUDES
 ******************************************************************************/

#include "lwip/netifapi.h"
#include "lwip/opt.h"
#include "lwip/tcpip.h"
#include "lwip/dhcp.h"
#include "lwip/prot/dhcp.h"
#include "netif/ethernet.h"
#include "WF200_host_common.h"
#include "board.h"


/*******************************************************************************
 * DEFINITIONS
 ******************************************************************************/

#define WF200_STA_MODE_MAX_NUMBER_CONNECTION_ATTEMPTS   10


/*******************************************************************************
 * VARIABLES
 ******************************************************************************/

// !!! These need to be in non volatile/persistent
// data area !!!
// Network Connection Details
char WLAN_SSID[30];
int  WLAN_SSID_Length;
char WLAN_Password[30];
int  WLAN_PasswordLength;
sl_wfx_security_mode_t WLAN_SecurityMode;
bool NetworkDetailsKnown = true;


/*******************************************************************************
 * FUNCTION PROTOTYPES
 ******************************************************************************/

static void WF200_STA_ModeTask(void *arg);
static sl_status_t WF200_STA_Mode_SetupIP_Add(const WF200_ControlInfo_t *pControlInfo);


/*******************************************************************************
 * FUNCTION DEFINITIONS
 ******************************************************************************/


/******************************************************************************
 * @brief       WF200_Setup_STA_Mode: Carry out basic checks to ensure
 *              conditions are met to create the STA Mode Task
 *
 * @param[in]   None
 *
 * @return      Error Status
 *
 ******************************************************************************/
sl_status_t WF200_Setup_STA_Mode(void)
{
	sl_status_t WF200_DriverStatus = SL_STATUS_OK;
	const WF200_ControlInfo_t *pControlInfo = WF200_GetControlInfoPtr();

	if(pControlInfo != NULL)
	{
		// Carry out some sanity checks to ensure correct setup conditions
		if((pControlInfo->WF200_TaskInfo[WF200_COMMS_TASK].TaskCreated == false)   ||
		   (pControlInfo->WF200_TaskInfo[WF200_STA_MODE_TASK].TaskCreated == true) ||
		   (pControlInfo->WF200_TaskInfo[WF200_EVENT_TASK].TaskCreated == false)   ||
		   (NetworkDetailsKnown == false))
		{
			// Log failure
			WF200_DriverStatus = SL_STATUS_FAIL;
			PRINTF("WF200_Setup_STA_Mode: ERROR - Initial checks\n");
			WF200_DEBUG_TRAP;
		}
		else
		{
			// Create the Task
			WF200_DriverStatus = WF200_Create_STA_ModeTask();
		}
	}
	else
	{
		// Log failure
		WF200_DriverStatus = SL_STATUS_FAIL;
		PRINTF("WF200_Setup_STA_Mode: ERROR - NULL Ptr\n");
		WF200_DEBUG_TRAP;
	}

	return WF200_DriverStatus;
}


/******************************************************************************
 * @brief       WF200_Create_STA_ModeTask: Creates the STA Mode Task
 *
 * @param[in]   None
 *
 * @return      Error Status
 *
 ******************************************************************************/
sl_status_t WF200_Create_STA_ModeTask(void)
{
	BaseType_t status;
	TaskHandle_t taskHandle = NULL;
	sl_status_t WF200_DriverStatus = SL_STATUS_OK;
	const WF200_ControlInfo_t *pControlInfo = WF200_GetControlInfoPtr();

	if(pControlInfo != NULL)
	{
		// Shouldn't be trying to create the task if it is already created
		if((pControlInfo->WF200_TaskInfo[WF200_STA_MODE_TASK].TaskCreated == false) &&
		   (pControlInfo->WF200_TaskInfo[WF200_STA_MODE_TASK].TaskRunning == false))
		{
			status = xTaskCreate(WF200_STA_ModeTask,
			    	             "WF200_STA_ModeTask",
				    			 WF200_STA_MODE_TASK_STACK_SIZE,
					    		 NULL,
						    	 WF200_STA_MODE_TASK_PRIORITY,
						    	 &taskHandle);

		    if (status == pdPASS)
		    {
		    	// Update control structure flags
		    	WF200_SetTaskHandle(WF200_STA_MODE_TASK, taskHandle);
		    	WF200_SetTaskCreated(WF200_STA_MODE_TASK, true);

		    	// Log status
			    PRINTF("WF200_Create_STA_ModeTask: SUCCESS - Task Created\n");
		    }
		    else
		    {
		    	// Log failure
		    	WF200_DriverStatus = SL_STATUS_FAIL;
			    PRINTF("WF200_Create_STA_ModeTask: ERROR - Task Creation\n");
			    WF200_DEBUG_TRAP;
		    }
		}
		else
		{
			// Log failure
			WF200_DriverStatus = SL_STATUS_FAIL;
			PRINTF("WF200_Create_STA_ModeTask: ERROR - Trying to setup a task that is already created\n" );
			WF200_DEBUG_TRAP;
		}
	}
	else
	{
		// Log failure
		WF200_DriverStatus = SL_STATUS_FAIL;
		PRINTF("WF200_Create_STA_ModeTask: ERROR - NULL Ptr\n");
		WF200_DEBUG_TRAP;
	}

	return WF200_DriverStatus;
}


/******************************************************************************
 * @brief       WF200_STA_ModeTask: Handles the setup of the STA Mode
 *
 * @param[in]
 *
 * @return      None
 *
 ******************************************************************************/
void WF200_STA_ModeTask(void *arg)
{
	err_t err;
	ip4_addr_t sta_netif_gw;
	struct netif *pSTA_netif;
	ip4_addr_t sta_netif_ipaddr;
	ip4_addr_t sta_netif_netmask;
	uint8_t NumConnectionAttempts = 0;
	sl_status_t WF200_DriverStatus = SL_STATUS_OK;

	const WF200_ControlInfo_t *pControlInfo = WF200_GetControlInfoPtr();

	if(pControlInfo != NULL)
	{
		// Log status
		PRINTF("WF200 STA MODE TASK RUNNING\r\n");

		// Update control structure
		WF200_SetTaskRunning(WF200_STA_MODE_TASK, true);

		// Get a pointer to the netif
  	    pSTA_netif = WF200_Get_STA_NetIF();

  		// Shouldn't already be connected
  		if(pControlInfo->STA_ModeNetworkConnected == false)
  		{
  			// Initialise address information
  			IP4_ADDR(&sta_netif_ipaddr, 0, 0, 0, 0);
            IP4_ADDR(&sta_netif_netmask, 0, 0, 0, 0);
            IP4_ADDR(&sta_netif_gw, 0, 0, 0, 0);

            // Add in the netif
            err = netifapi_netif_add(pSTA_netif,
    		                         &sta_netif_ipaddr,
					                 &sta_netif_netmask,
					                 &sta_netif_gw,
					                 NULL,
		                             sta_ethernetif_init,
					                 tcpip_input);

            // Update status in the Control Structure
            WF200_Set_STA_NetIF_Status(WF200_STA_NETIF_ADDED);

            if(err != ERR_OK)
            {
            	// Log failure
            	PRINTF("WF200_STA_ModeTask: ERROR - netif add fail = %i\n", err);
            	WF200_DriverStatus = SL_STATUS_FAIL;
        		WF200_DEBUG_TRAP;
            }

            if(WF200_DriverStatus == SL_STATUS_OK)
            {
            	// Set the netif to default
            	err = netifapi_netif_set_default(pSTA_netif);
            }

            if(err != ERR_OK)
            {
            	// Log failure
        	    PRINTF("WF200_STA_ModeTask: ERROR - netif set default fail = %i\n", err);
        	    WF200_DriverStatus = SL_STATUS_FAIL;
    		    WF200_DEBUG_TRAP;
            }

            if(WF200_DriverStatus == SL_STATUS_OK)
            {
            	// Let LWIP know the connection is up
                err = netifapi_netif_set_up(pSTA_netif);
            }

            if(err != ERR_OK)
            {
            	// Log failure
            	PRINTF("WF200_STA_ModeTask: ERROR - netif set up fail = %i\n", err);
            	WF200_DriverStatus = SL_STATUS_FAIL;
        		WF200_DEBUG_TRAP;
            }


            // Try to connect, if need be multiple times
            while((pControlInfo->STA_ModeNetworkConnected == false) &&
		          (NumConnectionAttempts < WF200_STA_MODE_MAX_NUMBER_CONNECTION_ATTEMPTS) &&
				  (WF200_DriverStatus == SL_STATUS_OK))
	        {
            	if(pControlInfo->STA_ModeNetworkConnected == false)
		        {
            		// Attempt the STA mode connect
            		WF200_DriverStatus = sl_wfx_send_join_command((uint8_t*) &WLAN_SSID,
			    		                                          WLAN_SSID_Length,
												                  NULL,
												                  0,
												                  WLAN_SecurityMode,
												                  0,
												                  0,
			                                                      (uint8_t*) &WLAN_Password,
												                  WLAN_PasswordLength,
			                                                      NULL,
												                  0);

				    NumConnectionAttempts++;
			        PRINTF("WF200_STA_ModeTask Connection attempt: %i\r\n", NumConnectionAttempts);

			        // Allow some time before
			        // trying again
			        vTaskDelay(5000);

			        // Check if a connection has been made
			        if(pControlInfo->STA_ModeNetworkConnected != false)
			        {
			        	break;
			        }
		        }
	        }

    		if(pControlInfo->STA_ModeNetworkConnected == false)
    		{
    			// Log failure
    			PRINTF("WF200_STA_ModeTask: ERROR - STA Mode Connection Failure\n");
    		}
  		}
        else
	    {
        	// Log failure
        	PRINTF("WF200_STA_ModeTask: ERROR - Trying to connect when already connected\n");
    		WF200_DEBUG_TRAP;
	    }

  		// On a successful connection get an IP address
  		if((pControlInfo->STA_ModeNetworkConnected == true) && (WF200_DriverStatus == SL_STATUS_OK))
	    {
  			WF200_DriverStatus = WF200_STA_Mode_SetupIP_Add(pControlInfo);
	    }

  		if (WF200_DriverStatus == SL_STATUS_OK)
  		{
 		    // Log status
            PRINTF("WF200 STA MODE TASK BEEN DELETED\n");

  			// Task has done its job so can be deleted
  		    WF200_DriverStatus = WF200_DeleteTask(WF200_STA_MODE_TASK);

   		}

  		// Task should have been deleted, however,
  		// a failure has occurred
  		if(WF200_DriverStatus != SL_STATUS_OK)
  		{
  			// Log failure
  			PRINTF("WF200_STA_ModeTask: Connection Error\n");
  			WF200_DEBUG_TRAP;
  		}
	}
	else
	{
		// Log failure
		PRINTF("WF200_STA_ModeTask: ERROR - NULL Ptr\n");
		WF200_DEBUG_TRAP;
	}
}


/******************************************************************************
 * @brief       WF200_STA_Mode_SetupIP_Add: Set up the IP address
 *
 *
 * @param[in]   Pointer to the control structure
 *
 * @return      Error Status
 *
 ******************************************************************************/
static sl_status_t WF200_STA_Mode_SetupIP_Add(const WF200_ControlInfo_t *pControlInfo)
{
	struct dhcp *dhcp;
	struct netif *pSTA_netif;
	sl_status_t WF200_DriverStatus = SL_STATUS_OK;

	if(pControlInfo != NULL)
	{
		PRINTF("Getting IP address from DHCP ...\n");

		pSTA_netif = WF200_Get_STA_NetIF();
        netifapi_dhcp_start(pSTA_netif);
        dhcp = netif_dhcp_data(pSTA_netif);

        while (dhcp->state != DHCP_STATE_BOUND)
        {
        	vTaskDelay(10);
        }

        if (dhcp->state == DHCP_STATE_BOUND)
        {
        	PRINTF("\r\n IPv4 Address     : %u.%u.%u.%u\r\n", ((u8_t *)&pSTA_netif->ip_addr.addr)[0],
        		   ((u8_t *)&pSTA_netif->ip_addr.addr)[1], ((u8_t *)&pSTA_netif->ip_addr.addr)[2],
				   ((u8_t *)&pSTA_netif->ip_addr.addr)[3]);

        }

        // Log status
        PRINTF("DHCP OK\r\n");
    }
    else
	{
    	// Log failure
    	PRINTF("WF200_STA_Mode_SetupIP_Add: ERROR NULL Ptr\n");
    	WF200_DriverStatus = SL_STATUS_FAIL;
		WF200_DEBUG_TRAP;
	}

    return WF200_DriverStatus;
}

