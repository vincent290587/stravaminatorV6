/*
 * dma_i2c0.h
 *
 *  Created on: 27 oct. 2017
 *      Author: Vincent
 */

#ifndef DRIVERS_I2C_INT_I2C0_H_
#define DRIVERS_I2C_INT_I2C0_H_

#include "fsl_i2c.h"

typedef struct {
	uint8_t  address;
	uint32_t reg;
	uint16_t reg_size;
	uint8_t* data;
	uint16_t data_nb;
	i2c_direction_t direction;
	uint32_t flags;
} i2c_transfer_settings;


#ifdef __cplusplus
extern "C" {
#endif


void     i2c0_init(void);

void     i2c0_uninit(void);

status_t i2c0_read_reg(uint8_t address_, uint8_t reg_, uint8_t *val_, uint8_t bytesNumber_);

status_t i2c0_write_reg(uint8_t address_, uint8_t reg_, uint8_t *val_, uint8_t bytesNumber_);

status_t i2c0_read(uint8_t address_, uint8_t *val_, uint8_t bytesNumber_);

status_t i2c0_write(uint8_t address_, uint8_t *val_, uint8_t bytesNumber_);


#ifdef __cplusplus
}
#endif

#endif /* DRIVERS_I2C_INT_I2C0_H_ */
