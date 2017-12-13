/*
 * Notif.h
 *
 *  Created on: 12 déc. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_VUE_NOTIF_H_
#define SOURCE_VUE_NOTIF_H_

#include <list>
#include <stdint.h>
#include "WString.h"

class Notif {
public:
	Notif(uint8_t type_, const char *title_, const char *msg_) {
		m_type  = type_;
		m_title = title_;
		m_msg   = msg_;
	}
	virtual ~Notif();

	uint8_t m_type;
	String m_title;
	String m_msg;
};


class NotifiableDevice {
public:
	NotifiableDevice() {m_notif_cnt = 0;}
	~NotifiableDevice() {
		m_notifs.clear();
	}

	void addNotif(uint8_t type_, const char *title_, const char *msg_) {
		m_notifs.push_back(Notif(type_, title_, msg_));
		m_notif_cnt++;
	}

protected:
	uint8_t          m_notif_cnt;
	std::list<Notif> m_notifs;
};

#endif /* SOURCE_VUE_NOTIF_H_ */
