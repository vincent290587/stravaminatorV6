/*
 * dma_i2c0.c
 *
 *  Created on: 27 oct. 2017
 *      Author: Vincent
 */


/*  SDK Included Files */
#include "segger_wrapper.h"
#include "board.h"
#include "fsl_i2c_edma.h"
#include "dma_i2c0.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define I2C_MASTER_CLK_FREQ CLOCK_GetFreq(I2C0_CLK_SRC)

#define I2C_DATA_LENGTH (32) /* MAX is 256 */

#define I2C0_BAUDRATE (400000) /* 100K */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

static i2c_master_config_t masterConfig;
static i2c_master_transfer_t masterXfer;

uint8_t g_master_buff[I2C_DATA_LENGTH];

i2c_master_edma_handle_t g_m_dma_handle;
edma_handle_t edmaHandle;

volatile bool isTransferCompleted = true;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void _i2c0_callback(I2C_Type *base, i2c_master_edma_handle_t *handle,
		status_t status, void *userData)
{
	SEGGER_SYSVIEW_RecordEnterISR();
	if (status != kStatus_Success)
	{
		LOG_ERROR("DMA I2C0 master callback error\r\n");
	}
	isTransferCompleted = true;
//	LOG_INFO("Xfer completed %d\r\n\r\n", status);
	SEGGER_SYSVIEW_RecordExitISR();
}

/**
 *
 */
void dma_i2c0_init(void)
{
	I2C_MasterGetDefaultConfig(&masterConfig);

	masterConfig.baudRate_Bps = I2C0_BAUDRATE;

	I2C_MasterInit(I2C0, &masterConfig, CLOCK_GetFreq(I2C0_CLK_SRC));

	memset(&g_m_dma_handle, 0, sizeof(g_m_dma_handle));

	DMAMUX_SetSource(DMAMUX0, I2C0_DMA_CHANNEL, kDmaRequestMux0I2C0);
	DMAMUX_EnableChannel(DMAMUX0, I2C0_DMA_CHANNEL);

	EDMA_CreateHandle(&edmaHandle, DMA0, I2C0_DMA_CHANNEL);

	I2C_MasterCreateEDMAHandle(I2C0, &g_m_dma_handle, _i2c0_callback, NULL, &edmaHandle);

	isTransferCompleted = true;
}

/**
 *
 */
void dma_i2c0_uninit(void)
{
	isTransferCompleted = true;

	DMAMUX_DisableChannel(DMAMUX0, I2C0_DMA_CHANNEL);

	I2C_MasterDeinit(I2C0);

	LOG_INFO("I2C0 uninit\n");
}

/**
 *
 * @param i2c_settings
 */
static status_t dma_i2c0_transfer(i2c_transfer_settings* i2c_settings) {

	W_SYSVIEW_OnTaskStartExec(I2C_TASK);

	memset(&masterXfer, 0, sizeof(masterXfer));

	masterXfer.slaveAddress = i2c_settings->address;
	masterXfer.direction = i2c_settings->direction;
	masterXfer.subaddress = i2c_settings->reg;
	masterXfer.subaddressSize = i2c_settings->reg_size;
	masterXfer.data = i2c_settings->data;
	masterXfer.dataSize = i2c_settings->data_nb;
	masterXfer.flags = i2c_settings->flags;

	status_t ret_code;
	if (kStatus_Success != (ret_code = I2C_MasterTransferEDMA(I2C0, &g_m_dma_handle, &masterXfer)))
	{
		LOG_ERROR("I2C_MasterTransferEDMA error: %u\r\n ", ret_code);
	} else {
		isTransferCompleted = false;
	}

	while (!isTransferCompleted) {
		// TODO power optimize
	}

	W_SYSVIEW_OnTaskStopExec(I2C_TASK);

	return ret_code;
}

/**
 *
 * @param address_
 * @param reg_
 * @param val_
 * @param bytesNumber_
 * @param repeat_start
 */
status_t dma_i2c0_read_reg(uint8_t address_, uint8_t reg_, uint8_t *val_, uint8_t bytesNumber_, bool repeat_start) {

	i2c_transfer_settings i2c_settings = {
			.address = address_,
			.reg = reg_,
			.reg_size = 1U,
			.data = val_,
			.data_nb = bytesNumber_,
			.direction = kI2C_Read,
			.flags = repeat_start ? kI2C_TransferRepeatedStartFlag : kI2C_TransferDefaultFlag,
	};

	return dma_i2c0_transfer(&i2c_settings);
}

/**
 *
 * @param address_
 * @param reg_
 * @param val_
 * @param bytesNumber_
 * @param repeat_start
 */
status_t dma_i2c0_write_reg(uint8_t address_, uint8_t reg_, uint8_t *val_, uint8_t bytesNumber_, bool repeat_start) {

	i2c_transfer_settings i2c_settings = {
			.address = address_,
			.reg = reg_,
			.reg_size = 1U,
			.data = val_,
			.data_nb = bytesNumber_,
			.direction = kI2C_Write,
			.flags = repeat_start ? kI2C_TransferRepeatedStartFlag : kI2C_TransferDefaultFlag,
	};

	return dma_i2c0_transfer(&i2c_settings);
}

/**
 *
 * @param address_
 * @param reg_
 * @param val_
 * @param bytesNumber_
 * @param repeat_start
 */
status_t dma_i2c0_read(uint8_t address_, uint8_t *val_, uint8_t bytesNumber_, bool repeat_start) {

	i2c_transfer_settings i2c_settings = {
			.address = address_,
			.reg = 0U,
			.reg_size = 0U,
			.data = val_,
			.data_nb = bytesNumber_,
			.direction = kI2C_Read,
			.flags = repeat_start ? kI2C_TransferRepeatedStartFlag : kI2C_TransferDefaultFlag,
	};

	return dma_i2c0_transfer(&i2c_settings);
}

/**
 *
 * @param address_
 * @param reg_
 * @param val_
 * @param bytesNumber_
 * @param repeat_start
 */
status_t dma_i2c0_write(uint8_t address_, uint8_t *val_, uint8_t bytesNumber_, bool repeat_start) {

	i2c_transfer_settings i2c_settings = {
			.address = address_,
			.reg = 0U,
			.reg_size = 0U,
			.data = val_,
			.data_nb = bytesNumber_,
			.direction = kI2C_Write,
			.flags = repeat_start ? kI2C_TransferRepeatedStartFlag : kI2C_TransferDefaultFlag,
	};

	return dma_i2c0_transfer(&i2c_settings);
}
