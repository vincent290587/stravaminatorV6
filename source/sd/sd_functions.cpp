/*
 * sd_functions.cpp
 *
 *  Created on: 18 oct. 2017
 *      Author: Vincent
 */

#include <stdio.h>
#include <string.h>
#include "ff.h"
#include "diskio.h"
#include "segger_wrapper.h"
#include "fsl_sysmpu.h"
#include "fsl_debug_console.h"
#include "composite_public.h"
#include "sdcard_fatfs.h"
#include "board.h"
#include "Model.h"
#include "millis.h"

extern const TCHAR driverNumberBuffer[];

/*!
 * @brief Main function
 */
int init_liste_segments(void)
{
	FRESULT error;
	DIR directory; /* Directory object */
	FILINFO fileInformation;
	uint16_t nb_files_errors = 0;

	mes_segments._segs.clear();

#if (_FS_RPATH >= 2U)
	error = f_chdrive((char const *)&driverNumberBuffer[0U]);
	if (error)
	{
		LOG_INFO("Change drive failed.\r\n");
		return -1;
	}
#endif

	LOG_INFO("\r\nList the file in the root directory......\r\n");
	if (f_opendir(&directory, "/"))
	{
		LOG_INFO("Open directory failed.\r\n");
		return -1;
	}

	for (;;)
	{
		error = f_readdir(&directory, &fileInformation);

		/* To the end. */
		if ((error != FR_OK) || (fileInformation.fname[0U] == 0U))
		{
			break;
		}
		if (fileInformation.fname[0] == '.')
		{
			continue;
		}
		if (fileInformation.fattrib & AM_DIR)
		{
			LOG_INFO("Directory file : %s\r\n", fileInformation.fname);
		}
		else
		{
			fileInformation.fname[12] = 0;

			LOG_INFO("General file : %s\r\n", fileInformation.fname);

			if (Segment::nomCorrect(fileInformation.fname)) {
				LOG_INFO("Segment added\r\n");
				mes_segments.push_back(Segment(fileInformation.fname));
			} else if (Parcours::nomCorrect(fileInformation.fname)) {
				// pas de chargement en double
				LOG_INFO("Parcours added\r\n");
				mes_parcours.push_back(Parcours(fileInformation.fname));
			} else {
				LOG_INFO("File refused\r\n");
				nb_files_errors++;
			}

		}
	}

	LOG_INFO("%u files refused\r\n", nb_files_errors);

	return 0;
}
