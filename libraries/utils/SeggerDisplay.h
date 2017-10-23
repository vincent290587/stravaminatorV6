/*
 * SeggerDisplay.h
 *
 *  Created on: 20 oct. 2017
 *      Author: Vincent
 */

#ifndef LIBRARIES_UTILS_SEGGERDISPLAY_H_
#define LIBRARIES_UTILS_SEGGERDISPLAY_H_

#include "Print.h"
#include "Adafruit_GFX.h"

#define adagfxswap(a, b) { int16_t t = a; a = b; b = t; }

// LCD Dimensions
#define SEG_LCDWIDTH       (80)
#define SEG_LCDHEIGHT      (35)

#define S_BUFFER_SIZE      (SEG_LCDHEIGHT*(((SEG_LCDWIDTH-1)/8)+1))


class SeggerDisplay : public Adafruit_GFX  {
public:
	SeggerDisplay();

	void clear();

	virtual void drawPixel(int16_t x, int16_t y, uint16_t color);

	bool getPixel(int16_t x, int16_t y);

	void displayRTT();
	void displayUSB();

private:
	uint8_t buffer[S_BUFFER_SIZE]; /* buffer for the display */

};

#endif /* LIBRARIES_UTILS_SEGGERDISPLAY_H_ */
