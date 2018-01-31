/*
 * VueCRS.h
 *
 *  Created on: 12 déc. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_VUE_VUECRS_H_
#define SOURCE_VUE_VUECRS_H_

#include "parameters.h"
#include "Segment.h"
#include <Adafruit_GFX.h>


typedef enum {
	eVueCRSScreenInit,
	eVueCRSScreenDataFull,
	eVueCRSScreenDataSS,
	eVueCRSScreenDataDS,
} eVueCRSScreenModes;

typedef struct {
	uint8_t  is_prio;
	Segment* p_seg;
} sVueCRSPSeg;

typedef struct {
	uint8_t     nb_segs;
	sVueCRSPSeg s_segs[NB_SEG_ON_DISPLAY];
} sVueCRSSegArray;


class VueCRS: virtual public Adafruit_GFX {
public:
	VueCRS();

	eVueCRSScreenModes tasksCRS();

	void addSegment(Segment*);
	void addSegmentPrio(Segment*);

	virtual void cadranH(uint8_t p_lig, uint8_t nb_lig, const char *champ, String  affi, const char *p_unite)=0;
	virtual void cadran(uint8_t p_lig, uint8_t nb_lig, uint8_t p_col, const char *champ, String  affi, const char *p_unite)=0;

protected:
	eVueCRSScreenModes m_crs_screen_mode;

	sVueCRSSegArray m_segs;

private:
	void afficheListePoints(uint8_t ligne, Segment *p_seg);
	void partner(uint8_t ligne, Segment *p_seg);

	void displayGPS(void);
};

#endif /* SOURCE_VUE_VUECRS_H_ */
