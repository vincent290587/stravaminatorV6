/* 
 * File:   utils.c
 * Author: vincent
 *
 * Created on October 27, 2015, 10:55 AM
 */


#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "utils.h"


#define FACTOR 100000.

static const float R1 = 6356752.;
static const float R2 = 6378137.;


float min(float val1, float val2) {
  if (val1 <= val2) return val1;
  else return val2;
}

float max(float val1, float val2) {
  if (val1 <= val2) return val2;
  else return val1;
}

double radians(double value) {
	return value * M_PI / 180.;
}

double degrees(double value) {
	return value * 180. / M_PI;
}

double sq(double value) {
	return value * value;
}

float regFen(float val_, float b1_i, float b1_f, float b2_i, float b2_f) {
  
  float x, res;
  // calcul x
  x = (val_ - b1_i) / (b1_f - b1_i);
  
  // calcul valeur: x commun
  res = x * (b2_f - b2_i) + b2_i;
  return res;
}

float regFenLim(float val_, float b1_i, float b1_f, float b2_i, float b2_f) {
  
  float x, res;
  // calcul x
  x = (val_ - b1_i) / (b1_f - b1_i);
  
  // calcul valeur: x commun
  res = x * (b2_f - b2_i) + b2_i;
  if (res < min(b2_i,b2_f)) res = min(b2_i,b2_f);
  if (res > max(b2_i,b2_f)) res = max(b2_i,b2_f);
  return res;
}

float distance_between2(float lat1, float long1, float lat2, float long2) {
  float delta = 3.141592 * (long1 - long2) / 180.;
  float sdlong = sin(delta);
  float cdlong = cos(delta);
  lat1 = 3.141592 * (lat1) / 180.;
  lat2 = 3.141592 * (lat2) / 180.;
  float slat1 = sin(lat1);
  float clat1 = cos(lat1);
  float slat2 = sin(lat2);
  float clat2 = cos(lat2);
  delta = (clat1 * slat2) - (slat1 * clat2 * cdlong);
  delta = delta*delta;
  delta += clat2 * sdlong * clat2 * sdlong;
  delta = sqrt(delta);
  float denom = (slat1 * slat2) + (clat1 * clat2 * cdlong);
  delta = atan2(delta, denom);
  return delta * 6369933;
}

/**
 * Approximation petits angles sur une Terre ellipsoidale
 *
 * @param lat1 En degres
 * @param long1 En degres
 * @param lat2 En degres
 * @param long2 En degres
 * @return
 */
float distance_between(float lat1, float long1, float lat2, float long2) {

  static float Rm = 6356752.;
  static float latRm = 0.;

  float lat1rad = 3.141592 * lat1 / 180.;

  float cos2lat1 = pow(cos(lat1rad), 2.);

  if (fabs(latRm - lat1rad) > 0.008) {
	  latRm = lat1rad;
	  Rm = sqrt(pow(R1,2.)*(1-cos2lat1) + pow(R2,2.)*cos2lat1);
  }

  // petits angles: tan = Id
  float deltalat = 3.141592 * (lat2 -lat1) / 180.;
  float deltalon = 3.141592 * (long2-long1) / 180.;

  float dhori  = deltalon * R2 * cos(lat1rad);
  float dverti = deltalat * Rm;

  // projection plane et pythagore
  return sqrt(dhori*dhori + dverti*dverti);

}

void calculePos (const char *nom, float *lat, float *lon) {

    static char tab[15];
    int iLat;
    int iLon;
    
    if (!nom) {
      return;
    }

    strncpy(tab, nom, 5);
    tab[5] = '\0';
    iLat = toBase10(tab);
    if (lat) {
        *lat = (float) iLat / FACTOR - 90.;
    }

    strncpy(tab, nom + 6, 2);
    strncpy(tab + 2, nom + 9, 3);
    tab[5] = '\0';
    iLon = toBase10(tab);
    if (lon) {
        *lon = (float) iLon / FACTOR - 180.;
    }

    return;
}


long unsigned int toBase10 (char *entree) {

    static char tab[15];

    if (!entree) {
        return 0;
    }
    
    if (!strstr(entree, ".")) {
        strncpy(tab, entree, 5);
        tab[5] = '\0';
    } else {
        strncpy(tab, entree, 2);
        strncpy(tab + 2, entree + 3, 3);
        tab[5] = '\0';
    }

    return strtoul(tab, NULL, 36);

}

uint32_t get_sec_jour(uint8_t hour_, uint8_t min_, uint8_t sec_)
{
  unsigned long res = 0;

  res = 3600 * hour_ + 60 * min_ + sec_;

  return res;
}


float compute2Complement(uint8_t msb, uint8_t lsb) {
	uint16_t t;
	uint16_t val;
	uint8_t tl=lsb, th=msb;
	float ret;

	if (th & 0b00100000) {
		t = th << 8;
		val = (t & 0xFF00) | (tl & 0x00FF);
		val -= 1;
		val = ~(val | 0b1110000000000000);
		//NRF_LOG_INFO("Raw 2c1: %u\r\n", val);
		ret = (float)val;
	} else {
		t = (th & 0xFF) << 8;
		val = (t & 0xFF00) | (tl & 0x00FF);
		//NRF_LOG_INFO("Raw 2c2: %u\r\n", val);
		ret = (float)-val;
	}

	return ret;
}

float percentageBatt(float tensionValue, float current) {

    float fp_ = 0.;

    if (tensionValue > 4.2) {
			fp_ = 100.;
    } else if (tensionValue > 3.78) {
        fp_ = 536.24 * tensionValue * tensionValue * tensionValue;
		fp_ -= 6723.8 * tensionValue * tensionValue;
        fp_ += 28186 * tensionValue - 39402;

		if (fp_ > 100.) fp_ = 100.;

    } else if (tensionValue > 3.2) {
        fp_ = pow(10, -11.4) * pow(tensionValue, 22.315);
    } else {
        fp_ = -1;
    }

    return fp_;
}

