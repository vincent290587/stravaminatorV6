/*
 * button.h
 *
 *  Created on: 13 nov. 2017
 *      Author: Vincent
 */

#ifndef __BUTTON_H__
#define __BUTTON_H__

typedef enum {
	k_Buttons_no_event     = 0U,
	k_Buttons_interrupt    = 1U,
	k_Buttons_press_left   = 2U,
	k_Buttons_press_center = 3U,
	k_Buttons_press_right  = 4U,
} e_buttons_event;


#if defined(__cplusplus)
extern "C" {
#endif /* _cplusplus */


void buttons_init(void);

e_buttons_event buttons_tasks(void);


#if defined(__cplusplus)
}
#endif /* _cplusplus */

#endif /* __BUTTON_H__ */
