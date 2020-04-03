/*
 * WF200_testing_task.c
 *
 *  Created on: 13 Mar 2020
 *      Author: SMurva
 */

/*******************************************************************************
 * INCLUDES
 ******************************************************************************/

#include "WF200_host_common.h"
#include "dhcp_server.h"

#include "netdb.h"
#include "netifapi.h"
#include "httpsclient.h"
#include "tcpecho_raw.h"

/*******************************************************************************
 * DEFINITIONS
 ******************************************************************************/

typedef enum
{
	// AP MODE
	WF200_TEST_START_AP = 0,
	WF200_TEST_STOP_AP,
	WF200_TEST_CREATE_AP_MODE_TASK,
	WF200_TEST_DELETE_AP_MODE_TASK,
	WF200_TEST_INPUT_AP_MODE_DETAILS,

	// STA MODE
	WF200_TEST_SETUP_STA_MODE,
	WF200_TEST_CREATE_STA_MODE_TASK,
	WF200_TEST_STA_CONNECT,
	WF200_TEST_STA_DISCONNECT,
	WF200_TEST_DELETE_STA_MODE_TASK,
	WF200_TEST_INPUT_STA_MODE_NETWORK_DETAILS,

	// INIT TASK
	WF200_TEST_CREATE_INIT_TASK,
	WF200_TEST_DELETE_INIT_TASK,

	// COMMS TASK
	WF200_TEST_CREATE_COMMS_TASK,
	WF200_TEST_DELETE_COMMS_TASK,

	// EVENT TASK
	WF200_TEST_CREATE_EVENT_TASK,
	WF200_TEST_DELETE_EVENT_TASK,

	// PTA
	WF200_TEST_PTA_SETTINGS,
	WF200_TEST_PTA_PRIORITY,
	WF200_TEST_PTA_STATE,

	// POWER/SIGNAL STRENGTH
	WF200_TEST_GET_SIGNAL_STRENGTH,
	WF200_TEST_SET_MAX_TX_POWER,
	WF200_TEST_GET_MAX_TX_POWER,

    // POWER MODES
	WF200_TEST_SHUTDOWN,
	WF200_TEST_WAKEUP,

    // TESTS
	WF200_TEST_MBEDTLS_TEST,
	WF200_TEST_ECHO_TEST,

	WF200_TEST_EXIT_TESTS,
	WF200_TEST_MAX_NUM_TESTS
} TestIDs;



/*******************************************************************************
 * VARIABLES
 ******************************************************************************/

// Network Connection Details
extern char WLAN_SSID[20];
extern int  WLAN_SSID_Length;
extern char WLAN_Password[20];
extern int  WLAN_PasswordLength;
extern sl_wfx_security_mode_t WLAN_SecurityMode;

extern char softap_ssid[32];
extern char softap_passkey[64];
extern sl_wfx_security_mode_t softap_security;
extern uint8_t softap_channel;

// WF200 Host Control Info
extern WF200_ControlInfo_t WF200_ControlInfo;

bool TestNotComplete = false;

extern struct netif sta_netif;


// Both below need to be global since the code below passes a ptr on the queue, i.e. by reference
uint8_t TestNumber;
uint8_t AP_Mode_InputDetails[20] = {WF200_TEST_INPUT_AP_MODE_DETAILS, 8, 'T', 'E', 'M', 'P', 'T', 'E', 'S', 'T' , 8, 'T', 'E', 'S', 'T', 'P', 'A', 'S', 'S', 4};
uint8_t STA_Mode_InputDetails[20] = {WF200_TEST_INPUT_STA_MODE_NETWORK_DETAILS, 8, 'T', 'E', 'M', 'P', 'T', 'E', 'S', 'T' , 8, 'T', 'E', 'S', 'T', 'P', 'A', 'S', 'S', 4};


/*******************************************************************************
 * FUNCTION PROTOTYPES
 ******************************************************************************/

sl_status_t WF200_Create_TEST_ModeTask(void);
void WF200_TEST_ModeTask(void *arg);

//////////////////////////////////////////////////////////////////////////////////
// !!! TEMPORARY INPUT TASK UNTIL THE TEST FRAMEWORK/CLI HAS BEEN DECIDED UPON !!!
void TempInputTask(void *arg);

/*******************************************************************************
 * FUNCTION DEFINITIONS
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *
 *
 * @param[in]
 *
 * @return
 *    None
 ******************************************************************************/
sl_status_t WF200_Setup_Testing_Mode(void)
{
	QueueHandle_t queueHandle = NULL;
	sl_status_t WF200_DriverStatus = SL_STATUS_OK;
	const WF200_ControlInfo_t *pControlInfo = WF200_GetControlInfoPtr();

	if(pControlInfo == NULL)
	{
		PRINTF("WF200_Setup_Testing_Mode: Error NULL Ptr\n");
		WF200_DEBUG_TRAP;
	}

	// Carry out some sanity checks to ensure correct setup conditions
	if((pControlInfo->WF200_TaskInfo[WF200_AP_MODE_TASK].TaskCreated != false)   ||
	   (pControlInfo->WF200_TaskInfo[WF200_COMMS_TASK].TaskCreated != true)      ||
	   (pControlInfo->WF200_TaskInfo[WF200_STA_MODE_TASK].TaskCreated != false)  ||
	   (pControlInfo->WF200_TaskInfo[WF200_TEST_MODE_TASK].TaskCreated != false))
	{
		WF200_DriverStatus = SL_STATUS_FAIL;
	}

	if(WF200_DriverStatus == SL_STATUS_OK)
	{
		queueHandle = xQueueCreate(5, sizeof(uint8_t*));

		if(queueHandle != NULL)
		{
			WF200_SetQueueHandle(WF200_TEST_MODE_TASK, queueHandle);
		}
		else
		{
			WF200_DriverStatus = SL_STATUS_FAIL;
			PRINTF("WF200_Setup_Testing_Mode: TEST Mode Queue Creation Fail\n");
			WF200_DEBUG_TRAP;
		}

		if(WF200_DriverStatus == SL_STATUS_OK)
		{
			WF200_DriverStatus = WF200_CreateTestModeTask();
		}
	}

	return WF200_DriverStatus;
}


/***************************************************************************//**
 * @brief
 *
 *
 * @param[in]
 *
 * @return
 *    None
 ******************************************************************************/
sl_status_t WF200_CreateTestModeTask(void)
{
	BaseType_t status;
	TaskHandle_t taskHandle = NULL;
	sl_status_t WF200_DriverStatus = SL_STATUS_FAIL;
	const WF200_ControlInfo_t *pControlInfo = WF200_GetControlInfoPtr();

	if(pControlInfo == NULL)
	{
		PRINTF("WF200_CreateTestModeTask: Error NULL Ptr\n");
		WF200_DEBUG_TRAP;
	}

	// Shouldn't be trying to create the Task if it is already created
	if(pControlInfo->WF200_TaskInfo[WF200_TEST_MODE_TASK].TaskCreated == false)
	{
		status = xTaskCreate(WF200_TEST_ModeTask,
			                 "WF200_TEST_ModeTask",
						     WF200_TEST_MODE_TASK_STACK_SIZE,
						     NULL,
						     WF200_TEST_MODE_TASK_PRIORITY,
						     &taskHandle);

	    if (status == pdPASS)
	    {
	    	WF200_DriverStatus = SL_STATUS_OK;
	    	WF200_SetTaskHandle(WF200_TEST_MODE_TASK, taskHandle);
	    	WF200_SetTaskCreated(WF200_TEST_MODE_TASK, true);

		    PRINTF("WF200_CreateTestModeTask: SUCCESS - Task Created\n");
	    }
	    else
	    {
		    // Log failure
		    PRINTF("WF200_CreateTestModeTask: FAIL - Creation Failed\n");
		    WF200_DEBUG_TRAP;
	    }
	}
	else
	{
		PRINTF("WF200_CreateTestModeTask: FAIL - Trying to setup a task that is already created\n" );
		WF200_DEBUG_TRAP;
	}

	return WF200_DriverStatus;
}


/***************************************************************************//**
 * @brief
 *
 *
 * @param[in]
 *
 * @return
 *    None
 ******************************************************************************/
void WF200_TEST_ModeTask(void *arg)
{
	uint32_t rcpi;
	bool Exit = false;
	BaseType_t status = pdTRUE;
	sl_status_t WF200_DriverStatus;
	const WF200_ControlInfo_t *pControlInfo = WF200_GetControlInfoPtr();
	uint8_t *RxBuffer;
	uint8_t loop;
	uint8_t *ptr;

	uint8_t softap_ssid_length;
	uint8_t softap_passkey_length;

	int32_t max_tx_power_rf_port_1;
	int32_t max_tx_power_rf_port_2;
	int32_t max_tx_power;
	sl_wfx_interface_t interface;

	if(pControlInfo == NULL)
	{
		PRINTF("WF200_TEST_ModeTask: Error NULL Ptr\n");
		WF200_DEBUG_TRAP;
	}

	PRINTF("WF200 TEST MODE TASK RUNNING\r\n");
	WF200_SetTaskRunning(WF200_TEST_MODE_TASK, true);

	/////////////////////
	// Temp to be removed
	if (xTaskCreate(TempInputTask, "TempInputTask", 3000, NULL, configMAX_PRIORITIES - 4, NULL) != pdPASS)
	{
		WF200_DEBUG_TRAP;
	}
	/////////////////////


	while(Exit == false)
	{

		status = xQueueReceive(pControlInfo->WF200_TaskInfo[WF200_TEST_MODE_TASK].QueueHandle,
	                           &RxBuffer, 100);

		ptr = RxBuffer;

	    if(status == pdTRUE)
	    {
	    	PRINTF("\n\n   *** TEST MODE DEBUG: %i\n\n", *RxBuffer);

	    	switch(*ptr /*RxBuffer[0]*/)
	    	{
	    	case WF200_TEST_START_AP:
                PRINTF("   *** WF200_TEST_ModeTask: START AP\n");

                // The NetIF must be setup, otherwise errors occur
	    		WF200_DriverStatus = WF200_AP_ModeSetupNetIF();

	    		if(WF200_DriverStatus == SL_STATUS_OK)
	    		{
	    			// Carry out a AP Start
	                WF200_DriverStatus = sl_wfx_start_ap_command(softap_channel,              // Channel
	    		                                                 (uint8_t*) softap_ssid,      // SSID
								     			                 strlen(softap_ssid),         // SSID Length
													             0,                           // Hidden SSID
													             0,                           // Client Isolation
													             softap_security,             // Security Mode
													             0,                           // MFP
	                                                             (uint8_t*) softap_passkey,   // Passkey
													             strlen(softap_passkey),      // Passkey Length
	                                                             NULL,                        // Beacon IE Data
													             0,                           // Beacon IE Data Length
													             NULL,                        // Probe Response IE Data
													             0);                          // Probe Response IE Data Length
	    		}
	    		break;

	    	case  WF200_TEST_STOP_AP:
	    		PRINTF("   *** WF200_TEST_ModeTask: STOP AP\n");
	    		WF200_DriverStatus = sl_wfx_stop_ap_command();
	    		break;

	    	case WF200_TEST_CREATE_AP_MODE_TASK:
	    		// Setup the AP mode of operation
	    		WF200_DriverStatus = WF200_Setup_AP_Mode();
	    		break;

	    	case WF200_TEST_DELETE_AP_MODE_TASK:
	    		PRINTF("   *** WF200_TEST_ModeTask: Delete AP MODE Task\n");
				WF200_DeleteTask(WF200_AP_MODE_TASK);
				WF200_DriverStatus = SL_STATUS_OK;
	    		break;

	    	case WF200_TEST_INPUT_AP_MODE_DETAILS:
	    		PRINTF("   *** WF200_TEST_ModeTask: Input AP Mode Details\n");

	    		softap_ssid_length = 0;
	    		softap_passkey_length = 0;
	    		softap_security = 0;
	    		memset(&softap_ssid, 0x00, sizeof(softap_ssid));
	    		memset(&softap_passkey, 0x00, sizeof(softap_passkey));

	    		ptr++;
	    		softap_ssid_length = *ptr++;

	    		for(loop=0; loop<softap_ssid_length; loop++)
	    		{
	    			softap_ssid[loop] = *ptr++;
	    		}

	    		// Add in the NULL Terminator
	    		softap_ssid[loop] = '\0';
	    		softap_ssid_length += 1;

	    		softap_passkey_length = *ptr++;

	    		for(loop=0; loop<softap_passkey_length; loop++)
	    		{
	    			softap_passkey[loop] = *ptr++;
	    		}

	    		// Add in the NULL Terminator
	    		softap_passkey[loop] = '\0';
	    		softap_passkey_length += 1;

				softap_security = *ptr;

				PRINTF("INPUT AP MODE DETAILS: SSID = %s, PASSKEY = %s, Security = %i\n",
						softap_ssid,
						softap_passkey,
						softap_security);

				WF200_DriverStatus = SL_STATUS_OK;
	    		break;

	    	case WF200_TEST_CREATE_STA_MODE_TASK:

	    		PRINTF("   *** WF200_TEST_ModeTask: Setup STA Mode\n");
	    		WF200_DriverStatus = WF200_Setup_STA_Mode();
	    		break;

	    	case WF200_TEST_MBEDTLS_TEST:
	    		WF200_DeleteTask(WF200_EVENT_TASK);

	    		PRINTF("   *** WF200_TEST_ModeTask: https client tls test\n");
	    	    if (xTaskCreate(httpsclient_task, "httpsclient_task", 3000, NULL, configMAX_PRIORITIES - 4, NULL) != pdPASS)
	    	    {
	    	        PRINTF("Task creation failed!\r\n");
	    	        while (1)
	    	        {
	    	        }
	    	    }

	    	    WF200_DriverStatus = SL_STATUS_OK;
	    		break;

	    	case WF200_TEST_STA_CONNECT:

	    		PRINTF("   *** WF200_TEST_ModeTask: STA Connect\n");
			    status = sl_wfx_send_join_command((uint8_t*) &WLAN_SSID,
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
			    WF200_DriverStatus = SL_STATUS_OK;
	    		break;

	    	case WF200_TEST_STA_DISCONNECT:

	    		PRINTF("   *** WF200_TEST_ModeTask: STA Disconnect\n");
	    		status =  sl_wfx_send_disconnect_command();
	    		break;

	    	case WF200_TEST_SHUTDOWN:

	    		PRINTF("   *** WF200_TEST_ModeTask: Shutdown the WF200\n");
	    		WF200_ShutDown();
	    		WF200_DriverStatus = SL_STATUS_OK;
	    		break;

	    	case WF200_TEST_DELETE_COMMS_TASK:

	    		PRINTF("   *** WF200_TEST_ModeTask: WF200_DeleteTask(WF200_COMMS_TASK)\n");
				WF200_DeleteTask(WF200_COMMS_TASK);
				PRINTF("   *** WF200_TEST_ModeTask: WF200 COMMS TASK BEEN DELETED\n");

				WF200_DriverStatus = SL_STATUS_OK;
	    		break;

	    	case WF200_TEST_DELETE_EVENT_TASK:

	    		PRINTF("   *** WF200_TEST_ModeTask: WF200_DeleteTask(WF200_EVENT_TASK)\n");
				WF200_DeleteTask(WF200_EVENT_TASK);
				WF200_DriverStatus = SL_STATUS_OK;
	    		break;

	    	case WF200_TEST_DELETE_STA_MODE_TASK:

	    		PRINTF("   *** WF200_TEST_DELETE_STA_MODE_TASK: WF200_DeleteTask(WF200_TEST_DELETE_STA_MODE_TASK)\n");
				WF200_DeleteTask(WF200_TEST_DELETE_STA_MODE_TASK);
				WF200_DriverStatus = SL_STATUS_OK;
	    		break;

	    	case WF200_TEST_INPUT_STA_MODE_NETWORK_DETAILS:
	    		PRINTF("   *** WF200_TEST_ModeTask: Input STA Mode Network Details\n");

	    		WLAN_SSID_Length = 0;
	    		WLAN_PasswordLength = 0;
	    		WLAN_SecurityMode = 0;
	    		memset(&WLAN_SSID, 0x00, sizeof(WLAN_SSID));
	    		memset(&WLAN_Password, 0x00, sizeof(WLAN_Password));

	    		ptr++;
	    		WLAN_SSID_Length = *ptr++;

	    		for(loop=0; loop<WLAN_SSID_Length; loop++)
	    		{
	    			WLAN_SSID[loop] = *ptr++;
	    		}

	    		// Add in the NULL Terminator
	    		WLAN_SSID[loop] = '\0';
	    		WLAN_SSID_Length += 1;

	    		WLAN_PasswordLength = *ptr++;

	    		for(loop=0; loop<WLAN_PasswordLength; loop++)
	    		{
	    			WLAN_Password[loop] = *ptr++;
	    		}

	    		// Add in the NULL Terminator
	    		WLAN_Password[loop] = '\0';
	    		WLAN_PasswordLength += 1;

	    		WLAN_SecurityMode = *ptr;

				PRINTF("INPUT STA MODE NETWORK DETAILS: SSID = %s, PASSKEY = %s, Security = %i\n",
						WLAN_SSID,
						WLAN_Password,
						WLAN_SecurityMode);

				WF200_DriverStatus = SL_STATUS_OK;

	    		break;

	    	case WF200_TEST_CREATE_COMMS_TASK:

	    		PRINTF("WF200_TEST_ModeTask: WF200_CreateCommsTask()\n");
	    		WF200_DriverStatus = WF200_CreateCommsTask();
	    		break;

	    	case WF200_TEST_CREATE_EVENT_TASK:

	    		PRINTF("WF200_TEST_ModeTask: WF200_CreateEventTask()\n");
	    		WF200_DriverStatus = WF200_CreateEventTask();
	    		break;

	    	case WF200_TEST_CREATE_INIT_TASK:

	    		PRINTF("WF200_TEST_ModeTask: WF200_Create_WF200_InitTask()\n");
	    		WF200_DriverStatus = WF200_Create_WF200_InitTask();
	    		break;

	    	case WF200_TEST_ECHO_TEST:
	    		// Set up the Echo test
	    		tcpecho_raw_init();
	    		break;


	    	case WF200_TEST_WAKEUP:

	    		break;

	    	case WF200_TEST_PTA_SETTINGS:

	    		WF200_DriverStatus = sl_wfx_pta_settings(RxBuffer[1],  /* uint8_t pta_mode, */
	    		                                         RxBuffer[2],  /* uint8_t request_signal_active_level, */
	    		                                         RxBuffer[3],  /* uint8_t priority_signal_active_level, */
	    		                                         RxBuffer[4],  /* uint8_t freq_signal_active_level, */
	    		                                         RxBuffer[5],  /* uint8_t grant_signal_active_level, */
	    		                                         RxBuffer[6],  /* uint8_t coex_type, */
	    		                                         RxBuffer[7],  /* uint8_t default_grant_state, */
	    		                                         RxBuffer[8],  /* uint8_t simultaneous_rx_access, */
	    		                                         RxBuffer[9],  /* uint8_t priority_sampling_time, */
	    		                                         RxBuffer[10], /* uint8_t tx_rx_sampling_time, */
	    		                                         RxBuffer[11], /* uint8_t freq_sampling_time, */
	    		                                         RxBuffer[12], /* uint8_t grant_valid_time, */
	    		                                         RxBuffer[13], /* uint8_t fem_control_time, */
	    		                                         RxBuffer[14], /* uint8_t first_slot_time, */
	    		                                         RxBuffer[15], /* uint16_t periodic_tx_rx_sampling_time, ?????????????? */
	    		                                         RxBuffer[16], /* uint16_t coex_quota, ?????????????*/
	    		                                         RxBuffer[17]  /* uint16_t wlan_quota  ?????????????*/);
	    		break;

	    	case WF200_TEST_PTA_PRIORITY:
	    		WF200_DriverStatus = sl_wfx_pta_priority((uint32_t)RxBuffer[1] /* uint32_t priority */);
	    		break;

	    	case WF200_TEST_PTA_STATE:
	    		WF200_DriverStatus = sl_wfx_pta_state((uint32_t)RxBuffer[1] /* uint32_t pta_state */);
	    		break;



	    	case WF200_TEST_GET_SIGNAL_STRENGTH:
	    		WF200_DriverStatus = sl_wfx_get_signal_strength(&rcpi);
	    		if(WF200_DriverStatus == SL_STATUS_OK)
	    		{
	    			PRINTF("WF200_TESTING_MODE: Signal Strength = %i\n", rcpi);
	    		}
	    		break;

	    	case WF200_TEST_SET_MAX_TX_POWER:
	    		WF200_DriverStatus = sl_wfx_set_max_tx_power(max_tx_power, interface);
	    		break;

	    	case WF200_TEST_GET_MAX_TX_POWER:
	    		WF200_DriverStatus = sl_wfx_get_max_tx_power(&max_tx_power_rf_port_1,
	    		                                             &max_tx_power_rf_port_2,
													         SL_WFX_STA_INTERFACE);

				if(WF200_DriverStatus == SL_STATUS_OK)
	    		{
	    			PRINTF("WF200_TESTING_MODE: MAX TX POWER, Port1 = %i, Port2 = %i, Interface Used = %i\n",
	    					max_tx_power_rf_port_1,
							max_tx_power_rf_port_2,
							SL_WFX_STA_INTERFACE);
	    		}
	    		break;

	    	case WF200_TEST_EXIT_TESTS:
	    		Exit = true;
	    		break;

	    	default:
    			PRINTF("WF200 TESTING MODE: Unknown Test\n");
    			WF200_DEBUG_TRAP;
	    		break;
	    	}

	    	// Catch error cases
	    	if(WF200_DriverStatus != SL_STATUS_OK)
	    	{
	    		PRINTF("WF200 TESTING MODE: ERROR - Test Number = %i, WF200_DriverStatus = %i\n",
	    				RxBuffer[0],
						WF200_DriverStatus);
	    		WF200_DEBUG_TRAP;
	    	}
	    }
	}

	WF200_DeleteTask(WF200_TEST_MODE_TASK);
	PRINTF("WF200 TEST MODE TASK BEEN DELETED\n");
}


void httpsclient_task(void *arg)
{
	PRINTF("\n\nSTART HTTPS\n");

	https_client_tls_init();

	PRINTF("DELETE HTTPS\n\n");

    vTaskDelete(NULL);
}


//////////////////////////////////////////////////////////////////////////////////
// !!! TEMPORARY INPUT TASK UNTIL THE TEST FRAMEWORK/CLI HAS BEEN DECIDED UPON !!!
void TempInputTask(void *arg)
{
	uint8_t *ptr;
	BaseType_t sendStatus;
	const WF200_ControlInfo_t *pControlInfo = WF200_GetControlInfoPtr();

	if(pControlInfo == NULL)
	{
		PRINTF("WF200_TEST_ModeTask: Error NULL Ptr\n");
		WF200_DEBUG_TRAP;
	}

	PRINTF("WF200 TEMP INPUT TASK RUNNING\r\n");

	vTaskDelay(5000);


	//////////////////
	TestNumber = WF200_TEST_START_AP;
	ptr = &TestNumber;
	sendStatus = xQueueSend(pControlInfo->WF200_TaskInfo[WF200_TEST_MODE_TASK].QueueHandle,
			                &ptr, 10);

	if(sendStatus != pdPASS)
	{
	   PRINTF("TempInputTask: QUEUE SEND FAIL\n");
	   WF200_DEBUG_TRAP;
	}

	vTaskDelay(7500);

	//////////////////
	TestNumber = WF200_TEST_STOP_AP;
	ptr = &TestNumber;
	sendStatus = xQueueSend(pControlInfo->WF200_TaskInfo[WF200_TEST_MODE_TASK].QueueHandle,
			                &ptr, 10);

	if(sendStatus != pdPASS)
	{
	   PRINTF("TempInputTask: QUEUE SEND FAIL\n");
	   WF200_DEBUG_TRAP;
	}

	vTaskDelay(7500);


	//////////////////
	TestNumber = WF200_TEST_START_AP;
	ptr = &TestNumber;
	sendStatus = xQueueSend(pControlInfo->WF200_TaskInfo[WF200_TEST_MODE_TASK].QueueHandle,
			                &ptr, 10);

	if(sendStatus != pdPASS)
	{
	   PRINTF("TempInputTask: QUEUE SEND FAIL\n");
	   WF200_DEBUG_TRAP;
	}

	vTaskDelay(7500);

	//////////////////
	TestNumber = WF200_TEST_STOP_AP;
	ptr = &TestNumber;
	sendStatus = xQueueSend(pControlInfo->WF200_TaskInfo[WF200_TEST_MODE_TASK].QueueHandle,
			                &ptr, 10);

	if(sendStatus != pdPASS)
	{
	   PRINTF("TempInputTask: QUEUE SEND FAIL\n");
	   WF200_DEBUG_TRAP;
	}

	vTaskDelay(7500);

	//////////////////
	TestNumber = WF200_TEST_CREATE_STA_MODE_TASK;
	ptr = &TestNumber;
	sendStatus = xQueueSend(pControlInfo->WF200_TaskInfo[WF200_TEST_MODE_TASK].QueueHandle,
			                &ptr, 10);

	if(sendStatus != pdPASS)
	{
	   PRINTF("TempInputTask: QUEUE SEND FAIL\n");
	   WF200_DEBUG_TRAP;
	}

	vTaskDelay(7500);


	//////////////////
	TestNumber = WF200_TEST_MBEDTLS_TEST;
	ptr = &TestNumber;
	sendStatus = xQueueSend(pControlInfo->WF200_TaskInfo[WF200_TEST_MODE_TASK].QueueHandle,
			                &ptr, 10);

	if(sendStatus != pdPASS)
	{
	   PRINTF("TempInputTask: QUEUE SEND FAIL\n");
	   WF200_DEBUG_TRAP;
	}

	vTaskDelay(15000);


	//////////////////
	TestNumber = WF200_TEST_CREATE_EVENT_TASK;
	ptr = &TestNumber;
	sendStatus = xQueueSend(pControlInfo->WF200_TaskInfo[WF200_TEST_MODE_TASK].QueueHandle,
			                &ptr, 10);

	if(sendStatus != pdPASS)
	{
	   PRINTF("TempInputTask: QUEUE SEND FAIL\n");
	   WF200_DEBUG_TRAP;
	}

	vTaskDelay(15000);


	//////////////////
	TestNumber = WF200_TEST_GET_SIGNAL_STRENGTH;
	ptr = &TestNumber;
	sendStatus = xQueueSend(pControlInfo->WF200_TaskInfo[WF200_TEST_MODE_TASK].QueueHandle,
			                &ptr, 10);

	if(sendStatus != pdPASS)
	{
	   PRINTF("TempInputTask: QUEUE SEND FAIL\n");
	   WF200_DEBUG_TRAP;
	}

	vTaskDelay(7500);


	//////////////////
	TestNumber = WF200_TEST_GET_MAX_TX_POWER;
	ptr = &TestNumber;
	sendStatus = xQueueSend(pControlInfo->WF200_TaskInfo[WF200_TEST_MODE_TASK].QueueHandle,
			                &ptr, 10);

	if(sendStatus != pdPASS)
	{
	   PRINTF("TempInputTask: QUEUE SEND FAIL\n");
	   WF200_DEBUG_TRAP;
	}

	vTaskDelay(7500);


	//////////////////
	TestNumber = WF200_TEST_SHUTDOWN;
	ptr = &TestNumber;
	sendStatus = xQueueSend(pControlInfo->WF200_TaskInfo[WF200_TEST_MODE_TASK].QueueHandle,
			                &ptr, 10);

	if(sendStatus != pdPASS)
	{
	   PRINTF("TempInputTask: QUEUE SEND FAIL\n");
	   WF200_DEBUG_TRAP;
	}

	vTaskDelay(15000);


	//////////////////
	TestNumber = WF200_TEST_CREATE_COMMS_TASK;
	ptr = &TestNumber;
	sendStatus = xQueueSend(pControlInfo->WF200_TaskInfo[WF200_TEST_MODE_TASK].QueueHandle,
			                &ptr, 10);

	if(sendStatus != pdPASS)
	{
	   PRINTF("TempInputTask: QUEUE SEND FAIL\n");
	   WF200_DEBUG_TRAP;
	}

	vTaskDelay(15000);

	//////////////////
	TestNumber = WF200_TEST_CREATE_EVENT_TASK;
	ptr = &TestNumber;
	sendStatus = xQueueSend(pControlInfo->WF200_TaskInfo[WF200_TEST_MODE_TASK].QueueHandle,
			                &ptr, 10);

	if(sendStatus != pdPASS)
	{
	   PRINTF("TempInputTask: QUEUE SEND FAIL\n");
	   WF200_DEBUG_TRAP;
	}

	vTaskDelay(15000);


	//////////////////
	TestNumber = WF200_TEST_CREATE_INIT_TASK;
	ptr = &TestNumber;
	sendStatus = xQueueSend(pControlInfo->WF200_TaskInfo[WF200_TEST_MODE_TASK].QueueHandle,
			                &ptr, 10);

	if(sendStatus != pdPASS)
	{
	   PRINTF("TempInputTask: QUEUE SEND FAIL\n");
	   WF200_DEBUG_TRAP;
	}

	vTaskDelay(15000);


	//////////////////
	TestNumber = WF200_TEST_CREATE_STA_MODE_TASK;
	ptr = &TestNumber;
	sendStatus = xQueueSend(pControlInfo->WF200_TaskInfo[WF200_TEST_MODE_TASK].QueueHandle,
			                &ptr, 10);

	if(sendStatus != pdPASS)
	{
	   PRINTF("TempInputTask: QUEUE SEND FAIL\n");
	   WF200_DEBUG_TRAP;
	}

	vTaskDelay(15000);

	//////////////////
	TestNumber = WF200_TEST_MBEDTLS_TEST;
	ptr = &TestNumber;
	sendStatus = xQueueSend(pControlInfo->WF200_TaskInfo[WF200_TEST_MODE_TASK].QueueHandle,
			                &ptr, 10);

	if(sendStatus != pdPASS)
	{
	   PRINTF("TempInputTask: QUEUE SEND FAIL\n");
	   WF200_DEBUG_TRAP;
	}

	vTaskDelay(15000);

	TestNumber = WF200_TEST_CREATE_EVENT_TASK;
	ptr = &TestNumber;
	sendStatus = xQueueSend(pControlInfo->WF200_TaskInfo[WF200_TEST_MODE_TASK].QueueHandle,
			                &ptr, 10);

	if(sendStatus != pdPASS)
	{
	   PRINTF("TempInputTask: QUEUE SEND FAIL\n");
	   WF200_DEBUG_TRAP;
	}

	vTaskDelay(15000);


	//////////////////
	WF200_Set_STA_CommandedDisconnect(false);
	TestNumber = WF200_TEST_STA_DISCONNECT;
	ptr = &TestNumber;
	sendStatus = xQueueSend(pControlInfo->WF200_TaskInfo[WF200_TEST_MODE_TASK].QueueHandle,
			                &ptr, 10);

	if(sendStatus != pdPASS)
	{
	   PRINTF("TempInputTask: QUEUE SEND FAIL\n");
	   WF200_DEBUG_TRAP;
	}

	vTaskDelay(15000);


	//////////////////
	WF200_Set_STA_CommandedDisconnect(true);
	TestNumber = WF200_TEST_STA_DISCONNECT;
	ptr = &TestNumber;
	sendStatus = xQueueSend(pControlInfo->WF200_TaskInfo[WF200_TEST_MODE_TASK].QueueHandle,
			                &ptr, 10);

	if(sendStatus != pdPASS)
	{
	   PRINTF("TempInputTask: QUEUE SEND FAIL\n");
	   WF200_DEBUG_TRAP;
	}

	vTaskDelay(15000);

	//////////////////
	TestNumber = WF200_TEST_INPUT_STA_MODE_NETWORK_DETAILS;
	ptr = &STA_Mode_InputDetails[0];
	sendStatus = xQueueSend(pControlInfo->WF200_TaskInfo[WF200_TEST_MODE_TASK].QueueHandle,
			                &ptr, 10);

	if(sendStatus != pdPASS)
	{
	   PRINTF("TempInputTask: QUEUE SEND FAIL\n");
	   WF200_DEBUG_TRAP;
	}

	vTaskDelay(15000);

	//////////////////
	TestNumber = WF200_TEST_CREATE_STA_MODE_TASK;
	ptr = &TestNumber;
	sendStatus = xQueueSend(pControlInfo->WF200_TaskInfo[WF200_TEST_MODE_TASK].QueueHandle,
			                &ptr, 10);

	if(sendStatus != pdPASS)
	{
	   PRINTF("TempInputTask: QUEUE SEND FAIL\n");
	   WF200_DEBUG_TRAP;
	}

	vTaskDelay(15000);


#if 0
	testNumber = WF200_TEST_INPUT_AP_MODE_DETAILS;
	ptr = &AP_Mode_InputDetails[0];
	sendStatus = xQueueSend(pControlInfo->WF200_TaskInfo[WF200_TEST_MODE_TASK].QueueHandle,
			                &ptr, 10);

	if(sendStatus != pdPASS)
	{
	   PRINTF("TempInputTask: QUEUE SEND FAIL\n");
	   WF200_DEBUG_TRAP;
	}

	vTaskDelay(15000);


	//////////////////
	TestNumber = WF200_TEST_START_AP;
	ptr = &TestNumber;
	sendStatus = xQueueSend(pControlInfo->WF200_TaskInfo[WF200_TEST_MODE_TASK].QueueHandle,
			                &ptr, 10);

	if(sendStatus != pdPASS)
	{
	   PRINTF("TempInputTask: QUEUE SEND FAIL\n");
	   WF200_DEBUG_TRAP;
	}
#endif

	 vTaskDelete(NULL);
}


