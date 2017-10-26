/*
 * sd_functions.h
 *
 *  Created on: 18 oct. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_SD_SD_FUNCTIONS_H_
#define SOURCE_SD_SD_FUNCTIONS_H_

//#if defined(__cplusplus)
//extern "C" {
//#endif /* _cplusplus */

int init_liste_segments(void);

int load_segment(Segment& seg);

float segment_allocator(Segment& mon_seg, float lat1, float long1);

//#if defined(__cplusplus)
//}
//#endif /* _cplusplus */

#endif /* SOURCE_SD_SD_FUNCTIONS_H_ */
