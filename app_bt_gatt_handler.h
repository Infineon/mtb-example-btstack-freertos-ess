/*******************************************************************************
* File Name: app_bt_gatt_handler.h
*
* Description: This file consists of the function prototypes that are
*              necessary for developing the Bluetooth LE applications with GATT
*              Server callbacks.
*
* Related Document: See README.md
*
*
*********************************************************************************
 Copyright 2020-2021, Cypress Semiconductor Corporation (an Infineon company) or
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

#ifndef __APP_BT_GATT_HANDLER_H__
#define __APP_BT_GATT_HANDLER_H__

/* *****************************************************************************
 *                              INCLUDES
 * ****************************************************************************/
#include "wiced_bt_gatt.h"
#include "cybsp_types.h"
#include "GeneratedSource/cycfg_pins.h"
#include "GeneratedSource/cycfg_gap.h"

/* *****************************************************************************
 *                              CONSTANTS
 * ****************************************************************************/
/* LED pin assignment for connection */
#define CONNECTION_LED                   CYBSP_USER_LED1
/* The error code for invalid  attribute index in attribute table */
#define INVALID_ATT_TBL_INDEX            (0xFFFFFFFF)

/* *****************************************************************************
 *                              VARIABLES
 * ****************************************************************************/

/* A Global variable to check the status of this device if it is
 * connected to any peer devices*/
extern uint16_t app_bt_conn_id;

/* *****************************************************************************
 *                              FUNCTION DECLARATIONS
 * ****************************************************************************/
wiced_bt_gatt_status_t
app_gatt_attr_write_handler(wiced_bt_gatt_opcode_t opcode,
                            wiced_bt_gatt_write_req_t *p_write_req,
                            uint16_t len_req);

wiced_bt_gatt_status_t
app_gatt_attr_read_handler( uint16_t conn_id,
                            wiced_bt_gatt_opcode_t opcode,
                            wiced_bt_gatt_read_t *p_read_req,
                            uint16_t len_req);
wiced_bt_gatt_status_t
app_gatt_connect_handler(wiced_bt_gatt_connection_status_t *p_conn_status);

wiced_bt_gatt_status_t
app_gatts_attr_req_handler(wiced_bt_gatt_attribute_request_t *p_attr_req);

wiced_bt_gatt_status_t
app_bt_gatt_event_callback(wiced_bt_gatt_evt_t event,
                           wiced_bt_gatt_event_data_t *p_event_data);

wiced_bt_gatt_status_t app_set_gatt_attr_value(uint16_t attr_handle,
                                               uint8_t *p_val,
                                               uint16_t len);

int32_t app_get_attr_index_by_handle(uint16_t attr_handle);


#endif      /* __APP_BT_GATT_HANDLER_H__ */

/* [] END OF FILE */
