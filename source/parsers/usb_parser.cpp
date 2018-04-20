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
#include "RingBuffer.h"

#define BUFFERED_DATA_MAX_NB      512

static char m_st_buffer[BUFFERED_DATA_MAX_NB];
static RingBuffer<char> m_usb_rbuffer(BUFFERED_DATA_MAX_NB, m_st_buffer);

static uint16_t m_dma_buff_index = 0;

static uint32_t m_last_buffered = 0;

static char m_usb_char_buffer[BUFFERED_DATA_MAX_NB];

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

		usb_printf("New simulation LOC received \r\n");

		break;
	default:
		break;

	}

}

/**
 * Prints a char* to VCOM printf style
 * @param format
 */
void usb_printf(const char *format, ...) {

	va_list args;
	va_start(args, format);

	memset(m_usb_char_buffer, 0, sizeof(m_usb_char_buffer));

	int length = vsnprintf(m_usb_char_buffer,
			sizeof(m_usb_char_buffer),
			format, args);

	for (int i=0; i < length; i++) {

		if (!m_usb_rbuffer.isFull()) m_usb_rbuffer.add(m_usb_char_buffer+i);

	}

	//LOG_INFO("Filling USB ringbuffer\r\n");

	m_last_buffered = millis();

}

/**
 * USB VCOM tasks
 */
void usb_tasks(void) {

	if (!USB_DeviceCdcVcomIsBusy()) {

		// go from ringbuffer to dma buffer
		while (!m_usb_rbuffer.isEmpty() &&
				m_dma_buff_index + 1 < DATA_BUFF_SIZE) {

			USB_DeviceCdcVcomFillBuffer(m_usb_rbuffer.get()[0], m_dma_buff_index++);
			m_usb_rbuffer.popLast();

			//LOG_INFO("Filling USB DMA buffer\r\n");

		}

		// send if inactive and data is waiting in buffer or if the buffer is almost full
		if (m_dma_buff_index > DATA_BUFF_SIZE - 5 ||
				(m_dma_buff_index && millis() - m_last_buffered > 50)) {

			//LOG_INFO("Send USB DMA buffer...\r\n");

			// trigger send
			USB_DeviceCdcVcomSend(NULL, m_dma_buff_index);

			// reset index
			m_dma_buff_index = 0;

		}
	} else {
		//LOG_INFO("USB busy\r\n");
	}


}
