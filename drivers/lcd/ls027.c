/*
 * ls027.c
 *
 *  Created on: 29 sept. 2017
 *      Author: Vincent
 */

#include "ls027.h"
#include "spi.h"
#include "segger_wrapper.h"

// release
#define LS027_CE_PIN          kDSPI_MasterPcs2
#define LS027_CE_PIN_INIT     kDSPI_Pcs2

// debug
//#define LS027_CE_PIN          kDSPI_MasterPcs4
//#define LS027_CE_PIN_INIT     kDSPI_Pcs4

#define SPI_BAUDRATE          5000000

#define TRANSFER_SIZE         64U        /*! Transfer dataSize */

static uint8_t LS027_DisplayBuf[LS027_BUFFER_SIZE]; /* buffer for the display */

/* some aspects of the protocol are pretty timing sensitive... */
#define LS027_BIT_WRITECMD   (0x01)
#define LS027_BIT_VCOM       (0x02)
#define LS027_BIT_CLEAR      (0x04)
#define LS027_TOGGLE_VCOM    do { LS027_sharpmem_vcom = LS027_sharpmem_vcom ? 0x00 : LS027_BIT_VCOM; } while(0);
#define adagfxswap(a, b) { int16_t t = a; a = b; b = t; }

static const uint8_t set[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };
static const uint8_t clr[] = { 0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F };

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t masterRxData[TRANSFER_SIZE] = {0U};
static uint8_t masterTxData[TRANSFER_SIZE] = {0U};

static uint8_t LS027_sharpmem_vcom;

static spi_transfer_settings spi_settings;

static uint8_t m_orientation = 0;

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

	dspi_master_config_t  masterConfig;

	/*
	 * Data is valid on clock rising edge and shifted on clock falling edge
	 * Clock Idle Polarity: Low
	 */
	masterConfig.whichCtar                                = kDSPI_Ctar0;
	masterConfig.ctarConfig.baudRate                      = SPI_BAUDRATE;
	masterConfig.ctarConfig.bitsPerFrame                  = 8;
	masterConfig.ctarConfig.cpol                          = kDSPI_ClockPolarityActiveHigh;
	masterConfig.ctarConfig.cpha                          = kDSPI_ClockPhaseFirstEdge;
	masterConfig.ctarConfig.direction                     = kDSPI_LsbFirst;
	masterConfig.ctarConfig.pcsToSckDelayInNanoSec        = 1000000000 / SPI_BAUDRATE ;
	masterConfig.ctarConfig.lastSckToPcsDelayInNanoSec    = 1000000000 / SPI_BAUDRATE ;
	masterConfig.ctarConfig.betweenTransferDelayInNanoSec = 1000000000 / SPI_BAUDRATE ;
	masterConfig.whichPcs                                 = LS027_CE_PIN_INIT;
	masterConfig.pcsActiveHighOrLow                       = kDSPI_PcsActiveHigh;
	masterConfig.enableContinuousSCK                      = false;
	masterConfig.enableRxFifoOverWrite                    = false;
	masterConfig.enableModifiedTimingFormat               = false;
	masterConfig.samplePoint                              = kDSPI_SckToSin0Clock;

	spi_init(&masterConfig);

	// fill settings
	spi_settings.configFlags        = kDSPI_MasterCtar0 | LS027_CE_PIN;
	spi_settings.masterRxData       = masterRxData;
	spi_settings.masterTxData       = masterTxData;
	spi_settings.spi_tx_data_length = 0;


}

static void start_transfer(bool is_last_byte) {

    /* Start master transfer */
	spi_start_transfer(&spi_settings, is_last_byte);

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

	m_orientation = 1;

	/* Set the vcom bit to a defined state */
	LS027_sharpmem_vcom = LS027_BIT_VCOM;

	memcpy(LS027_DisplayBuf, SPLASH_BMP, LS027_BUFFER_SIZE);

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

	W_SYSVIEW_OnTaskStartExec(LCD_TASK);

	uint8_t *data = LS027_DisplayBuf;

	for (int i=0; i < LS027_HW_HEIGHT; i++) {

		LS027_UpdateLine(i, data + (i*LS027_HW_WIDTH/8));

	}

	W_SYSVIEW_OnTaskStopExec(LCD_TASK);
}

/*
 ** ===================================================================
 **     Method      :  LS027_SetDisplayOrientation (component SharpMemDisplay)
 **     Description :
 **         Sets current display orientation
 **     Parameters  :
 **         NAME            - DESCRIPTION
 **         newOrientation  - the new orientation
 **                           for the display
 **     Returns     : Nothing
 ** ===================================================================
 */
void LS027_SetDisplayOrientation(LS027_DisplayOrientation newOrientation)
{
	m_orientation = newOrientation; /* setting the display orientation is not implemented yet */
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
void LS027_drawPixel(int16_t x, int16_t y, uint16_t color) {
	// TODO clip
//	if((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) return;

	switch(m_orientation) {
	case 1:
		adagfxswap(x, y);
		x = LS027_HW_WIDTH  - 1 - x;
		break;
	case 2:
		x = LS027_HW_WIDTH  - 1 - x;
		y = LS027_HW_HEIGHT - 1 - y;
		break;
	case 3:
		adagfxswap(x, y);
		y = LS027_HW_HEIGHT - 1 - y;
		break;
	}

	if(color) {
		LS027_DisplayBuf[(y*LS027_HW_WIDTH + x) / 8] |= set[x & 7];
	} else {
		LS027_DisplayBuf[(y*LS027_HW_WIDTH + x) / 8] &= clr[x & 7];
	}
}
