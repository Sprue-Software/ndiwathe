/*
 * WF200_comms_task.c
 *
 *  Created on: 12 Mar 2020
 *      Author: SMurva
 */

/*******************************************************************************
 * INCLUDES
 ******************************************************************************/

#include "WF200_host_common.h"
#include "sl_wfx_host.h"
#include "netifapi.h"

/*******************************************************************************
 * DEFINITIONS
 ******************************************************************************/


/*******************************************************************************
 * VARIABLES
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES
 ******************************************************************************/

static void WF200_Comms_Task(void *arg);
static sl_status_t WF200_ReceiveFrames(void);
static void WF200_Event_Task(void *arg);



/*******************************************************************************
 * FUNCTION DEFINITIONS
 ******************************************************************************/


/******************************************************************************
 * @brief       WF200_ReceiveFrames: Carries out the reception of data frames
 *              using appropriate driver API calls
 *
 * @param[in]   None
 *
 * @return      Error Status
 *
 ******************************************************************************/
static sl_status_t WF200_ReceiveFrames(void)
{
  sl_status_t result;
  uint16_t control_register = 0;

  do
  {
    result = sl_wfx_receive_frame(&control_register);
    SL_WFX_ERROR_CHECK( result );
  }while ((control_register & SL_WFX_CONT_NEXT_LEN_MASK) != 0);

error_handler:
  return result;
}


/******************************************************************************
 * @brief       WF200_Comms_Task: After carrying out some initialisation,
 *              this task handles the waiting and processing of the interrupt
 *              event that indicates data is ready to be read from the WF200
 *
 * @param[in]
 *
 * @return      None
 *
 ******************************************************************************/
static void WF200_Comms_Task(void *arg)
{
	EventGroupHandle_t eventGroupHandle = NULL;
	QueueHandle_t receiveFrameSemaphoreHandle = NULL;

	PRINTF("WF200_Comms_Task: COMMS TASK RUNNING\n");

	// Set the task running flag
	WF200_SetTaskRunning(WF200_COMMS_TASK, true);

	/* Create a mutex used for making driver accesses atomic */
	receiveFrameSemaphoreHandle = xSemaphoreCreateMutex();

	if(receiveFrameSemaphoreHandle != NULL)
	{
		WF200_SetSemaphoreHandle(receiveFrameSemaphoreHandle);
	}
	else
	{
		PRINTF("WF200_Comms_Task: Semaphore create failed\n");
		WF200_DEBUG_TRAP;
	}

	// Create an event group to track Wi-Fi events
	eventGroupHandle = xEventGroupCreate();

	if(eventGroupHandle != NULL)
	{
		WF200_SetEventGroupHandle(eventGroupHandle);
	}
	else
	{
		PRINTF("WF200_Comms_Task: Semaphore create failed\n");
		WF200_DEBUG_TRAP;
	}

    for(;;)
    {
    	// Wait for an interrupt from WF200
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Receive the frame(s) pending in WF200
        WF200_ReceiveFrames();
    }
}


/******************************************************************************
 * @brief       WF200_CreateCommsTask: creates the Comms task and handles
 *              appropriate task related initialisation/setup
 *
 * @param[in]   None
 *
 * @return      Error Status
 *
 ******************************************************************************/
sl_status_t WF200_CreateCommsTask(void)
{
	BaseType_t status;
	TaskHandle_t taskHandle;
	sl_status_t WF200_DriverStatus = SL_STATUS_FAIL;
	const WF200_ControlInfo_t *pControlInfo = WF200_GetControlInfoPtr();

	if(pControlInfo == NULL)
	{
		// Log failure
		PRINTF("WF200_CreateCommsTask: Error NULL Ptr\n");
		WF200_DEBUG_TRAP;
	}

	// Shouldn't be trying to create the Comms task if it is already created
	if(pControlInfo->WF200_TaskInfo[WF200_COMMS_TASK].TaskCreated == false)
	{
		status = xTaskCreate(WF200_Comms_Task,
			                 "WF200_Comms_Task",
						     WF200_COMMS_TASK_STACK_SIZE,
						     NULL,
						     WF200_COMMS_TASK_PRIORITY,
						     &taskHandle);

		if (status == pdPASS)
	    {
			// Update appropriate variables in the control structure
			WF200_SetTaskHandle(WF200_COMMS_TASK, taskHandle);
		    WF200_SetTaskCreated(WF200_COMMS_TASK, true);
		    PRINTF("WF200_CreateCommsTask: SUCCESS - Task Created!\r\n");
		    WF200_DriverStatus = SL_STATUS_OK;
	    }
	    else
	    {
	    	// Log failure
		    PRINTF("WF200_CreateCommsTask: ERROR - Task Creation Failed!\r\n");
		    WF200_DEBUG_TRAP;
	    }
	}
	else
	{
		// Log failure
		PRINTF("WF200_CreateCommsTask: ERROR - Trying to create a task that is already created\n" );
		WF200_DEBUG_TRAP;
	}

	return WF200_DriverStatus;
}


/******************************************************************************
 * @brief       WF200_CreateEventTask: creates the Events Task and handles
 *              appropriate task related initialisation/setup
 *
 * @param[in]   None
 *
 * @return      Error Status
 *
 ******************************************************************************/
sl_status_t WF200_CreateEventTask(void)
{
	BaseType_t status;
	TaskHandle_t taskHandle;
	sl_status_t WF200_DriverStatus = SL_STATUS_FAIL;
	const WF200_ControlInfo_t *pControlInfo = WF200_GetControlInfoPtr();

	if(pControlInfo == NULL)
	{
		// Log failure
		PRINTF("WF200_CreateEventTask: Error NULL Ptr\n");
		WF200_DEBUG_TRAP;
	}

	// Shouldn't be trying to create the Event Task if it is already created
	if(pControlInfo->WF200_TaskInfo[WF200_EVENT_TASK].TaskCreated == false)
	{
		status = xTaskCreate(WF200_Event_Task,
			                 "WF200_Event_Task",
						     WF200_EVENT_TASK_STACK_SIZE,
						     NULL,
						     WF200_EVENT_TASK_PRIORITY,
						     &taskHandle);

		if (status == pdPASS)
	    {
			// Update appropriate variables in the control structure
			WF200_SetTaskHandle(WF200_EVENT_TASK, taskHandle);
		    WF200_SetTaskCreated(WF200_EVENT_TASK, true);
		    PRINTF("WF200_CreateEventTask: SUCCESS - Task Created!\r\n");
		    WF200_DriverStatus = SL_STATUS_OK;
	    }
	    else
	   {
	    	// Log failure
		    PRINTF("WF200_CreateEventTask: ERROR - Task Creation Failed!\r\n");
		    WF200_DEBUG_TRAP;
	   }
	}
	else
	{
		// Log failure
		PRINTF("WF200_CreateEventTask: FAIL - Trying to create a task that is already created\n" );
		WF200_DEBUG_TRAP;
	}

	return WF200_DriverStatus;
}


/******************************************************************************
 * @brief       WF200_Event_Task: Waits for Events and handles them
 *
 * @param[in]
 *
 * @return      None
 *
 ******************************************************************************/
static void WF200_Event_Task(void *arg)
{
	err_t err;
    EventBits_t flags;
    struct netif *pAP_netif;
    struct netif *pSTA_netif;
    sl_status_t WF200_DriverStatus;
    const WF200_ControlInfo_t *pControlInfo = WF200_GetControlInfoPtr();

	PRINTF("WF200_Event_Task: EVENT TASK RUNNING\n");

	// Set the task running flag
	WF200_SetTaskRunning(WF200_EVENT_TASK, true);

	if(pControlInfo == NULL)
	{
		// Log failure
		PRINTF("WF200_Event_Task: Error NULL Ptr\n");
		WF200_DEBUG_TRAP;
	}

    for(;;)
    {
    	flags = xEventGroupWaitBits(pControlInfo->EventGroupHandle,
                                    SL_WFX_CONNECT | SL_WFX_STOP_AP | SL_WFX_DISCONNECT | SL_WFX_START_AP,
                                    pdTRUE,
                                    pdFALSE,
                                    3000/* 5000/portTICK_PERIOD_MS */);

        if (flags & SL_WFX_CONNECT)
        {
        	WF200_Set_STA_ModeNetworkConnected(true);
    	    WF200_DriverStatus = lwip_set_sta_link_up();

    	    if(WF200_DriverStatus != SL_STATUS_OK)
    	    {
    	    	// Log failure
    	    	PRINTF("WF200_Event_Task: ERROR - lwip sta link up error = %i\n", WF200_DriverStatus);
    		    WF200_DEBUG_TRAP;
    	    }

#ifdef SLEEP_ENABLED
            if (!(wifi.state & SL_WFX_AP_INTERFACE_UP))
            {
                // Enable the power save
                sl_wfx_set_power_mode(WFM_PM_MODE_PS, 0);
                sl_wfx_enable_device_power_save();
            }
#endif
        }

        if (flags & SL_WFX_DISCONNECT)
        {
        	WF200_DriverStatus = lwip_set_sta_link_down();

    	    if(WF200_DriverStatus != SL_STATUS_OK)
    	    {
    	    	// Log failure
    	    	PRINTF("WF200_Event_Task: ERROR - lwip sta link down error = %i\n", WF200_DriverStatus);
    		    WF200_DEBUG_TRAP;
    	    }

    	    pSTA_netif = WF200_Get_STA_NetIF();
    	    err = netifapi_netif_remove(pSTA_netif);

    	    if(err != ERR_OK)
    	    {
    	    	// Log failure
    	    	PRINTF("WF200_Event_Task: ERROR - netif remove fail = %i\n", err);
    	    	WF200_DriverStatus = SL_STATUS_FAIL;
    		    WF200_DEBUG_TRAP;
    	    }

    	    if(WF200_DriverStatus == SL_STATUS_OK)
    	    {
    	    	// Update the status in the Control Structure
    	    	WF200_Set_STA_NetIF_Status(WF200_STA_NETIF_NOT_ADDED);
    	    }

    	    // If the disconnect is not a result of a commanded
    	    // disconnect, start STA mode Task, to try to
    	    // reconnect
    	    if(pControlInfo->STA_CommandedDisconnect == false)
            {
            	WF200_DriverStatus = WF200_Create_STA_ModeTask();

        	    if(WF200_DriverStatus != SL_STATUS_OK)
        	    {
        	    	// Log failure
        	    	PRINTF("WF200_Event_Task: ERROR - STA Mode Task Create Fail = %i\n", WF200_DriverStatus);
        		    WF200_DEBUG_TRAP;
        	    }
            }

    	    // Update flag
    	    WF200_Set_STA_CommandedDisconnect(true);
        }


        if (flags & SL_WFX_START_AP)
        {
    	    WF200_DriverStatus = lwip_set_ap_link_up();
    	    PRINTF("WF200_Event_Task: LWIP: AP Mode Link Up\n");

    	    if(WF200_DriverStatus != SL_STATUS_OK)
    	    {
    		    // Log failure
	    	    PRINTF("WF200_Event_Task: ERROR - lwip set ap link up fail = %i\n", WF200_DriverStatus);
		        WF200_DEBUG_TRAP;
    	    }

#ifdef SLEEP_ENABLED
            // Power save always disabled when SoftAP mode enabled
            sl_wfx_set_power_mode(WFM_PM_MODE_ACTIVE, 0);
            sl_wfx_disable_device_power_save();
#endif
        }

        if (flags & SL_WFX_STOP_AP)
        {
    	    WF200_DriverStatus = lwip_set_ap_link_down();

    	    if(WF200_DriverStatus != SL_STATUS_OK)
    	    {
    		    // Log failure
	    	    PRINTF("WF200_Event_Task: ERROR - lwip set ap link down fail = %i\n", WF200_DriverStatus);
		        WF200_DEBUG_TRAP;
    	    }

    	    pAP_netif = WF200_Get_AP_NetIF();
            err = netifapi_netif_remove(pAP_netif);

            if(err != ERR_OK)
            {
        	    // Log failure
	    	    PRINTF("WF200_Event_Task: ERROR - netif remove fail = %i\n", err);
	    	    WF200_DriverStatus = SL_STATUS_FAIL;
		        WF200_DEBUG_TRAP;
            }

    	    if(WF200_DriverStatus == SL_STATUS_OK)
    	    {
    	    	// Update the status in the Control Structure
    	    	WF200_Set_AP_NetIF_Status(WF200_AP_NETIF_NOT_ADDED);
    	    }

#ifdef SLEEP_ENABLED
            if (wifi.state & SL_WFX_STA_INTERFACE_CONNECTED)
            {
                // Enable the power save
                sl_wfx_set_power_mode(WFM_PM_MODE_PS, 0);
                sl_wfx_enable_device_power_save();
            }
#endif
        }
    }
}


