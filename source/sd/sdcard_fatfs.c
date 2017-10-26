/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
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
#include "segger_wrapper.h"
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

/*******************************************************************************
 * Variables
 ******************************************************************************/
static FATFS g_fileSystem; /* File system object */
static FIL g_fileObject;   /* File object */

static uint8_t g_bufferWrite[BUFFER_SIZE]; /* Write buffer */
static uint8_t g_bufferRead[BUFFER_SIZE];  /* Read buffer */

const TCHAR driverNumberBuffer[3U] = {SDDISK + '0', ':', '/'};
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
//	CompositeTask();
}

/**
 * @brief Inits the SD card and mounts the FAT system
 */
void usb_comp_init() {

    SYSMPU_Enable(SYSMPU, false);

    if (f_mount(&g_fileSystem, driverNumberBuffer, 0U))
    {
        LOG_INFO("Mount volume failed.\r\n");
    } else {
    	LOG_INFO("Mount volume success.\r\n");
    }

    CompositeInit();

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
