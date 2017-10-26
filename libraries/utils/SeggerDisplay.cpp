/*
 * SeggerDisplay.cpp
 *
 *  Created on: 20 oct. 2017
 *      Author: Vincent
 */

#include "segger_wrapper.h"
#include "virtual_com.h"
#include <SeggerDisplay.h>
#include "millis.h"
#include <string.h>

static const uint8_t set[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };
static const uint8_t clr[] = { 0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F };


/**
 *
 */
SeggerDisplay::SeggerDisplay() : Adafruit_GFX(SEG_LCDWIDTH, SEG_LCDHEIGHT) {

	memset(buffer, 0, S_BUFFER_SIZE);

	this->setRotation(0);
}

/**
 *
 */
void SeggerDisplay::clear() {

	memset(buffer, 0, S_BUFFER_SIZE);

}

/**
 *
 * @param x
 * @param y
 * @param color
 */
void SeggerDisplay::drawPixel(int16_t x, int16_t y, uint16_t color) {

	if((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) return;

	switch(rotation) {
	case 1:
		adagfxswap(x, y);
		x = WIDTH  - 1 - x;
		break;
	case 2:
		x = WIDTH  - 1 - x;
		y = HEIGHT - 1 - y;
		break;
	case 3:
		adagfxswap(x, y);
		y = HEIGHT - 1 - y;
		break;
	}

	if(color) {
		buffer[(y*SEG_LCDWIDTH + x) / 8] |= set[x & 7];
	} else {
		buffer[(y*SEG_LCDWIDTH + x) / 8] &= clr[x & 7];
	}

}

bool SeggerDisplay::getPixel(int16_t x, int16_t y) {

	if((x >= _width) || (y >= _height))
		return false; // <0 test not needed, unsigned

//	switch(rotation) {
//	case 1:
//		adagfxswap(x, y);
//		x = WIDTH  - 1 - x;
//		break;
//	case 2:
//		x = WIDTH  - 1 - x;
//		y = HEIGHT - 1 - y;
//		break;
//	case 3:
//		adagfxswap(x, y);
//		y = HEIGHT - 1 - y;
//		break;
//	}

	if (buffer[(y*SEG_LCDWIDTH + x) / 8] & set[x & 7]) {
		return true;
	}

	return false;
}

/**
 *
 */
void SeggerDisplay::displayUSB() {

	uint8_t usb_buffer[SEG_LCDWIDTH/8 + 10];
	uint16_t pos;

	for (int i=0; i < SEG_LCDHEIGHT; i++) {

		pos = 0;
		usb_buffer[pos++] = '@';
		usb_buffer[pos++] = i;

		memcpy(usb_buffer + pos, buffer + (i*(SEG_LCDWIDTH/8)), SEG_LCDWIDTH/8);
		pos += SEG_LCDWIDTH/8;

		usb_buffer[pos++] = '\n';

		while (kStatus_USB_Success != USB_DeviceCdcVcomSend(usb_buffer, pos)) {
			sleep();
		}

	}
}

/**
 *
 */
void SeggerDisplay::displayRTT() {

	LOG_SET_TERM(RTT_GRAPH_CHANNEL);

	LOG_FLUSH();
	LOG_INFO("\r\n");

	for (int i=-1; i <= SEG_LCDHEIGHT; i++) {

		for (int j=0; j < SEG_LCDWIDTH; j++) {

			if (0 <= i && i < SEG_LCDHEIGHT) {
				if (this->getPixel(j, i)) {
					LOG_INFO("*");
				} else {
					LOG_INFO(" ");
				}
			} else {
				LOG_INFO("-");
			}

		}

		LOG_INFO("\r\n");
		LOG_FLUSH();

	}

	LOG_SET_TERM(RTT_LOG_CHANNEL);
}



