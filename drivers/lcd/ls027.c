/*
 * ls027.c
 *
 *  Created on: 29 sept. 2017
 *      Author: Vincent
 */

#include <stdbool.h>
#include "ls027.h"
#include "dma_spi0.h"
#include "segger_wrapper.h"

// release
#define LS027_CE_PIN          kDSPI_MasterPcs2
#define LS027_CE_PIN_INIT     kDSPI_Pcs2

// debug
//#define LS027_CE_PIN          kDSPI_MasterPcs4
//#define LS027_CE_PIN_INIT     kDSPI_Pcs4

#define SPI_BAUDRATE          5000000

#define TRANSFER_SIZE         64U        /*! Transfer dataSize */

static uint8_t LS027_DisplayBuf1[LS027_BUFFER_SIZE]; /* buffer for the display */
static uint8_t LS027_DisplayBuf2[LS027_BUFFER_SIZE]; /* buffer for the display */

static uint8_t LS027_SpiBuf[1 + LS027_BUFFER_SIZE + (240*2) + 1]; /* buffer for the display */

static uint8_t* m_buffer_in_use;
static uint8_t* m_buffer_prev;

/* some aspects of the protocol are pretty timing sensitive... */
#define LS027_BIT_WRITECMD   (0x01)
#define LS027_BIT_VCOM       (0x02)
#define LS027_BIT_CLEAR      (0x04)
#define LS027_TOGGLE_VCOM    do { LS027_sharpmem_vcom = LS027_sharpmem_vcom ? 0x00 : LS027_BIT_VCOM; } while(0);
#define adagfxswap(a, b) { int16_t t = a; a = b; b = t; }

static const uint8_t set[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };
static const uint8_t clr[] = { 0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F };

static const uint8_t conv_set[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
static const uint8_t conv_clr[] = { 0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE };

/*******************************************************************************
 * Variables
 ******************************************************************************/

static uint8_t masterTxData[TRANSFER_SIZE] = {0U};

static uint8_t LS027_sharpmem_vcom;

static spi_transfer_settings spi_settings;

static uint8_t m_is_color_inverted = 0;

static uint16_t m_last_updated_line = 0;
static uint16_t m_nb_updated_line = 0;

/* Internal method prototypes */
static void WriteData(uint8_t data);
static void CE_SetVal ();
static void CE_ClrVal ();
/*
 ** ===================================================================
 **     Method      :  WriteData (component SharpMemDisplay)
 **
 **     Description :
 **         Write a data byte to the display
 ** ===================================================================
 */
static uint8_t LS027_RevertBits(uint8_t data) {
	uint8_t tmp, i;

	/* revert bits */
	tmp = 0;
	for(i=0;;i++) {
		if (data&1) {
			tmp |= 0x1;
		}
		if (i==7) {
			break;
		}
		tmp <<= 1;
		data >>= 1;
	}
	return tmp;
}

static inline void WriteData(uint8_t data)
{
	masterTxData[spi_settings.spi_tx_data_length++] = data;
}


static void ls027_spi_init() {

	// fill settings
	spi_settings.configFlags        = kDSPI_MasterCtar0 | LS027_CE_PIN;
	spi_settings.masterRxData       = 0;
	spi_settings.masterTxData       = masterTxData;
	spi_settings.spi_tx_data_length = 0;

}

/**
 * Starts a SPI transfer
 * @param is_last_byte
 */
static void start_transfer(bool is_last_byte) {

    /* Start master transfer */
	dma_spi0_transfer(&spi_settings);

    spi_settings.spi_tx_data_length = 0;

}

static void CE_ClrVal () {
	// CS pin is toggled by HW SPI automatically
	start_transfer(true);
}
static void CE_SetVal () {
	// CS pin is toggled by HW SPI automatically
	spi_settings.spi_tx_data_length = 0;
}

/**
 *
 * @param i Line number: 0..240
 */
static bool wasLineChanged(uint16_t i) {

	if (memcmp(m_buffer_in_use + (i*LS027_HW_WIDTH/8),
			m_buffer_prev + (i*LS027_HW_WIDTH/8),
			LS027_HW_WIDTH/8)) {
		return true;
	} else {
		return false;
	}

}

/**
 *
 * @param x Col number:  0..400
 * @param y Line number: 0..240
 * @param color Color to be printed
 */
static void setBufferPixel(uint16_t x, uint16_t y, uint16_t color) {

	// clip
	if((x < 0) || (x >= LS027_HW_WIDTH) || (y < 0) || (y >= LS027_HW_HEIGHT)) return;

	// fill buffer
	if (color && !m_is_color_inverted) {
		LS027_SpiBuf[1 + (y*LS027_HW_WIDTH + x) / 8 + 2 * y] |= conv_set[x & 7];
	} else {
		LS027_SpiBuf[1 + (y*LS027_HW_WIDTH + x) / 8 + 2 * y] &= conv_clr[x & 7];
	}

}

/*
 ** ===================================================================
 **     Method      :  LS027_ToggleVCOM (component SharpMemDisplay)
 **     Description :
 **         Command used if EXTMODE=L. This function toggles the VCOM if
 **         no other command is executed
 **     Parameters  : None
 **     Returns     : Nothing
 ** ===================================================================
 */
void LS027_ToggleVCOM(void)
{
	/* send toggle VCOM command */
	CE_SetVal();
	WriteData(LS027_sharpmem_vcom);
	WriteData(0x00); /* 8bit trailer */
	LS027_TOGGLE_VCOM;
	CE_ClrVal();
}

/*
 ** ===================================================================
 **     Method      :  LS027_Clear (component SharpMemDisplay)
 **     Description :
 **         Clears the whole display memory.
 **     Parameters  : None
 **     Returns     : Nothing
 ** ===================================================================
 */
void LS027_Clear(void)
{
	/* send clear command */
	CE_SetVal();
	WriteData(LS027_sharpmem_vcom | LS027_BIT_CLEAR);
	WriteData(0x00);
	LS027_TOGGLE_VCOM;
	CE_ClrVal();
}

/*
 ** ===================================================================
 **     Method      :  LS027_Init (component SharpMemDisplay)
 **     Description :
 **         Display driver initialization
 **     Parameters  : None
 **     Returns     : Nothing
 ** ===================================================================
 */
void LS027_Init(void)
{

	m_is_color_inverted = 0;

	// reset buffers
	memset(LS027_DisplayBuf1, 0, sizeof(LS027_DisplayBuf1));
	memset(LS027_DisplayBuf2, 0, sizeof(LS027_DisplayBuf2));

	// init double buffer
	m_buffer_in_use = LS027_DisplayBuf1;
	m_buffer_prev   = LS027_DisplayBuf2;

	/* Set the vcom bit to a defined state */
	LS027_sharpmem_vcom = LS027_BIT_VCOM;

	ls027_spi_init();
}

/*
 ** ===================================================================
 **     Method      :  LS027_UpdateLine (component SharpMemDisplay)
 **     Description :
 **         Updates a single line on the LCD
 **     Parameters  :
 **         NAME            - DESCRIPTION
 **         line            - Line number to update, range 0-93
 **       * dataP           - Pointer to data, must be array
 **
 **     Returns     : Nothing
 ** ===================================================================
 */
void LS027_UpdateLine(uint8_t line, uint8_t *dataP)
{
	int i;

	/* Send the write command */
	CE_SetVal();
	WriteData(LS027_BIT_WRITECMD | LS027_sharpmem_vcom);
	LS027_TOGGLE_VCOM;

	/* Send the address for line */
	WriteData(line+1);

	/* Send data byte for selected line */
	for (i=0; i<(LS027_HW_WIDTH/8); i++) {
		WriteData(LS027_RevertBits(dataP[i]));
	}
	/* Send trailing 16 bits  */
	WriteData(0x00);
	WriteData(0x00);
	CE_ClrVal();
}

/**************************************************************************/
/*!
    @brief Draws a single pixel in image buffer

    @param[in]  x
                The x position (0 based)
    @param[in]  y
                The y position (0 based)
 */
/**************************************************************************/
void LS027_drawPixel(uint16_t x, uint16_t y, uint16_t color) {
	setBufferPixel(x, y, color);
}


/*
 ** ===================================================================
 **     Method      :  LS027_UpdateFull (component SharpMemDisplay)
 **     Description :
 **         Updates the whole display
 **     Parameters  : None
 **     Returns     : Nothing
 ** ===================================================================
 */
void LS027_UpdateFull(void)
{
	uint16_t addr = 0;

	W_SYSVIEW_OnTaskStartExec(LCD_TASK);

	LS027_SpiBuf[addr++] = (LS027_BIT_WRITECMD | LS027_sharpmem_vcom);

	LS027_TOGGLE_VCOM;

	/* Set the address for lines and dummy data */
	for (int i=0; i < LS027_HW_HEIGHT; i++) {

		// line address
		LS027_SpiBuf[addr++] = i + 1;

		// data bytes
		addr += LS027_HW_WIDTH / 8;

		// dummy data
		LS027_SpiBuf[addr++] = 0x00;

	}

	// dummy data
	LS027_SpiBuf[addr++] = 0x00;

	assert(addr == sizeof(LS027_SpiBuf));

	spi_settings.masterRxData       = 0;
	spi_settings.masterTxData       = LS027_SpiBuf;
	spi_settings.spi_tx_data_length = sizeof(LS027_SpiBuf);

	start_transfer(true);

	W_SYSVIEW_OnTaskStopExec(LCD_TASK);
}


/**
 *
 * @param
 * @return
 */
int LS027_UpdateFullManage(void *user_data)
{
	if (wasLineChanged(m_last_updated_line)) {

		LS027_UpdateLine(m_last_updated_line,
				m_buffer_in_use + (m_last_updated_line*LS027_HW_WIDTH/8));

		m_nb_updated_line++;

	}

	if (++m_last_updated_line < LS027_HW_HEIGHT) {
		return 1;
	}

	return 0;
}
