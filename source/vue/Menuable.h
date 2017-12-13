/*
 * Menuable.h
 *
 *  Created on: 13 déc. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_VUE_MENUABLE_H_
#define SOURCE_VUE_MENUABLE_H_

#include <stdint.h>
#include <stdbool.h>
#include "WString.h"
#include "button.h"

#define MENU_MAX_ITEMS_NB     10

typedef void (*f_menu_callback)(int);

typedef enum {
	eMenuableModeAffi,
	eMenuableModeMenu,
	eMenuableModeSubMenu,
} eMenuableMode;

typedef struct {
	String          name;
	f_menu_callback p_func;
} sMenuItem;

typedef struct {
	uint16_t  nb_elem;
	sMenuItem item[MENU_MAX_ITEMS_NB];
} sMenuItemList;



class Menuable {
public:
	Menuable();
	virtual ~Menuable();

	void propagateEvent(e_buttons_event event);

	void tasksMenu(void);

protected:
	bool m_is_menu_selected;
};

#endif /* SOURCE_VUE_MENUABLE_H_ */
