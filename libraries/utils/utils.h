/* 
 * File:   utils.h
 * Author: vincent
 *
 * Created on October 27, 2015, 10:55 AM
 */

#ifndef UTILS_H
#define	UTILS_H

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif


float regFen(float val_, float b1_i, float b1_f, float b2_i, float b2_f);

float regFenLim(float val_, float b1_i, float b1_f, float b2_i, float b2_f);

float distance_between(float lat1, float long1, float lat2, float long2);

float distance_between2(float lat1, float long1, float lat2, float long2);

void calculePos (const char *nom, float *lat, float *lon);

long unsigned int toBase10 (char *entree);

extern void loggerMsg(const char *msg_);

double radians(double value);

double degrees(double value);

double sq(double value);

uint32_t get_sec_jour(uint8_t hour_, uint8_t min_, uint8_t sec_);

#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_H */

