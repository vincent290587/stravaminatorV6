/*
 * usb_parser.cpp
 *
 *  Created on: 18 oct. 2017
 *      Author: Vincent
 */


#include "Model.h"
#include "virtual_com.h"
#include "usb_parser.h"
#include "segger_wrapper.h"


static char m_usb_char_buffer[128];

/**
 * Decodes chars un the VCOM line
 * @param c
 */
void usb_parser_decode(char c) {

	switch (nordic.encode(c)) {
	case _SENTENCE_LOC:

		locator.sim_loc.data.lat = (float)nordic.getLat() / 10000000.;
		locator.sim_loc.data.lon = (float)nordic.getLon() / 10000000.;
		locator.sim_loc.data.alt = (float)nordic.getEle();
		locator.sim_loc.data.utc_time = nordic.getSecJ();

		locator.sim_loc.setIsUpdated();

		usb_send_chars("New simulation LOC received \r\n");

		break;
	default:
		break;

	}


}

/**
 * Prints a char* to VCOM
 * @param buffer
 */
void usb_send_chars(const char *buffer) {

	USB_DeviceCdcVcomSend((uint8_t*)buffer, strlen(buffer));

}

/**
 * Prints a char* to VCOM printf style
 * @param format
 */
void usb_printf(const char *format, ...) {

	va_list args;
	va_start(args, format);

	memset(m_usb_char_buffer, 0, sizeof(m_usb_char_buffer));

	vsnprintf(m_usb_char_buffer,
			sizeof(m_usb_char_buffer),
			format, args);

	USB_DeviceCdcVcomSend((uint8_t*)m_usb_char_buffer, strlen(m_usb_char_buffer));

}
