/*
 * DebugDisplay.h
 *
 *  Created on: 20 oct. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_DISPLAY_DEBUGDISPLAY_H_
#define SOURCE_DISPLAY_DEBUGDISPLAY_H_

#include "SeggerDisplay.h"
#include "Zoom.h"

#define VH_RATIO               0.8

#define BLACK                  1
#define WHITE                  0


class DebugDisplay : public SeggerDisplay {
public:
	DebugDisplay();

	void printSeg(uint8_t ligne, Segment& seg, uint8_t mode);

private:
	Zoom zoom;
};

#endif /* SOURCE_DISPLAY_DEBUGDISPLAY_H_ */
