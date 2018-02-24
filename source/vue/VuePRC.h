/*
 * VuePRC.h
 *
 *  Created on: 12 d�c. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_VUE_VUEPRC_H_
#define SOURCE_VUE_VUEPRC_H_

#include <Adafruit_GFX.h>
#include <display/Zoom.h>
#include <display/SegmentManager.h>
#include <routes/Parcours.h>
#include <sensors/button.h>
#include <vue/VueCRS.h>

typedef enum {
	eVuePRCScreenInit,
	eVuePRCScreenGps,
	eVuePRCScreenDataFull,
} eVuePRCScreenModes;

class VuePRC: virtual public Adafruit_GFX, protected Zoom, virtual public SegmentManager {
public:
	VuePRC();

	eVuePRCScreenModes tasksPRC();

	bool propagateEventsPRC(e_buttons_event event);

	virtual void cadranH(uint8_t p_lig, uint8_t nb_lig, const char *champ, String  affi, const char *p_unite)=0;
	virtual void cadran(uint8_t p_lig, uint8_t nb_lig, uint8_t p_col, const char *champ, String  affi, const char *p_unite)=0;

protected:
	eVuePRCScreenModes m_prc_screen_mode;
	float m_distance_prc;

private:
	void afficheSegment(uint8_t ligne, Segment *p_seg);
	void afficheParcours(uint8_t ligne, ListePoints *p_liste);

	void parcoursSelector(void);

	Parcours *m_s_parcours;
	uint8_t   m_parcours_sel;
	bool      m_selec_en;
	bool      m_start_loading;
};

#endif /* SOURCE_VUE_VUEPRC_H_ */
