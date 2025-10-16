/**
 * \file
 *
 * \brief iBeacon Application Declarations
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
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Atmel
 *Support</a>
 */
#ifndef __IBEACON_APP_H__
#define __IBEACON_APP_H__


/****************************************************************************************
*							        Macros	                                     		*
****************************************************************************************/

/* Beacon identifier value */
#define BEACON_IDENTIFIER (0x46)

/* Beacon advertisement Interval */
#define BEACON_ADV_INTERVAL (1600) //in 0.625ms

/* Beacon Advertisement timeout */
#define BEACON_ADV_TIMEOUT	(0) //in 0.625ms

/* Beacon Advertise at exact interval */
#define BEACON_ABSOLUTE_INTERVAL_ADV (true)

/* Beacon Advertisement data */
#define ADV_TYPE_MANUFACTURER_SPECIFIC_DATA			(0xff)
#define COMPANY_IDENTIFIER_CODE						("\x4c\x00")
#define COMPANY_IDENTIFIER_CODE_LENGTH				(0x02)
#define ADDTIONAL_MANUFACTURER_SPECIFIC_DATA		("\x02\x15\x21\x8a\xf6\x52\x73\xe3\x40\xb3\xb4\x1c\x19\x53\x24\x2c\x72\xf4\x00\xbb\x00\x45\xc5")
#define ADDTIONAL_MANUFACTURER_SPECIFIC_DATA_LENGTH	(0x17)
#define BEACON_ADV_LENGTH							(0x1a)
#define BEACON_ADV_TYPE								ADV_TYPE_MANUFACTURER_SPECIFIC_DATA
#define BEACON_ADV_TYPE_LENGTH						(0x01)
#define BEACON_SCAN_RESPONSE_LENGTH					(0x00)

/* Debug Log printer */
#define DBG_LOG printf("\r\n");\
				printf

#endif /* __IBEACON_APP_H__ */
