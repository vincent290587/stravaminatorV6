/*
 * The Clear BSD License
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <string.h>
#include "ff.h"
#include "diskio.h"
#include "fsl_sd_disk.h"
#include "segger_wrapper.h"
#include "fsl_sdmmc_common.h"
#include "fsl_common.h"
#include "fsl_sysmpu.h"
#include "fsl_debug_console.h"
#include "composite_public.h"
#include "sdcard_fatfs.h"
#include "board.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* buffer size (in byte) for read/write operations */
#define BUFFER_SIZE (100U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
* @brief wait card insert function.
*/
static status_t sdcardWaitCardInsert(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static FATFS g_fileSystem; /* File system object */
static FIL g_fileObject;   /* File object */

const TCHAR driverNumberBuffer[3U] = {SDDISK + '0', ':', '/'};

/* @brief decription about the read/write buffer
* The size of the read/write buffer should be a multiple of 512, since SDHC/SDXC card uses 512-byte fixed
* block length and this driver example is enabled with a SDHC/SDXC card.If you are using a SDSC card, you
* can define the block length by yourself if the card supports partial access.
* The address of the read/write buffer should align to the specific DMA data buffer address align value if
* DMA transfer is used, otherwise the buffer address is not important.
* At the same time buffer address/size should be aligned to the cache line size if cache is supported.
*/
SDK_ALIGN(uint8_t g_bufferWrite[SDK_SIZEALIGN(BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CACHE)],
          MAX(SDMMC_DATA_BUFFER_ALIGN_CACHE, SDMMCHOST_DMA_BUFFER_ADDR_ALIGN));
SDK_ALIGN(uint8_t g_bufferRead[SDK_SIZEALIGN(BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CACHE)],
          MAX(SDMMC_DATA_BUFFER_ALIGN_CACHE, SDMMCHOST_DMA_BUFFER_ADDR_ALIGN));
/*! @brief SDMMC host detect card configuration */
static const sdmmchost_detect_card_t s_sdCardDetect = {
#ifndef BOARD_SD_DETECT_TYPE
    .cdType = kSDMMCHOST_DetectCardByGpioCD,
#else
    .cdType = BOARD_SD_DETECT_TYPE,
#endif
    .cdTimeOut_ms = (~0U),
};
/*******************************************************************************
 * Code
 ******************************************************************************/

/**
 * MSC Init
 */

/**
 * MSC tasks
 */
void sdcard_tasks(void) {
	CompositeTask();
}

/**
 * @brief Inits the SD card and mounts the FAT system
 */
void usb_comp_init() {

    SYSMPU_Enable(SYSMPU, false);

    CompositeInit();

    if (f_mount(&g_fileSystem, driverNumberBuffer, 0U))
    {
        LOG_INFO("Mount volume failed.\r\n");
    } else {
    	LOG_INFO("Mount volume success.\r\n");
    }

}



/*!
 * @brief Main function
 */
int essai(void)
{
    FRESULT error;
    DIR directory; /* Directory object */
    FILINFO fileInformation;
    UINT bytesWritten;
    UINT bytesRead;
    volatile bool failedFlag = false;
    char ch = '0';
    //BYTE work[_MAX_SS];

#if (_FS_RPATH >= 2U)
    error = f_chdrive((char const *)&driverNumberBuffer[0U]);
    if (error)
    {
        LOG_INFO("Change drive failed.\r\n");
        return -1;
    }
#endif

#if _USE_MKFS
    LOG_INFO("\r\nMake file system......The time may be long if the card capacity is big.\r\n");
    if (f_mkfs(driverNumberBuffer, FM_ANY, 0U, work, sizeof work))
    {
        LOG_INFO("Make file system failed.\r\n");
        return -1;
    }
#endif /* _USE_MKFS */

    LOG_INFO("\r\nCreate directory......\r\n");
    error = f_mkdir(_T("/dir_1"));
    if (error)
    {
        if (error == FR_EXIST)
        {
            LOG_INFO("Directory exists.\r\n");
        }
        else
        {
            LOG_INFO("Make directory failed.\r\n");
            return -1;
        }
    }

    LOG_INFO("\r\nCreate a file in that directory......\r\n");
    error = f_open(&g_fileObject, _T("/dir_1/f_1.dat"), (FA_WRITE | FA_READ | FA_CREATE_ALWAYS));
    if (error)
    {
        if (error == FR_EXIST)
        {
            LOG_INFO("File exists.\r\n");
        }
        else
        {
            LOG_INFO("Open file failed.\r\n");
            return -1;
        }
    }

    LOG_INFO("\r\nCreate a directory in that directory......\r\n");
    error = f_mkdir(_T("/dir_1/dir_2"));
    if (error)
    {
        if (error == FR_EXIST)
        {
            LOG_INFO("Directory exists.\r\n");
        }
        else
        {
            LOG_INFO("Directory creation failed.\r\n");
            return -1;
        }
    }

    LOG_INFO("\r\nList the file in that directory......\r\n");
    if (f_opendir(&directory, "/dir_1"))
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
            LOG_INFO("Directory file : %s.\r\n", fileInformation.fname);
        }
        else
        {
            LOG_INFO("General file : %s.\r\n", fileInformation.fname);
        }
    }

    memset(g_bufferWrite, 'a', sizeof(g_bufferWrite));
    g_bufferWrite[BUFFER_SIZE - 2U] = '\r';
    g_bufferWrite[BUFFER_SIZE - 1U] = '\n';

    LOG_INFO("\r\nWrite/read file until encounters error......\r\n");
    while (true)
    {
        if (failedFlag || (ch == 'q'))
        {
            break;
        }

        LOG_INFO("\r\nWrite to above created file.\r\n");
        error = f_write(&g_fileObject, g_bufferWrite, sizeof(g_bufferWrite), &bytesWritten);
        if ((error) || (bytesWritten != sizeof(g_bufferWrite)))
        {
            LOG_INFO("Write file failed. \r\n");
            failedFlag = true;
            continue;
        }

        /* Move the file pointer */
        if (f_lseek(&g_fileObject, 0U))
        {
            LOG_INFO("Set file pointer position failed. \r\n");
            failedFlag = true;
            continue;
        }

        LOG_INFO("Read from above created file.\r\n");
        memset(g_bufferRead, 0U, sizeof(g_bufferRead));
        error = f_read(&g_fileObject, g_bufferRead, sizeof(g_bufferRead), &bytesRead);
        if ((error) || (bytesRead != sizeof(g_bufferRead)))
        {
            LOG_INFO("Read file failed. \r\n");
            failedFlag = true;
            continue;
        }

        LOG_INFO("Compare the read/write content......\r\n");
        if (memcmp(g_bufferWrite, g_bufferRead, sizeof(g_bufferWrite)))
        {
            LOG_INFO("Compare read/write content isn't consistent.\r\n");
            failedFlag = true;
            continue;
        }
        LOG_INFO("The read/write content is consistent.\r\n");

        LOG_INFO("\r\nInput 'q' to quit read/write.\r\nInput other char to read/write file again.\r\n");
        break;
    }
    LOG_INFO("\r\nThe example will not read/write file again.\r\n");

    if (f_close(&g_fileObject))
    {
        LOG_INFO("\r\nClose file failed.\r\n");
        return -1;
    }

    if (f_mount(NULL, driverNumberBuffer, 0U))
    {
    	LOG_INFO("Unmount volume failed.\r\n");
    	return -1;
    }

    return 0;
}


static status_t sdcardWaitCardInsert(void)
{
    /* Save host information. */
    g_sd.host.base = SD_HOST_BASEADDR;
    g_sd.host.sourceClock_Hz = SD_HOST_CLK_FREQ;
    /* card detect type */
    g_sd.usrParam.cd = &s_sdCardDetect;
    /* SD host init function */
    if (SD_HostInit(&g_sd) != kStatus_Success)
    {
        PRINTF("\r\nSD host init fail\r\n");
        return kStatus_Fail;
    }
    /* power off card */
    SD_PowerOffCard(g_sd.host.base, g_sd.usrParam.pwr);
    /* wait card insert */
    if (SD_WaitCardDetectStatus(SD_HOST_BASEADDR, &s_sdCardDetect, true) == kStatus_Success)
    {
        PRINTF("\r\nCard inserted.\r\n");
        /* power on the card */
        SD_PowerOnCard(g_sd.host.base, g_sd.usrParam.pwr);
    }
    else
    {
        PRINTF("\r\nCard detect fail.\r\n");
        return kStatus_Fail;
    }

    return kStatus_Success;
}
