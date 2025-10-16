/**
 * \file
 *
 * \brief Simple BTLC1000 Beacon Application
 *
 * Copyright (c) 2017-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

 /**
 * \mainpage
 * \section preface Preface
 * This is the reference manual for the Simple BTLC1000 Beacon Application
 */
/*- Includes ---------------------------------------------------------------*/

#include <asf.h>
#include "platform.h"
#include "at_ble_api.h"
#include "console_serial.h"
#include "conf_extint.h"
#include "ibeacon.h"
#include "ble_utils.h"

uint8_t event_pool_memory[256] = {0};
uint8_t event_params_memory[1024] = {0};
uint32_t rx_fifo_memory[256] = {0};
#ifndef 	HOST_UART_BAUDRATE_CONFIG_VALUE
#define HOST_UART_BAUDRATE_CONFIG_VALUE 115200 
#endif

/* Initialize the BLE */
static void ble_init(void);

/* BLE connected event indication */
static void ble_device_adv_success_ind(void);

/* BLE disconnected event indication */
static void ble_device_adv_failed_ind(void);

/* Set BLE Address, If address is NULL then it will use BD public address */
static void ble_set_address(at_ble_addr_t *addr);

/* Beacon intialization */
static void beacon_init(void);

at_ble_init_config_t pf_cfg;

/* Set BLE Address, If address is NULL then it will use BD public address */
static void ble_set_address(at_ble_addr_t *addr)
{
	at_ble_addr_t address = {AT_BLE_ADDRESS_PUBLIC, {0xAB, 0xCD, 0xEF, 0xAB, 0xCD, 0xEF}};
	at_ble_addr_t *address_ptr = addr;
	
	if (addr == NULL)
	{
		/* get BD address from BLE device */
		if(at_ble_addr_get(&address) != AT_BLE_SUCCESS)
		{
			DBG_LOG("BD address get failed");
		}
		address_ptr = &address;
	}
	
	/* set the BD address */
	if(at_ble_addr_set(address_ptr) != AT_BLE_SUCCESS)
	{
		DBG_LOG("BD address set failed");
	}
	
	DBG_LOG("BD Address:0x%02X%02X%02X%02X%02X%02X, Address Type:%d",
	address_ptr->addr[5],
	address_ptr->addr[4],
	address_ptr->addr[3],
	address_ptr->addr[2],
	address_ptr->addr[1],
	address_ptr->addr[0], address_ptr->type);
}


/* BLE connected event indication */
static void ble_device_adv_success_ind(void)
{
	/* Switch on the application LED */
	LED_On(LED0);
}

/* BLE disconnected event indication */
static void ble_device_adv_failed_ind(void)
{
	/* Switch off the application LED */
	LED_Off(LED0);
}

void button_cb(void)
{
	/* For user usage */
}


at_ble_init_config_t pf_cfg = {
	/* Register Platform callback API's */
	.platform_api_list.at_ble_create_timer = platform_create_timer,
	.platform_api_list.at_ble_delete_timer = platform_delete_timer,
	.platform_api_list.at_ble_start_timer = platform_start_timer,
	.platform_api_list.at_ble_stop_timer = platform_stop_timer,
	.platform_api_list.at_ble_sleep = platform_sleep,
	.platform_api_list.at_ble_gpio_set = platform_gpio_set,
	.platform_api_list.at_ble_send_sync = platform_send_sync,
	.platform_api_list.at_ble_recv_async = platform_recv_async,
	.platform_api_list.at_ble_reconfigure_usart = platform_configure_hw_fc_uart,
	.platform_api_list.at_ble_wait_for_signal = platform_wait_for_signal,
	.platform_api_list.at_ble_create_signal = platform_create_signal,
	.platform_api_list.at_ble_delete_signal = platform_delete_signal,
	.platform_api_list.at_ble_trigger_signal = platform_trigger_signal,
	.platform_api_list.at_ble_reset_signal = platform_reset_signal
};

/* Initialize the BLE */
static void ble_init(void)
{
	/* Initialize the platform */
	DBG_LOG("Initializing BTLC1000");
	
	/*Memory allocation required by GATT Server DB*/
	pf_cfg.memPool.memSize = 0;
	pf_cfg.memPool.memStartAdd = NULL;
	
	pf_cfg.event_mem_pool.memStartAdd        = event_pool_memory;
	pf_cfg.event_mem_pool.memSize            = sizeof(event_pool_memory);
	pf_cfg.event_params_mem_pool.memStartAdd = event_params_memory;
	pf_cfg.event_params_mem_pool.memSize     = sizeof(event_params_memory);
	pf_cfg.rx_fifo_mem_pool.memStartAdd      = (uint8_t *)rx_fifo_memory; 
	pf_cfg.rx_fifo_mem_pool.memSize          = sizeof(rx_fifo_memory);
	
	/* BTLC1000 XPro Module ZR/MR */
	#if (BLE_MODULE == BTLC1000_ZR)
	pf_cfg.btlc1000_module_version			 = AT_BTLC1000_ZR;

	DBG_LOG("BTLC1000 XPro Module: BTLC1000-ZR");
	#if ((UART_FLOWCONTROL_6WIRE_MODE == false) && (UART_FLOWCONTROL_4WIRE_MODE == true))
	DBG_LOG("BTLC1000 Host Interface UART Mode:4-Wire, Baudrate:%d", (unsigned int)HOST_UART_BAUDRATE_CONFIG_VALUE);
	#else
	DBG_LOG("Error: Invalid BTLC1000 Host Interface UART Mode, BTLC1000-ZR supports only 4-WIRE UART mode");
	return;
	#endif
	#elif (BLE_MODULE == BTLC1000_MR)
	pf_cfg.btlc1000_module_version			 = AT_BTLC1000_MR;

	DBG_LOG("BTLC1000 XPro Module: BTLC1000-MR");
	#if ((UART_FLOWCONTROL_6WIRE_MODE == false) && (UART_FLOWCONTROL_4WIRE_MODE == true))
	DBG_LOG("BTLC1000 Host Interface UART Mode:4-Wire(works only when eFuse enabled), Baudrate:%d", \
	(unsigned int)HOST_UART_BAUDRATE_CONFIG_VALUE);
	#elif ((UART_FLOWCONTROL_6WIRE_MODE == true) && (UART_FLOWCONTROL_4WIRE_MODE == false))
	DBG_LOG("BTLC1000 Host Interface UART Mode:6-Wire(without Efuse Enabled), Baudrate:%d", \
	(unsigned int)HOST_UART_BAUDRATE_CONFIG_VALUE);
	#else
	DBG_LOG("Error: Invalid BTLC1000 Host Interface UART Mode, BTLC1000-MR supports only 4-Wire or 6-Wire UART mode");
	return;
	#endif
	#else
	DBG_LOG("Error: Select a Valid BTLC1000 XPro Module");
	return;
	#endif
	
	/*Bus configuration*/
	pf_cfg.bus_info.bus_type = AT_BLE_UART;
		
#if ((UART_FLOWCONTROL_6WIRE_MODE == true) && (BLE_MODULE == BTLC1000_MR))
	/* Enable Hardware Flow-control on BTLC1000  using patch */
   pf_cfg.bus_info.btlc1000_uart_pinout_switch = true;
#else
	/* Use default Flowcontrol option on BTLC1000 */
   pf_cfg.bus_info.btlc1000_uart_pinout_switch = false;
#endif	

	pf_cfg.bus_info.bus_baudrate = HOST_UART_BAUDRATE_CONFIG_VALUE; // 9600, 19200, 38400, 57600, 115200, 230400, 460800
	pf_cfg.bus_info.host_external_wakeup_gpio = BTLC1000_EXT_PIN9;
	
	pf_cfg.platform_api_list.at_ble_reconfigure_usart = pf_cfg.bus_info.btlc1000_uart_pinout_switch ? platform_configure_hw_fc_uart : platform_configure_primary_uart;
	
	platform_init(pf_cfg.bus_info.bus_type, pf_cfg.bus_info.btlc1000_uart_pinout_switch);
	
	/* Init BLE device */
	if(at_ble_init(&pf_cfg) != AT_BLE_SUCCESS)
	{
		DBG_LOG("BTLC1000 Initialization failed");
		DBG_LOG("Please check the power and connection / hardware connector");
		while(1);
	}
	
	platform_host_set_sleep(HOST_SLEEP_ENABLE);
	if (at_ble_calib_config(0, 2, 2) != AT_BLE_SUCCESS)
	{
		DBG_LOG("Auto-Calibration Disable cmd Failed");
	}
	
}

/* Define service, advertisement data set and start advertisement */
static void beacon_init(void)
{
	uint8_t idx = 0;
	static uint8_t adv_data[BEACON_ADV_LENGTH + BEACON_ADV_TYPE_LENGTH];
	
	/* Adding advertisement data length and advertisement 
	data type to advertisement data */
	adv_data[idx++] = BEACON_ADV_LENGTH;
	adv_data[idx++] = BEACON_ADV_TYPE;
	
	/* Adding manufacturer specific data(company identifier code and 
	additional manufacturer specific data) to advertisement data */
	memcpy(&adv_data[idx], COMPANY_IDENTIFIER_CODE, COMPANY_IDENTIFIER_CODE_LENGTH);
	idx += COMPANY_IDENTIFIER_CODE_LENGTH;
	
	memcpy(&adv_data[idx], ADDTIONAL_MANUFACTURER_SPECIFIC_DATA, ADDTIONAL_MANUFACTURER_SPECIFIC_DATA_LENGTH);
	idx += ADDTIONAL_MANUFACTURER_SPECIFIC_DATA_LENGTH;

	/* set beacon advertisement data */
	if(at_ble_adv_data_set(adv_data, idx, NULL, BEACON_SCAN_RESPONSE_LENGTH) != AT_BLE_SUCCESS)
	{
		DBG_LOG("BLE Beacon advertisement data set failed");
	}
	
	/* BLE start advertisement */
	if(at_ble_adv_start(AT_BLE_ADV_TYPE_NONCONN_UNDIRECTED, AT_BLE_ADV_GEN_DISCOVERABLE, NULL, AT_BLE_ADV_FP_ANY, 
	   BEACON_ADV_INTERVAL, BEACON_ADV_TIMEOUT, BEACON_ABSOLUTE_INTERVAL_ADV) != AT_BLE_SUCCESS)
	{
		DBG_LOG("BLE Beacon advertisement failed");
		ble_device_adv_failed_ind();
	}
	else
	{
		DBG_LOG("Advertisement started");		
		ble_device_adv_success_ind();
	}
}

int main (void)
{
#if SAMG55 || SAM4S
	/* Initialize the SAM system. */
	sysclk_init();
	board_init();
#elif SAM0
	system_init();
#endif
	
	/* Initialize the button */
	button_init();
	
	/* Initialize serial console */
	serial_console_init();
	
	DBG_LOG("Initializing iBeacon Application");
	
	/* initialize the BLE chip */
	ble_init();
	
	/* set its own public address */
	ble_set_address(NULL);
	
	/* Initialize the Beacon advertisement */
	beacon_init();
	
	DBG_LOG("BLE iBeacon Initialization completed");
	
 	while(1){ 	
 	}
}
