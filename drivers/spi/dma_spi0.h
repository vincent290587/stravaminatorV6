/*
 * dma_spi0.h
 *
 *  Created on: 26 oct. 2017
 *      Author: Vincent
 */

#ifndef DRIVERS_SPI_DMA_SPI0_H_
#define DRIVERS_SPI_DMA_SPI0_H_

#include <stdint.h>
#include "fsl_dspi.h"

typedef struct {
	uint8_t* masterTxData;
	uint8_t* masterRxData;
	uint16_t spi_tx_data_length;
	uint32_t configFlags;
} spi_transfer_settings;

#define SPI0_TX_DMA_CHANNEL 0U
#define SPI0_IN_DMA_CHANNEL 1U
#define SPI0_RX_DMA_CHANNEL 2U


#ifdef __cplusplus
extern "C" {
#endif

// DMA SPI driver

void dma_spi0_init(void);

void dma_spi0_update_clocks(void);

void dma_spi0_uninit(void);

void dma_spi0_transfer(spi_transfer_settings* spi_settings);

void dma_spi0_finish_transfer(void);

#ifdef __cplusplus
}
#endif

#endif /* DRIVERS_SPI_DMA_SPI0_H_ */
