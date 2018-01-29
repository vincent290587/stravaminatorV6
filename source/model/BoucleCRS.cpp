/*
 * Boucle.cpp
 *
 *  Created on: 19 oct. 2017
 *      Author: Vincent
 */


#include "BoucleCRS.h"
#include "Segment.h"
#include "Model.h"
#include "uart0.h"
#include "nrf52.h"
#include "sd_functions.h"
#include "segger_wrapper.h"
#include "spi_scheduler.h"


/**
 *
 */
BoucleCRS::BoucleCRS() : BoucleInterface() {
	m_dist_next_seg = 5000;
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

	// turn GPS ON
	gps_mgmt.awake();
 	gps_mgmt.startEpoUpdate();

	init_liste_segments();

	memset(&att, 0, sizeof(SAtt));

	m_needs_init = false;
}

/**
 *
 */
void BoucleCRS::run() {

	float min_dist_seg = 50000;
	float tmp_dist;

	pwManager.switchToRun120();

	if (m_needs_init) this->init();

	// update position
	att.nbact = 0;
	att.alt = 0;
	locator.getPosition(att.lat, att.lon, att.secj);

//	sdisplay.clear();
//	sdisplay.setCursor(5,5);
//	sdisplay.setTextSize(1);
//	sdisplay.print("No Seg");
//	sdisplay.drawRect(40, 15, 20, 10, BLACK);

//	lcd.resetSegments();

	mes_points.ajouteFinIso(att.lat, att.lon, 0., millis(), 8);

	for (auto& seg : mes_segments._segs) {

		if (seg.isValid() && mes_points.size() > 2) {

			tmp_dist = segment_allocator(seg, att.lat, att.lon);

			// calculate distance to closes segment
			if (tmp_dist < min_dist_seg) min_dist_seg = tmp_dist;

			if (seg.getStatus() != SEG_OFF) {

				W_SYSVIEW_OnTaskStartExec(SEG_PERF_TASK);
				seg.majPerformance(mes_points);
				W_SYSVIEW_OnTaskStopExec(SEG_PERF_TASK);
				att.nbact += 1;

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

//				lcd.registerSegment(seg);

//				sdisplay.clear();
//				sdisplay.printSeg(0, seg, 0);
				break;

			} else if (tmp_dist < 250) {

				W_SYSVIEW_OnTaskStartExec(SEG_PERF_TASK);
				seg.majPerformance(mes_points);
				W_SYSVIEW_OnTaskStopExec(SEG_PERF_TASK);

//				lcd.registerSegment(seg);

				// TODO just display the segment
//				LOG_INFO("Segment preview\r\n");
//				sdisplay.clear();
//				sdisplay.printSeg(0, seg, 0);
				break;
			}

		} // fin isValid

	} // fin for

	att.next = min_dist_seg;

//	lcd.afficheSegments();

	//LOG_INFO("Next segment: %u\r\n", att.next);

	pwManager.switchToRun24();

	W_SYSVIEW_OnTaskStartExec(LCD_TASK);
	vue.refresh();

	dma_spi0_mngr_tasks_start();
	dma_spi0_mngr_finish();
	W_SYSVIEW_OnTaskStopExec(LCD_TASK);

//	sdisplay.displayRTT();

	// go back to low power
	pwManager.switchToVlpr();

	W_SYSVIEW_OnIdle();
}
