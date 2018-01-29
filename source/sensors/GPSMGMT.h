/*
 * GPS.h
 *
 *  Created on: 13 déc. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_SENSORS_GPSMGMT_H_
#define SOURCE_SENSORS_GPSMGMT_H_

#include "stdint.h"

typedef enum {
	eGPSMgmtPowerOn,
	eGPSMgmtPowerOff,
} eGPSMgmtPowerState;

typedef enum {
	eGPSMgmtTransNMEA,
	eGPSMgmtTransBIN,
} eGPSMgmtTransType;

typedef enum {
	eGPSMgmtEPOIdle,
	eGPSMgmtEPOStart,
	eGPSMgmtEPORunning,
	eGPSMgmtEPOWaitForEvent,
	eGPSMgmtEPOEnd,
} eGPSMgmtEPOState;

#if defined(__cplusplus)
extern "C" {
#endif /* _cplusplus */

uint32_t gps_encode_char(char c);

void gps_uart_start(void);

void gps_uart_stop(void);

void gps_uart_resume(void);

#if defined(__cplusplus)
}

class GPS_MGMT {
public:
	GPS_MGMT();

	void init(void);
	bool is3D(void);

	void standby(void);
	void awake(void);

	void startEpoUpdate(void);
	void tasks(void);

private:
	eGPSMgmtPowerState m_power_state;

	uint16_t m_epo_packet_ind;
	uint16_t m_epo_packet_nb;
};

#endif /* _cplusplus */
#endif /* SOURCE_SENSORS_GPSMGMT_H_ */
