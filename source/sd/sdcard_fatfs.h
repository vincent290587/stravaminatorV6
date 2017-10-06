/*
 * sdcard_fatfs.h
 *
 *  Created on: 5 oct. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_SD_SDCARD_FATFS_H_
#define SOURCE_SD_SDCARD_FATFS_H_

#if defined(__cplusplus)
extern "C" {
#endif /* _cplusplus */

void sdcard_init(void);

void sdcard_tasks(void);

#if defined(__cplusplus)
}
#endif /* _cplusplus */

#endif /* SOURCE_SD_SDCARD_FATFS_H_ */
