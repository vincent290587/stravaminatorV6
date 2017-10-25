/*
 * Boucle.cpp
 *
 *  Created on: 19 oct. 2017
 *      Author: Vincent
 */


#include "BoucleCRS.h"
#include "Segment.h"
#include "Model.h"
#include "sd_functions.h"
#include "segger_wrapper.h"

/**
 *
 */
BoucleCRS::BoucleCRS() : Boucle() {
}

/**
 *
 * @return true if we are ready to run
 */
bool BoucleCRS::isTime() {

	if (locator.isUpdated()) {
		return true;
	}

	return false;
}

/**
 *
 */
void BoucleCRS::init() {

	init_liste_segments();

}

/**
 *
 */
void BoucleCRS::run() {

	float min_dist_seg = 50000;
	float tmp_dist;

	pwManager.switchToRun();

	W_SYSVIEW_OnTaskStartExec(SEG_PERF_TASK);

	// update position
	att.alt = 0;
	locator.getPosition(att.lat, att.lon);
	locator.clearIsUpdated();

	sdisplay.clear();
	sdisplay.setCursor(5,5);
	sdisplay.setTextSize(1);
	sdisplay.print("No Seg");
//	sdisplay.drawRect(40, 15, 20, 10, BLACK);

	mes_points.ajouteFinIso(att.lat, att.lon, 0., millis(), 8);

	for (auto& seg : mes_segments._segs) {

		if (seg.isValid() && mes_points.size() > 2) {

			W_SYSVIEW_OnTaskStopExec(SEG_PERF_TASK);

			tmp_dist = segment_allocator(seg, att.lat, att.lon);

			W_SYSVIEW_OnTaskStartExec(SEG_PERF_TASK);

			// calculate distance to closes segment
			if (tmp_dist < min_dist_seg) min_dist_seg = tmp_dist;

			if (seg.getStatus() != SEG_OFF) {

				seg.majPerformance(mes_points);

				//				att.nbact += 1;
				if (seg.getStatus() == SEG_START) {

					LOG_INFO("Segment START %s\r\n", seg.getName());

				} else if (seg.getStatus() == SEG_FIN) {

					LOG_INFO("Segment FIN %s\r\n", seg.getName());


					if (seg.getAvance() > 0.) {
						//						att.nbpr++;
						//						att.nbkom++;
						//						victoryTone ();
					}

				} else if (seg.getStatus() == SEG_ON) {
					//					Serial3.println(Nordic::encodeOrder(seg.getAvance(), seg.getCur()));
					//					order_glasses = 1;
					LOG_INFO("Avance: %d\r\n", (int)(seg.getAvance() / 1000));

				}

				//				display.registerSegment(seg);

				sdisplay.clear();
				sdisplay.printSeg(0, seg, 0);
				break;

			}
			else if (tmp_dist < 250) {

				seg.majPerformance(mes_points);

				// TODO just display the segment
				LOG_INFO("Segment preview\r\n");
				sdisplay.clear();
				sdisplay.printSeg(0, seg, 0);
				break;
			}

		} // fin isValid

	} // fin for

	W_SYSVIEW_OnTaskStopExec(SEG_PERF_TASK);

	LOG_INFO("Next segment: %d\r\n", (int)min_dist_seg);

	// update the screen
	lcd.setCursor(10,10);
	lcd.setTextSize(3);
	lcd.print(min_dist_seg);
	lcd.writeWhole();

//	sdisplay.displayRTT();

	// go back to low power
	pwManager.switchToVlpr();

}
