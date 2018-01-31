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
#include "sd_functions.h"
#include "fsl_debug_console.h"
#include "fsl_common.h"
#include "composite_public.h"
#include "sdcard_fatfs.h"
#include "board.h"
#include "Model.h"
#include "millis.h"
#include "file_parser.h"
#include "WString.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* buffer size (in byte) for read/write operations */
#define BUFFER_SIZE (100U)

/*******************************************************************************
 * Variables
 ******************************************************************************/

//static TCHAR g_bufferWrite[BUFFER_SIZE]; /* Write buffer */
static TCHAR g_bufferRead[BUFFER_SIZE];  /* Read buffer */

static FIL g_fileObject;   /* File object */
static FIL g_EpoFileObject;   /* File object */

extern const TCHAR driverNumberBuffer[];


/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int init_liste_segments(void)
{
	FRESULT error;
	DIR directory; /* Directory object */
	FILINFO fileInformation;
	uint16_t nb_files_errors = 0;

	W_SYSVIEW_OnTaskStartExec(SD_ACCESS_TASK);

	mes_segments._segs.clear();

#if (_FS_RPATH >= 2U)
	error = f_chdrive((char const *)&driverNumberBuffer[0U]);
	if (error)
	{
		LOG_INFO("Change drive failed.\r\n");
		W_SYSVIEW_OnTaskStopExec(SD_ACCESS_TASK);
		return -1;
	}
#endif

	LOG_INFO("\r\nList the file in the root directory......\r\n");
	if (f_opendir(&directory, "/"))
	{
		LOG_INFO("Open directory failed.\r\n");
		W_SYSVIEW_OnTaskStopExec(SD_ACCESS_TASK);
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
//			LOG_INFO("Directory file : %s\r\n", fileInformation.fname);
		}
		else
		{
			fileInformation.fname[12] = 0;

//			LOG_INFO("General file : %s\r\n", fileInformation.fname);

			if (Segment::nomCorrect(fileInformation.fname)) {
//				LOG_INFO("Segment added\r\n");
				mes_segments.push_back(Segment(fileInformation.fname));
			} else if (Parcours::nomCorrect(fileInformation.fname)) {
				// pas de chargement en double
//				LOG_INFO("Parcours added\r\n");
				mes_parcours.push_back(Parcours(fileInformation.fname));
			} else {
				LOG_INFO("File refused\r\n");
				nb_files_errors++;
			}

		}
	}

	LOG_INFO("%u files refused\r\n", nb_files_errors);
	LOG_INFO("%u segments addded\r\n", mes_segments.size());

	W_SYSVIEW_OnTaskStopExec(SD_ACCESS_TASK);

	return 0;
}

/**
 * Loads a segment from the SD card
 * @param seg The reference of this segment
 * @return -1 if failure, else the number of points loaded
 */
int load_segment(Segment& seg) {

	int res = 0;
	FRESULT error;
	float time_start = 0.;

	time_start = 0.;

	W_SYSVIEW_OnTaskStartExec(SD_ACCESS_TASK);

	String fat_name = String("/") + seg.getName();

	error = f_open(&g_fileObject, _T(fat_name.c_str()), FA_READ);
	if (error)
	{
		LOG_INFO("Open file failed.\r\n");
		W_SYSVIEW_OnTaskStopExec(SD_ACCESS_TASK);
		return -1;
	}

	memset(g_bufferRead, 0U, sizeof(g_bufferRead));

	while (f_gets(g_bufferRead, sizeof(g_bufferRead)-1, &g_fileObject)) {

		if (strstr(g_bufferRead, "<")) {
			// meta data
		} else if (strstr(g_bufferRead, ";")) {
			// on est pret a charger le point
			if (!chargerPointSeg(g_bufferRead, seg, time_start))
				res++;
		}

	}

	error = f_close (&g_fileObject);
	if (error)
	{
		LOG_INFO("Close file failed.\r\n");
		W_SYSVIEW_OnTaskStopExec(SD_ACCESS_TASK);
		return -1;
	}

	W_SYSVIEW_OnTaskStopExec(SD_ACCESS_TASK);

	LOG_INFO("%d points loaded\r\n", res);

	return res;
}

/**
 *
 * @param mon_parcours
 * @return
 */
int load_parcours(Parcours& mon_parcours) {

	int res = 0;
	FRESULT error;

	String fat_name = String("/") + mon_parcours.getName();

	W_SYSVIEW_OnTaskStartExec(SD_ACCESS_TASK);

	error = f_open(&g_fileObject, _T(fat_name.c_str()), FA_READ);
	if (error)
	{
		LOG_INFO("Open file failed.\r\n");
		return -1;
	}

	memset(g_bufferRead, 0U, sizeof(g_bufferRead));

	while (f_gets(g_bufferRead, sizeof(g_bufferRead)-1, &g_fileObject)) {

		// on se met au bon endroit
		if (strstr(g_bufferRead, "<")) {
			// meta data
		} else if (strstr(g_bufferRead, ";")) {
			// on est pret a charger le point
			if (!chargerPointPar(g_bufferRead, mon_parcours))
				res++;
		}

	} // fin du fichier

	error = f_close (&g_fileObject);
	if (error)
	{
		LOG_INFO("Close file failed.\r\n");
		return -1;
	}

	return res;
}

/**
 * desalloue automatiquement les segments lointains
 * alloue automatiquement les segments proches
 *
 * @param mon_seg
 * @param lat1
 * @param long1
 * @return
 */
float segment_allocator(Segment& mon_seg, float lat1, float long1) {

	static float tmp_dist = 0.;
	static float tmp_lat = 0.;
	static float tmp_lon = 0.;
	float ret_val = 5000;

	// le segment est rempli
	if (mon_seg.isValid() && mon_seg.getStatus() == SEG_OFF) {

		if (mon_seg.longueur() >= 1) {
			// on teste l'eloignement
			Point pp = *mon_seg.getFirstPoint();

			tmp_dist = distance_between(lat1, long1, pp._lat, pp._lon);
			ret_val = tmp_dist;

			if (tmp_dist > DIST_ALLOC) {
				// on desalloue
				LOG_INFO("Unallocate %s\r\n", mon_seg.getName());

				mon_seg.desallouerPoints();
				mon_seg.setStatus(SEG_OFF);
			}
		}
		else {

			if (parseSegmentName(mon_seg.getName(), &tmp_lat, &tmp_lon) == 1) {
//				Serial.println(F("Echec parsing du nom"));
//				loggerMsg("Echec parsing du nom");
//				loggerMsg(mon_seg->getName());
				return ret_val;
			}

			// on etudie si on doit charger
			tmp_dist = distance_between(lat1, long1, tmp_lat, tmp_lon);
			ret_val = tmp_dist;

			if (tmp_dist < DIST_ALLOC) {

				if (mon_seg.longueur() > 0) {
					LOG_INFO("Saving points %s\r\n", mon_seg.getName());

					mon_seg.desallouerPoints();
				}


				int res = load_segment(mon_seg);
				LOG_INFO("-->> Loading segment %s\r\n", mon_seg.getName(), res);

				if (res > 0) mon_seg.init();
			}
		}

	} else if (mon_seg.longueur() > 0 && mon_seg.isValid()) {

		// on teste l'eloignement au premier point
		Point pp = *mon_seg.getFirstPoint();
		tmp_dist = distance_between(lat1, long1, pp._lat, pp._lon);
		ret_val = tmp_dist;

		// distance au segment
		Point tmp_pt(lat1, long1, 0., 0.);
		tmp_dist = mon_seg.dist(&tmp_pt);

		// test desallocation
		if (tmp_dist > MARGE_DESACT * DIST_ALLOC) {
			// on desalloue
			LOG_INFO("Desallocation non nominale !\r\n");

			mon_seg.desallouerPoints();
			mon_seg.setStatus(SEG_OFF);

//			display.notifyANCS(1, "WTCH", "Seg trop loin");
		}


	}

	return ret_val;
}

/**
 *
 * @return The size of the EPO file
 */
int epo_file_size(void) {

	FRESULT error;
	const char* fname = "/MTK14.EPO";

	FILINFO file_info;
	error = f_stat (fname, &file_info);

	if (error) {
		LOG_INFO("Stat file failed.\r\n");
		return -1;
	}

	return file_info.fsize;
}

/**
 *
 * @return
 */
int epo_file_start(void) {

	FRESULT error;
	const char* fname = "/MTK14.EPO";

	error = f_open(&g_EpoFileObject, fname, FA_READ);
	if (error)
	{
		LOG_INFO("Open file failed.\r\n");
		return  -1;
	}

	return 0;
}

/**
 *
 * @param epo_data
 * @return The number of sat_data read, or -1 if error
 */
int epo_file_read(sEpoPacketSatData* sat_data) {

	memset(g_bufferRead, 0U, sizeof(g_bufferRead));

	assert(sat_data);

	UINT size_read = 0;
	FRESULT error = f_read (
			&g_EpoFileObject, 	/* Pointer to the file object */
			g_bufferRead,	    /* Pointer to data buffer */
			MTK_EPO_SAT_DATA_SIZE,/* Number of bytes to read */
			&size_read	        /* Pointer to number of bytes read */
	);

	if (error)
	{
		LOG_INFO("Read EPO file failed.\r\n");
		return -1;
	}

	if (size_read != MTK_EPO_SAT_DATA_SIZE) {
		LOG_INFO("End of EPO file\r\n");
		return 1;
	} else {
		memcpy(sat_data->sat, g_bufferRead, MTK_EPO_SAT_DATA_SIZE);
	}

	return 0;
}

/**
 *
 * @return
 */
int epo_file_stop(void) {

	FRESULT error = f_close (&g_EpoFileObject);
	if (error)
	{
		LOG_INFO("Close file failed.\r\n");
		return -1;
	}

#ifndef DEBUG_CONFIG
	error = f_unlink("/MTK14.EPO");
	if (error)
	{
		LOG_INFO("Unlink file failed.\r\n");
		return -2;
	}
#endif

	return 0;
}
