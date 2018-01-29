/*
 * GPS.cpp
 *
 *  Created on: 13 déc. 2017
 *      Author: Vincent
 */

#include <GPSMGMT.h>
#include "MTK.h"
#include "uart0.h"
#include "utils.h"
#include "millis.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_uart.h"
#include "uart0.h"
#include "uart2.h"
#include "sd_functions.h"
#include "LocusCommands.h"
#include "segger_wrapper.h"
#include "locator.h"
#include "parameters.h"

#define GPS_DEFAULT_SPEED_BAUD     9600U
#define GPS_FAST_SPEED_BAUD        115200U

#if (GPS_FAST_SPEED_BAUD > GPS_DEFAULT_SPEED_BAUD)
#define GPS_BIN_CMD                PMTK_SET_BIN_BAUD_115200
#define GPS_NMEA_CMD               PMTK_SET_NMEA_BAUD_115200
#else
#define GPS_BIN_CMD                PMTK_SET_BIN_BAUD_9600
#define GPS_NMEA_CMD               PMTK_SET_NMEA_BAUD_9600
#endif

#define GPS_UART_SEND(X,Y) do { \
		uart0_send(X, Y); } while(0)

#define SEND_TO_GPS(X) do { \
		const_char_to_buffer(X, buffer, sizeof(buffer)); \
		GPS_UART_SEND(buffer, strlen(X)); } while(0)


static uint8_t buffer[256];

static eGPSMgmtTransType  m_trans_type = eGPSMgmtTransNMEA;
static eGPSMgmtEPOState   m_epo_state  = eGPSMgmtEPOIdle;

static MTK m_rec_packet;

static uart_config_t uartConfig;

void gps_uart_start() {

	// Re-init the UART.
	UART_GetDefaultConfig(&uartConfig);

	uartConfig.enableTx = true;
	uartConfig.enableRx = true;
	uartConfig.baudRate_Bps = GPS_DEFAULT_SPEED_BAUD;

	uart0_init(&uartConfig);

}

void gps_uart_stop() {

	uart0_uninit();

}

void gps_uart_resume() {

	uart0_init(&uartConfig);

}

GPS_MGMT::GPS_MGMT() {

	m_power_state = eGPSMgmtPowerOn;
	m_trans_type  = eGPSMgmtTransNMEA;
	m_epo_state   = eGPSMgmtEPOIdle;

}

void GPS_MGMT::init(void) {

	// configure PPS pin
	gpio_pin_config_t pin_config;
	pin_config.outputLogic  = 0;
	pin_config.pinDirection = kGPIO_DigitalInput;
	GPIO_PinInit(BOARD_INITPINS_PPS_GPIO, BOARD_INITPINS_PPS_GPIO_PIN, &pin_config);

	// configure 3D pin
	pin_config.pinDirection = kGPIO_DigitalInput;
	GPIO_PinInit(BOARD_INITPINS_PIN_3D_GPIO, BOARD_INITPINS_PIN_3D_GPIO_PIN, &pin_config);

#if GPS_USE_COLD_START
	SEND_TO_GPS(PMTK_COLD);
#endif

	// go to default baudrate
	uartConfig.baudRate_Bps = GPS_DEFAULT_SPEED_BAUD;

	if (GPS_FAST_SPEED_BAUD > GPS_DEFAULT_SPEED_BAUD) {

		gps_uart_stop();
		delay_us(500);
		gps_uart_resume();

		// change GPS baudrate
		SEND_TO_GPS(PMTK_SET_NMEA_BAUD_115200);

		// go to final baudrate
		delay_us(500);
		uartConfig.baudRate_Bps = GPS_FAST_SPEED_BAUD;
		gps_uart_stop();
		delay_us(500);
		gps_uart_resume();

	}

}

bool GPS_MGMT::is3D(void) {
	return !GPIO_PinRead(BOARD_INITPINS_PIN_3D_GPIO, BOARD_INITPINS_PIN_3D_GPIO_PIN);
}

void GPS_MGMT::standby(void) {

	if (eGPSMgmtPowerOff == m_power_state) return;

	LOG_INFO("GPS put in standby\r\n");

	m_power_state = eGPSMgmtPowerOff;

	SEND_TO_GPS(PMTK_STANDBY);
}

void GPS_MGMT::awake(void) {

	if (eGPSMgmtPowerOn == m_power_state) return;

	LOG_INFO("GPS awoken\r\n");

	m_power_state = eGPSMgmtPowerOn;

	SEND_TO_GPS(PMTK_AWAKE);

}

void GPS_MGMT::startEpoUpdate(void) {

	LOG_INFO("EPO update started\r\n");

	m_epo_state = eGPSMgmtEPOStart;

	this->awake();
}

void GPS_MGMT::tasks(void) {

	switch (m_epo_state) {
	case eGPSMgmtEPOIdle:
		break;

	case eGPSMgmtEPOStart:
	{
		m_epo_packet_nb = 0;
		m_epo_packet_ind = 0;

		int size = epo_file_size();
		int res  = epo_file_start();

		if (size <= 0 || res < 0) {
			LOG_ERROR("EPO start failure, size=%ld\r\n", size);

			m_epo_state = eGPSMgmtEPOIdle;

		} else {
			LOG_INFO("EPO size: %ld bytes\r\n", size);

			m_epo_state = eGPSMgmtEPORunning;

			// set transport to binary
			SEND_TO_GPS(GPS_BIN_CMD);

			delay_ms(5);

			m_trans_type = eGPSMgmtTransBIN;
		}
	}
	break;

	case eGPSMgmtEPORunning:
	{
		sEpoPacketBinData epo_data;
		memset(&epo_data, 0, sizeof(epo_data));

		// fill the packet
		for (int i=0; i < MTK_EPO_MAX_SAT_DATA; i++) {
			// read file
			int ret_code = epo_file_read(&epo_data.sat_data[epo_data.nb_sat]);

			if (ret_code < 0) {
				// error
			} else if (ret_code == 1) {
				// end
			} else {
				epo_data.nb_sat  += 1;
			}
		}

		if (epo_data.nb_sat > 0) {
			epo_data.epo_seq = m_epo_packet_ind++;

			LOG_INFO("EPO sending packet #%u - %u sats\r\n",
					epo_data.epo_seq, epo_data.nb_sat);

			LOG_DEBUG("Sat data 1: %X %X\r\n",
					epo_data.sat_data[0].sat[0],
					epo_data.sat_data[0].sat[1]);

			MTK tmp_mtk(&epo_data);
			tmp_mtk.toBuffer(buffer, sizeof(buffer));

			GPS_UART_SEND(buffer, tmp_mtk.getPacketLength());

			m_epo_state = eGPSMgmtEPOWaitForEvent;
		} else {
			// process is finished

			epo_data.epo_seq = 0xFFFF;

			LOG_INFO("EPO last packet\r\n");

			MTK tmp_mtk(&epo_data);
			tmp_mtk.toBuffer(buffer, sizeof(buffer));

			GPS_UART_SEND(buffer, tmp_mtk.getPacketLength());

			m_epo_state = eGPSMgmtEPOEnd;
		}
	}
	break;

	case eGPSMgmtEPOWaitForEvent:
	{
	}
	break;

	case eGPSMgmtEPOEnd:
	{
		LOG_INFO("EPO update end\r\n");

		(void)epo_file_stop();

		// NMEA + baud rate default
		uint8_t cmd[5] = {0x00,0x00,0x00,0x00,0x00};
		encode_uint32 (cmd + 1, GPS_FAST_SPEED_BAUD);

		// set transport to NMEA
		MTK tmp_mtk(MTK_FMT_NMEA_CMD_ID, cmd, sizeof(cmd));
		tmp_mtk.toBuffer(buffer, sizeof(buffer));

		LOG_INFO("MTK switched to NMEA\r\n");

		GPS_UART_SEND(buffer, tmp_mtk.getPacketLength());

		m_epo_state = eGPSMgmtEPOIdle;

	}
	break;

	default:
		break;
	}
}

/**
 *
 * @param c Input character
 * @return Error code
 */
uint32_t gps_encode_char(char c) {

	//LOG_INFO("%c", c);
	//LOG_FLUSH();

	if (eGPSMgmtTransNMEA == m_trans_type) {

		locator_encode_char(c);

	} else {

		if (m_rec_packet.encode(c)) {

			// the packet is valid
			switch (m_rec_packet.getCommandId()) {
			case MTK_EPO_BIN_ACK_CMD_ID:
				if (eGPSMgmtEPOWaitForEvent == m_epo_state) {

					if (m_rec_packet.m_packet.raw_packet.data[2] == 0x01) {
						LOG_INFO("Packet ack recv\r\n");

						// resume sending packets
						m_epo_state = eGPSMgmtEPORunning;
					} else {
						LOG_ERROR("Packet ack recv with error %X\r\n",
								m_rec_packet.m_packet.raw_packet.data[2]);

						// end it
						m_epo_state = eGPSMgmtEPOEnd;
					}
				}
				break;

			case MTK_ACK_CMD_ID:
			{
				uint16_t cmd_id = decode_uint16(m_rec_packet.m_packet.raw_packet.data);
				uint8_t status  = m_rec_packet.m_packet.raw_packet.data[2];
				LOG_INFO("Binary ACK to cmd 0x%X with status=%u\r\n", cmd_id, status);

				if (MTK_FMT_NMEA_CMD_ID == cmd_id &&
						status == 3) {
					m_trans_type = eGPSMgmtTransNMEA;
				}
			}
			break;

			case MTK_EMPTY_CMD_ID:
				break;

			default:
				break;
			}

			// reset packet
			m_rec_packet.clear();
		}

	}

	return 0;
}
