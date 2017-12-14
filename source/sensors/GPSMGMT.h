/*
 * GPS.h
 *
 *  Created on: 13 déc. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_SENSORS_GPSMGMT_H_
#define SOURCE_SENSORS_GPSMGMT_H_


class GPS_MGMT {
public:
	GPS_MGMT();

	void init(void);
	bool is3D(void);

	void standby(void);
	void awake(void);
};

#endif /* SOURCE_SENSORS_GPSMGMT_H_ */
