/*******************************************************************************
* File Name: main.c
*
* Description:
* This file contains the starting point of BLE environment sensing application.
* The wiced_bt_stack_init() registers for Bluetooth stack in this main function.
* The Bluetooth Management callback manages the Bluetooth events and the
* application developer can customize the functionality and behavior based on
* the Bluetooth events. The Bluetooth Management event acts like a
* Finite State Machine (FSM) for the PSoC application.
*
* Related Document: See README.md
*
********************************************************************************
* (c) 2020, Cypress Semiconductor Corporation. All rights reserved.
********************************************************************************
* This software, including source code, documentation and related materials
* ("Software"), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries ("Cypress") and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software ("EULA").
*
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress's integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer of such
* system or application assumes all risk of such use and in doing so agrees to
* indemnify Cypress against all liability.
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
#include "GeneratedSource/cycfg_gatt_db.h"
#include "app_bt_gatt_handler.h"
#include "app_bt_utils.h"
#include "app_bt_cfg.h"
#include "wiced_bt_ble.h"
#include "wiced_bt_uuid.h"
#include "wiced_memory.h"
#include "wiced_bt_stack.h"

/*******************************************************************************
*        Macro Definitions
*******************************************************************************/

/* This is the temperature measurement interval which is same as configured in
 * the BT Configurator - The variable represents interval in seconds.
 */
#define POLL_TIMER_IN_MS                (5000u)

/* Temperature Simulation Constants */
#define DEFAULT_TEMPERATURE             (2500u)
#define MAX_TEMPERATURE_LIMIT           (3000u)
#define MIN_TEMPERATURE_LIMIT           (2000u)
#define DELTA_TEMPERATURE               (100u)

/* Absolute value of an integer. The absolute value is always positive. */
#ifndef ABS
#define ABS(N) ((N<0) ? (-N) : (N))
#endif

/*******************************************************************************
*        Variable Definitions
*******************************************************************************/
/* Manages runtime configuration of Bluetooth stack */
extern const wiced_bt_cfg_settings_t wiced_bt_cfg_settings;

/* This cy_timer changes for every millisecond */
static cy_timer_t            seconds_timer;

/* Status variable for connection ID */
uint16_t                        app_bt_conn_id;

/* Dummy Room Temperature */
int16_t temperature = DEFAULT_TEMPERATURE;
uint8_t alternating_flag = 0;

/*******************************************************************************
*        Function Prototypes
*******************************************************************************/

/* Callback function for Bluetooth stack management type events */
static wiced_bt_dev_status_t
app_bt_management_callback(wiced_bt_management_evt_t event,
                           wiced_bt_management_evt_data_t *p_event_data);

/* This function sets the dummy advertisement data */
static wiced_result_t app_bt_set_advertisement_data(void);

/* This function initializes the required BLE ESS & thermistor */
static void bt_app_init(void);

/* This is a timer invoked callback function that is invoked in every timeout */
static void seconds_timer_temperature_cb(uint32_t arg);

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
    cy_rslt_t result;

    /* Initialize the board support package */
    result = cybsp_init();

    /* Verify the BSP initialization */
    CY_ASSERT(CY_RSLT_SUCCESS == result);

    /* Enable global interrupts */
    __enable_irq();

    /* Initialize retarget-io to use the debug UART port */
    cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX        \
                                           , CY_RETARGET_IO_BAUDRATE);

    /* Initialising the HCI UART for Host contol */
    cybt_platform_config_init(&bt_platform_cfg_settings);

    /* Debug logs on UART port */
    printf("**********AnyCloud Example*****************\n");

    printf("****** Environmental Sensing Service ******\n");

    /* Register call back and configuration with stack */
    result = wiced_bt_stack_init (app_bt_management_callback     \
                                 , &wiced_bt_cfg_settings);

    /* Check if stack initialization was successful */
    if( WICED_BT_SUCCESS == result)
    {
        printf("Bluetooth Stack Initialization Successful \n");
    }
    else
    {
        printf("Bluetooth Stack Initialization failed!!\n");
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
*  from the BLE stack and process as per the application.
*
* @param wiced_bt_management_evt_t  BLE event code of one byte length
* @param wiced_bt_management_evt_data_t  Pointer to BLE management event
*                                        structures
*
* @return wiced_result_t Error code from WICED_RESULT_LIST or BT_RESULT_LIST
*
*/
static wiced_result_t
app_bt_management_callback(wiced_bt_management_evt_t event,
                           wiced_bt_management_evt_data_t *p_event_data)
{
    wiced_bt_dev_status_t       status              = WICED_SUCCESS;
    wiced_bt_ble_advert_mode_t *p_adv_mode          = NULL;
    wiced_bt_device_address_t  local_device_bd_addr = {0};

    switch (event)
    {

    case BTM_ENABLED_EVT:
        printf( \
            "\nThis application implements BLE Environmental Sensing\n"
            "\nService and sends dummy temperature values in Celsius\n"
            "\nevery %d seconds over BLE \n", (POLL_TIMER_IN_MS/1000) );

        printf("\nDiscover this device with the name:%s\n", app_gap_device_name);

        wiced_bt_dev_read_local_addr(local_device_bd_addr);

        printf("\nBluetooth Device Address: ");
        print_bd_address(local_device_bd_addr);

        printf("\n");
        printf("Bluetooth Management Event: \t");
        printf(get_btm_event_name(event));
        printf("\n");

        /* Perform application-specific initialization */
        bt_app_init();
        break;

    case BTM_DISABLED_EVT:
        /* Bluetooth Controller and Host Stack Disabled */
        printf("\n");
        printf("Bluetooth Management Event: \t");
        printf(get_btm_event_name(event));
        printf("\n");
        printf("Bluetooth Disabled\n");
        break;

    case BTM_BLE_ADVERT_STATE_CHANGED_EVT:
        /* Advertisement State Changed */
        printf("\n");
        printf("Bluetooth Management Event: \t");
        printf(get_btm_event_name(event));
        printf("\n");
        p_adv_mode = &p_event_data->ble_advert_state_changed;
        printf("\n");
        printf("Advertisement state changed to ");
        printf(get_btm_advert_mode_name(*p_adv_mode));
        printf("\n");
        break;

    default:
        printf("\nUnhandled Bluetooth Management Event\n");
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

    wiced_bt_gatt_status_t status = WICED_BT_GATT_BUSY;

    /* Register with stack to receive GATT callback */
    status = wiced_bt_gatt_register(app_bt_event_handler);
    printf("\nGATT status:\t");
    printf(get_gatt_status_name(status));
    printf("\n");

    /* Initialize the User LED */
    cyhal_gpio_init(CONNECTION_LED, CYHAL_GPIO_DIR_OUTPUT,
                     CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);

    /*
     * The thermistor_init() is a dummy function here that should be replaced by
     * the actual hardware initialization such as ADC or I2C interface that
     * interfaces with the sensor
     */
    //  thermistor_init();

    /* Initialize GATT Database */
    if(WICED_BT_GATT_SUCCESS != wiced_bt_gatt_db_init(gatt_database,
                                                      gatt_database_len,
                                                      NULL))
    {
        printf("\n GATT DB Initialization not successful\n");
    }

    /* Starting the MILLI_SECONDS timer for every POLL_TIMER_IN_MS*/
    if ( CY_RSLT_SUCCESS == cy_rtos_init_timer( &seconds_timer,
                                                CY_TIMER_TYPE_PERIODIC,
                                                seconds_timer_temperature_cb,
                                                0))
    {
        if ( CY_RSLT_SUCCESS != cy_rtos_start_timer(&seconds_timer,
                                                    POLL_TIMER_IN_MS))
        {
            printf("Seconds Timer Error\n");
        }
    }

    /* Set Advertisement Data */
    if(WICED_SUCCESS != app_bt_set_advertisement_data())
    {
        printf("Raw advertisement failed\n");
    }

    /* Do not allow peer to pair */
    wiced_bt_set_pairable_mode(WICED_FALSE, TRUE);

    /* Start Undirected LE Advertisements on device startup. */
    if(WICED_SUCCESS !=
        wiced_bt_start_advertisements(BTM_BLE_ADVERT_UNDIRECTED_HIGH,
                                      BLE_ADDR_PUBLIC,
                                      NULL))
    {
        printf("Starting undirected BLE advertisements Failed\n");
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

    wiced_result_t result = WICED_SUCCESS;
    wiced_bt_ble_advert_elem_t adv_elem[3] = { 0 };

    uint8_t num_elem        = 0;
    uint8_t flag            = (BTM_BLE_GENERAL_DISCOVERABLE_FLAG | \
                              BTM_BLE_BREDR_NOT_SUPPORTED);
    // uint8_t appearance_data = FROM_BIT16_TO_8(APPEARANCE_GENERIC_THERMOMETER);
    uint8_t uuid_data     = FROM_BIT16_TO_8(UUID_SERVICE_ENVIRONMENTAL_SENSING);

    /* Advertisement Element for Advertisement Flags */
    adv_elem[num_elem].advert_type = BTM_BLE_ADVERT_TYPE_FLAG;
    adv_elem[num_elem].len         = sizeof(uint8_t);
    adv_elem[num_elem].p_data      = &flag;
    num_elem++;

    /* Advertisement Element for Name */
    adv_elem[num_elem].advert_type = BTM_BLE_ADVERT_TYPE_NAME_COMPLETE;
    adv_elem[num_elem].len         = app_gap_device_name_len;
    adv_elem[num_elem].p_data      = app_gap_device_name;
    num_elem++;

    /* Advertisement Element for Appearance */
    adv_elem[num_elem].advert_type = BTM_BLE_ADVERT_TYPE_FLAG;
    adv_elem[num_elem].len         = sizeof(
                        FROM_BIT16_TO_8(UUID_SERVICE_ENVIRONMENTAL_SENSING));
    adv_elem[num_elem].p_data      = &uuid_data;
    num_elem++;

    result = wiced_bt_ble_set_raw_advertisement_data(num_elem, adv_elem);

    return (result);
}

/*
 Function name:
 seconds_timer_temperature_cb

 Function Description:
 @brief  This callback function is invoked on timeout of seconds timer.

 @param  arg

 @return void
 */
static void seconds_timer_temperature_cb(uint32_t arg)
{

    /* Varying temperature by 1 degree on every timeout for simulation */
    if(0 == alternating_flag)
    {
        temperature += DELTA_TEMPERATURE;
        if(MAX_TEMPERATURE_LIMIT <= temperature)
        {
            alternating_flag = 1;
        }
    }
    else if ( (1 == alternating_flag) )
    {
        temperature -= DELTA_TEMPERATURE;
        if(MIN_TEMPERATURE_LIMIT >= temperature)
        {
            alternating_flag = 0;
        }
    }

    printf("\nTemperature (in degree Celsius) \t\t%d.%02d\n",
                    (temperature / 100),
                    ABS(temperature % 100));

    /*
     * app_ess_temperature value is set both for read operation and
     * notify operation.
     */
    app_ess_temperature[0] = (uint8_t) (temperature & 0xff);
    app_ess_temperature[1] = (uint8_t) ((temperature >> 8) & 0xff);

    /* To check that connection is up and
     * client is registered to receive notifications
     * to send temperature data in Little Endian Format
     * as per BT SIG's ESS Specification
     */
    if (0 != app_bt_conn_id)
    {
        if (0 != (app_ess_temperature_client_char_config[0] &
                 GATT_CLIENT_CONFIG_NOTIFICATION))
        {
            printf("This device is connected to a central device and\n"
                           "GATT client notifications are enabled\n");

            wiced_bt_gatt_send_notification(app_bt_conn_id,
                                            HDLC_ESS_TEMPERATURE_VALUE,
                                            app_ess_temperature_len,
                                            app_ess_temperature);
        }
        else
        {
            printf("This device is connected to a central device and\n"
                           "GATT client notifications are not enabled\n");
        }
    }
    else
    {
        printf("This device is not connected to any BLE central device\n");
    }

}

/* [] END OF FILE */
