/*******************************************************************************
 * File Name: main.c
 *
 * Description: This file contains the starting point of Bluetooth LE environment sensing
 *              service application.
 *
 * Related Document: See README.md
 *
 *
 *********************************************************************************
 Copyright 2020-2022, Cypress Semiconductor Corporation (an Infineon company) or
 an affiliate of Cypress Semiconductor Corporation.  All rights reserved.

 This software, including source code, documentation and related
 materials ("Software") is owned by Cypress Semiconductor Corporation
 or one of its affiliates ("Cypress") and is protected by and subject to
 worldwide patent protection (United States and foreign),
 United States copyright laws and international treaty provisions.
 Therefore, you may use this Software only as provided in the license
 agreement accompanying the software package from which you
 obtained this Software ("EULA").
 If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 non-transferable license to copy, modify, and compile the Software
 source code solely for use in connection with Cypress's
 integrated circuit products.  Any reproduction, modification, translation,
 compilation, or representation of this Software except as specified
 above is prohibited without the express written permission of Cypress.

 Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 reserves the right to make changes to the Software without notice. Cypress
 does not assume any liability arising out of the application or use of the
 Software or any product or circuit described in the Software. Cypress does
 not authorize its products for use in any products where a malfunction or
 failure of the Cypress product may reasonably be expected to result in
 significant property damage, injury or death ("High Risk Product"). By
 including Cypress's product in a High Risk Product, the manufacturer
 of such system or application assumes all risk of such use and in doing
 so agrees to indemnify Cypress against all liability.
 *******************************************************************************/


/*******************************************************************************
 *        Header Files
 *******************************************************************************/
#include "cybsp.h"
#include "cy_retarget_io.h"
#include "cybt_platform_trace.h"
#include "cyhal.h"
#include "cyhal_gpio.h"
#include "stdio.h"
#include "cyabs_rtos.h"
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <string.h>
#include <timers.h>
#include "GeneratedSource/cycfg_gatt_db.h"
#include "app_bt_gatt_handler.h"
#include "app_bt_utils.h"
#include "wiced_bt_ble.h"
#include "wiced_bt_uuid.h"
#include "wiced_memory.h"
#include "wiced_bt_stack.h"
#include "cycfg_bt_settings.h"
#include "cycfg_gap.h"
#include "cybsp_bt_config.h"
/*******************************************************************************
 *        Macro Definitions
 *******************************************************************************/

/* This is the temperature measurement interval which is same as configured in
 * the BT Configurator - The variable represents interval in milliseconds.
 */
#define POLL_TIMER_IN_MSEC              (49999u)
#define POLL_TIMER_FREQ                 (10000)
/* Temperature Simulation Constants */
#define DEFAULT_TEMPERATURE             (2500u)
#define MAX_TEMPERATURE_LIMIT           (3000u)
#define MIN_TEMPERATURE_LIMIT           (2000u)
#define DELTA_TEMPERATURE               (100u)

/* Number of advertisment packet */
#define NUM_ADV_PACKETS                 (3u)

/* Absolute value of an integer. The absolute value is always positive. */
#ifndef ABS
#define ABS(N) ((N<0) ? (-N) : (N))
#endif

/* Check if notification is enabled for a valid connection ID */
#define IS_NOTIFIABLE(conn_id, cccd) (((conn_id)!= 0)? (cccd) & GATT_CLIENT_CONFIG_NOTIFICATION: 0)

/******************************************************************************
 *                                 TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 *        Variable Definitions
 *******************************************************************************/
/* Configuring Higher priority for the application */
volatile int uxTopUsedPriority;

/* Manages runtime configuration of Bluetooth stack */
extern const wiced_bt_cfg_settings_t wiced_bt_cfg_settings;

/* FreeRTOS variable to store handle of task created to update and send dummy
   values of temperature */
TaskHandle_t ess_task_handle;

/* Status variable for connection ID */
uint16_t app_bt_conn_id;

/* Dummy Room Temperature */
int16_t temperature = DEFAULT_TEMPERATURE;
uint8_t alternating_flag = 0;

/* Variable for 5 sec timer object */
static cyhal_timer_t ess_timer_obj;
/* Configure timer for 5 sec */
const cyhal_timer_cfg_t ess_timer_cfg =
    {
        .compare_value = 0,                    /* Timer compare value, not used */
        .period = POLL_TIMER_IN_MSEC, /* Defines the timer period */
        .direction = CYHAL_TIMER_DIR_UP,       /* Timer counts up */
        .is_compare = false,                   /* Don't use compare mode */
        .is_continuous = true,                 /* Run timer indefinitely */
        .value = 0                             /* Initial value of counter */
};
/*******************************************************************************
 *        Function Prototypes
 *******************************************************************************/

/* Callback function for Bluetooth stack management type events */
static wiced_bt_dev_status_t
app_bt_management_callback(wiced_bt_management_evt_t event,
                           wiced_bt_management_evt_data_t *p_event_data);

/* This function sets the advertisement data */
static wiced_result_t app_bt_set_advertisement_data(void);

/* This function initializes the required BLE ESS & thermistor */
static void bt_app_init(void);

/* Task to send notifications with dummy temperature values */
void ess_task(void *pvParam);
/* HAL timer callback registered when timer reaches terminal count */
void ess_timer_callb(void *callback_arg, cyhal_timer_event_t event);

/* This function starts the advertisements */
static void app_start_advertisement(void);

/******************************************************************************
 *                          Function Definitions
 ******************************************************************************/

/*
 *  Entry point to the application. Set device configuration and start BT
 *  stack initialization.  The actual application initialization will happen
 *  when stack reports that BT device is ready.
 */
int main(void)
{
    uxTopUsedPriority = configMAX_PRIORITIES - 1;
    wiced_result_t wiced_result;
    BaseType_t rtos_result;

    /* Initialize and Verify the BSP initialization */
    CY_ASSERT(CY_RSLT_SUCCESS == cybsp_init());

    /* Enable global interrupts */
    __enable_irq();

    /* Initialize retarget-io to use the debug UART port */
    cy_retarget_io_init(CYBSP_DEBUG_UART_TX,
                        CYBSP_DEBUG_UART_RX,
                        CY_RETARGET_IO_BAUDRATE);

    /* Initialising the HCI UART for Host contol */
    cybt_platform_config_init(&cybsp_bt_platform_cfg);

    /* Debug logs on UART port */

    printf("****** Environmental Sensing Service ******\n");

    /* Register call back and configuration with stack */
    wiced_result = wiced_bt_stack_init(app_bt_management_callback,
                                 &wiced_bt_cfg_settings);

    /* Check if stack initialization was successful */
    if (WICED_BT_SUCCESS == wiced_result) {
        printf("Bluetooth Stack Initialization Successful \n");
    } else {
        printf("Bluetooth Stack Initialization failed!!\n");
    }

    rtos_result = xTaskCreate(ess_task, "ESS Task", (configMINIMAL_STACK_SIZE * 4),
                                        NULL, (configMAX_PRIORITIES - 3), &ess_task_handle);
    if(pdPASS == rtos_result)
    {
        printf("ESS task created successfully\n");
    }
    else
    {
        printf("ESS task creation failed\n");
    }

    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler();

    /* Should never get here */
    CY_ASSERT(0);
}

/*
 * Function Name: app_bt_management_callback()
 *
 *@brief
 *  This is a Bluetooth stack event handler function to receive management events
 *  from the Bluetooth LE stack and process as per the application.
 *
 * @param wiced_bt_management_evt_t  Bluetooth LE event code of one byte length
 * @param wiced_bt_management_evt_data_t  Pointer to Bluetooth LE management event
 *                                        structures
 *
 * @return wiced_result_t Error code from WICED_RESULT_LIST or BT_RESULT_LIST
 *
 */
static wiced_result_t
app_bt_management_callback(wiced_bt_management_evt_t event,
                           wiced_bt_management_evt_data_t *p_event_data)
{
    wiced_bt_dev_status_t status = WICED_ERROR;


    switch (event) {

    case BTM_ENABLED_EVT:
    {
        printf("\nThis application implements Bluetooth LE Environmental Sensing\n"
                "Service and sends dummy temperature values in Celsius\n"
                "every %d milliseconds over Bluetooth\n", (POLL_TIMER_IN_MSEC));

        printf("Discover this device with the name:%s\n", app_gap_device_name);

        print_local_bd_address();

        printf("\n");
        printf("Bluetooth Management Event: \t");
        printf("%s", get_btm_event_name(event));
        printf("\n");

        /* Perform application-specific initialization */
        bt_app_init();
    }break;

    case BTM_DISABLED_EVT:
        /* Bluetooth Controller and Host Stack Disabled */
        printf("\n");
        printf("Bluetooth Management Event: \t");
        printf("%s", get_btm_event_name(event));
        printf("\n");
        printf("Bluetooth Disabled\n");
        break;

    case BTM_BLE_ADVERT_STATE_CHANGED_EVT:
    {
        wiced_bt_ble_advert_mode_t *p_adv_mode = &p_event_data->ble_advert_state_changed;
        /* Advertisement State Changed */
        printf("\n");
        printf("Bluetooth Management Event: \t");
        printf("%s", get_btm_event_name(event));
        printf("\n");
        printf("\n");
        printf("Advertisement state changed to ");
        printf("%s", get_btm_advert_mode_name(*p_adv_mode));
        printf("\n");
    }break;

    default:
        printf("\nUnhandled Bluetooth Management Event: %d %s\n",
                event,
                get_btm_event_name(event));
        break;
    }

    return (status);
}

/*
 Function name:
 bt_app_init

 Function Description:
 @brief    This function is executed if BTM_ENABLED_EVT event occurs in
           Bluetooth management callback.

 @param    void

 @return    void
 */
static void bt_app_init(void)
{
    wiced_bt_gatt_status_t gatt_status = WICED_BT_GATT_ERROR;
    cy_rslt_t rslt;

    /* Register with stack to receive GATT callback */
    gatt_status = wiced_bt_gatt_register(app_bt_gatt_event_callback);
    printf("\n gatt_register status:\t%s\n",get_gatt_status_name(gatt_status));

    /* Initialize the User LED */
    cyhal_gpio_init(CONNECTION_LED,
                    CYHAL_GPIO_DIR_OUTPUT,
                    CYHAL_GPIO_DRIVE_STRONG,
                    CYBSP_LED_STATE_OFF);

    /* Initialize the HAL timer used to count 5 seconds */
    rslt = cyhal_timer_init(&ess_timer_obj, NC, NULL);
    if (CY_RSLT_SUCCESS != rslt)
    {
        printf("ESS timer init failed !\n");
    }
    /* Configure the timer for 5 seconds */
    cyhal_timer_configure(&ess_timer_obj, &ess_timer_cfg);
    rslt = cyhal_timer_set_frequency(&ess_timer_obj, POLL_TIMER_FREQ);
    if (CY_RSLT_SUCCESS != rslt)
    {
        printf("ESS timer set freq failed !\n");
    }
    /* Register for a callback whenever timer reaches terminal count */
    cyhal_timer_register_callback(&ess_timer_obj, ess_timer_callb, NULL);
    cyhal_timer_enable_event(&ess_timer_obj, CYHAL_TIMER_IRQ_TERMINAL_COUNT, 3, true);

    /* Start the timer */
    if (CY_RSLT_SUCCESS != cyhal_timer_start(&ess_timer_obj))
    {
        printf("ESS timer start failed !");
    }

    /* Initialize GATT Database */
    gatt_status = wiced_bt_gatt_db_init(gatt_database, gatt_database_len, NULL);
    if (WICED_BT_GATT_SUCCESS != gatt_status) {
        printf("\n GATT DB Initialization not successful err 0x%x\n", gatt_status);
    }

    /* Start Bluetooth LE advertisements */
    app_start_advertisement();

}


/**
 * @brief This function starts the Blueooth LE advertisements and describes
 *        the pairing support
 */
static void app_start_advertisement(void)
{
    wiced_result_t wiced_status;

    /* Set Advertisement Data */
    wiced_status = app_bt_set_advertisement_data();
    if (WICED_SUCCESS != wiced_status) {
        printf("Raw advertisement failed err 0x%x\n", wiced_status);
    }

    /* Do not allow peer to pair */
    wiced_bt_set_pairable_mode(WICED_FALSE, FALSE);

    /* Start Undirected LE Advertisements on device startup. */
    wiced_status = wiced_bt_start_advertisements(BTM_BLE_ADVERT_UNDIRECTED_HIGH,
                                                 BLE_ADDR_PUBLIC,
                                                 NULL);

    if (WICED_SUCCESS != wiced_status) {
        printf( "Starting undirected Bluetooth LE advertisements"
                "Failed err 0x%x\n", wiced_status);
    }
}

/*
 Function Name:
 app_bt_set_advertisement_data

 Function Description:
 @brief  Set Advertisement Data

 @param void

 @return wiced_result_t WICED_SUCCESS or WICED_failure
 */
static wiced_result_t app_bt_set_advertisement_data(void)
{

    wiced_result_t wiced_result = WICED_SUCCESS;
    wiced_result = wiced_bt_ble_set_raw_advertisement_data( NUM_ADV_PACKETS,
                                                            cy_bt_adv_packet_data);

    return (wiced_result);
}

/*
 Function name:
 ess_timer_callb

 Function Description:
 @brief  This callback function is invoked on timeout of 5 seconds timer.

 @param  void*: unused
 @param cyhal_timer_event_t: unused

 @return void
 */
void ess_timer_callb(void *callback_arg, cyhal_timer_event_t event)
{
    BaseType_t xHigherPriorityTaskWoken;
    xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(ess_task_handle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/*
 Function name:
 ess_task

 Function Description:
 @brief  This task updates dummy temperature value every time it is notified
         and sends a notification to the connected peer

 @param  void*: unused

 @return void
 */
void ess_task(void *pvParam)
{
    while(true)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        /* Varying temperature by 1 degree on every timeout for simulation */
        if (0 == alternating_flag)
        {
            temperature += DELTA_TEMPERATURE;
            if (MAX_TEMPERATURE_LIMIT <= temperature)
            {
                alternating_flag = 1;
            }
        }
        else if ((1 == alternating_flag))
        {
            temperature -= DELTA_TEMPERATURE;
            if (MIN_TEMPERATURE_LIMIT >= temperature)
            {
                alternating_flag = 0;
            }
        }

        printf("\nTemperature (in degree Celsius) \t\t%d.%02d\n",
                (temperature / 100), ABS(temperature % 100));

        /*
        * app_ess_temperature value is set both for read operation and
        * notify operation.
        */
        app_ess_temperature[0] = (uint8_t)(temperature & 0xff);
        app_ess_temperature[1] = (uint8_t)((temperature >> 8) & 0xff);

        /* To check that connection is up and
        * client is registered to receive notifications
        * to send temperature data in Little Endian Format
        * as per BT SIG's ESS Specification
        */

        if (IS_NOTIFIABLE (app_bt_conn_id, app_ess_temperature_client_char_config[0]) == 0)
        {
            if(!app_bt_conn_id)
            {
                printf("This device is not connected to a central device\n");
            }else{
                printf("This device is connected to a central device but\n"
                        "GATT client notifications are not enabled\n");
            }
        }
        else
        {
            wiced_bt_gatt_status_t gatt_status;

            /*
            * Sending notification, set the pv_app_context to NULL, since the
            * data 'app_ess_temperature' is not to be freed
            */
            gatt_status = wiced_bt_gatt_server_send_notification(app_bt_conn_id,
                                                                    HDLC_ESS_TEMPERATURE_VALUE,
                                                                    app_ess_temperature_len,
                                                                    app_ess_temperature,
                                                                    NULL);

            printf("Sent notification status 0x%x\n", gatt_status);

        }
    }
}

/* [] END OF FILE */
