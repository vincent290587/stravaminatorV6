/*
 * button.h
 *
 *  Created on: 13 nov. 2017
 *      Author: Vincent
 */

#ifndef __BUTTON_H__
#define __BUTTON_H__

typedef enum {
	k_Buttons_no_event,
	k_Buttons_press_left,
	k_Buttons_press_right,
	k_Buttons_press_center,
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
