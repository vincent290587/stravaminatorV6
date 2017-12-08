/*
 * dma_i2c0.c
 *
 *  Created on: 27 oct. 2017
 *      Author: Vincent
 */


/*  SDK Included Files */
#include "segger_wrapper.h"
#include "millis.h"
#include "board.h"
#include "fsl_i2c.h"
#include "int_i2c0.h"
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

static i2c_master_config_t   masterConfig;
static i2c_master_transfer_t masterXfer;

uint8_t g_master_buff[I2C_DATA_LENGTH];

i2c_master_handle_t g_m_handle;

volatile bool isI2cTransferCompleted = true;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void i2c_error_handling(void) {

	uint8_t is;
	uint8_t ic;

	is = I2C0->S;
	ic = I2C0->C1;

	if(is & 0x10) { // ARBL
	    I2C0->C1 &= 0xCF; // Switch to RX SLAVE before cleaning ARBL (errata e6749)
	    I2C0->S |= 0x10; // Clear ARBL
	}
	I2C0->S |= 0x02; // Clear IICIF

}


static void i2c_master_callback(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData)
{
	W_SYSVIEW_RecordEnterISR();

	if (status != kStatus_Success)
	{
		LOG_ERROR("DMA I2C0 master callback error %d\r\n", status);
		i2c_error_handling();
	}

	isI2cTransferCompleted = true;

    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif

	W_SYSVIEW_RecordExitISR();

}

/**
 *
 */
void i2c0_init(void)
{
	I2C_MasterGetDefaultConfig(&masterConfig);

	masterConfig.baudRate_Bps = I2C0_BAUDRATE;

	I2C_MasterInit(I2C0, &masterConfig, CLOCK_GetFreq(I2C0_CLK_SRC));

	memset(&g_m_handle, 0, sizeof(g_m_handle));
	I2C_MasterTransferCreateHandle(I2C0, &g_m_handle, i2c_master_callback, NULL);

	isI2cTransferCompleted = true;
}

/**
 *
 */
void i2c0_uninit(void)
{
	isI2cTransferCompleted = true;

	I2C_MasterDeinit(I2C0);

	LOG_INFO("I2C0 uninit\n");
}

/**
 *
 * @param i2c_settings
 */
static status_t _i2c0_transfer(i2c_transfer_settings* i2c_settings) {

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
	if (kStatus_Success != (ret_code = I2C_MasterTransferNonBlocking(I2C0, &g_m_handle, &masterXfer)))
	{
		LOG_ERROR("I2C_MasterTransferEDMA error: %u\r\n ", ret_code);
		i2c_error_handling();
	}
	else {
		isI2cTransferCompleted = false;
	}

	uint32_t time_start = millis();
	while (!isI2cTransferCompleted) {
		// TODO power optimize

		if (millis() - time_start > 200) {
			LOG_ERROR("I2C_MasterTransfer timeout\r\n ");
			isI2cTransferCompleted = true;
		}

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
status_t i2c0_read_reg(uint8_t address_, uint8_t reg_, uint8_t *val_, uint8_t bytesNumber_) {

	i2c_transfer_settings i2c_settings = {
			.address = address_,
			.reg = reg_,
			.reg_size = 1U,
			.data = val_,
			.data_nb = bytesNumber_,
			.direction = kI2C_Read,
			.flags = kI2C_TransferDefaultFlag,
	};

	return _i2c0_transfer(&i2c_settings);
}

/**
 *
 * @param address_
 * @param reg_
 * @param val_
 * @param bytesNumber_
 * @param repeat_start
 */
status_t i2c0_write_reg(uint8_t address_, uint8_t reg_, uint8_t *val_, uint8_t bytesNumber_) {

	i2c_transfer_settings i2c_settings = {
			.address = address_,
			.reg = reg_,
			.reg_size = 1U,
			.data = val_,
			.data_nb = bytesNumber_,
			.direction = kI2C_Write,
			.flags = kI2C_TransferDefaultFlag,
	};

	return _i2c0_transfer(&i2c_settings);
}

/**
 *
 * @param address_
 * @param reg_
 * @param val_
 * @param bytesNumber_
 * @param repeat_start
 */
status_t i2c0_read(uint8_t address_, uint8_t *val_, uint8_t bytesNumber_) {

	i2c_transfer_settings i2c_settings = {
			.address = address_,
			.reg = 0U,
			.reg_size = 0U,
			.data = val_,
			.data_nb = bytesNumber_,
			.direction = kI2C_Read,
			.flags = kI2C_TransferDefaultFlag,
	};

	return _i2c0_transfer(&i2c_settings);
}

/**
 *
 * @param address_
 * @param reg_
 * @param val_
 * @param bytesNumber_
 * @param repeat_start
 */
status_t i2c0_write(uint8_t address_, uint8_t *val_, uint8_t bytesNumber_) {

	i2c_transfer_settings i2c_settings = {
			.address = address_,
			.reg = 0U,
			.reg_size = 0U,
			.data = val_,
			.data_nb = bytesNumber_,
			.direction = kI2C_Write,
			.flags = kI2C_TransferDefaultFlag,
	};

	return _i2c0_transfer(&i2c_settings);
}
