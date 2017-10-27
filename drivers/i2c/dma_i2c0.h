/*
 * dma_i2c0.h
 *
 *  Created on: 27 oct. 2017
 *      Author: Vincent
 */

#ifndef DRIVERS_I2C_DMA_I2C0_H_
#define DRIVERS_I2C_DMA_I2C0_H_

#include "fsl_i2c.h"

#define I2C0_DMA_CHANNEL 7U

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


void     dma_i2c0_init(void);

void     dma_i2c0_uninit(void);

status_t dma_i2c0_read_reg(uint8_t address_, uint8_t reg_, uint8_t *val_, uint8_t bytesNumber_, bool repeat_start);

status_t dma_i2c0_write_reg(uint8_t address_, uint8_t reg_, uint8_t *val_, uint8_t bytesNumber_, bool repeat_start);

status_t dma_i2c0_read(uint8_t address_, uint8_t *val_, uint8_t bytesNumber_, bool repeat_start);

status_t dma_i2c0_write(uint8_t address_, uint8_t *val_, uint8_t bytesNumber_, bool repeat_start);


#ifdef __cplusplus
}
#endif

#endif /* DRIVERS_I2C_DMA_I2C0_H_ */
