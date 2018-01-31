/*
 * UAParser.h
 *
 *  Created on: 13 nov. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_PARSERS_UAPARSER_H_
#define SOURCE_PARSERS_UAPARSER_H_

void ua_send_notification_green (uint8_t on_time_);

void ua_send_notification_blue (uint8_t on_time_);

void ua_send_notification_red (uint8_t on_time_);

class UAParser {
public:
	UAParser();

	void init(void);
	void tasks(void);

private:



};

#endif /* SOURCE_PARSERS_UAPARSER_H_ */
