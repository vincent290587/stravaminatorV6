/*
 * dma_spi0.h
 *
 *  Created on: 26 oct. 2017
 *      Author: Vincent
 */

#ifndef DRIVERS_SPI_DMA_SPI0_H_
#define DRIVERS_SPI_DMA_SPI0_H_

#include "spi.h"
#include "xfer_manager.h"

#define SPI0_TX_DMA_CHANNEL 0U
#define SPI0_IN_DMA_CHANNEL 1U
#define SPI0_RX_DMA_CHANNEL 2U


#ifdef __cplusplus
extern "C" {
#endif

// DMA SPI driver

void dma_spi0_init(void);

void dma_spi0_uninit(void);

void dma_spi0_transfer(spi_transfer_settings* spi_settings);

// SPI manager

void dma_spi0_mngr_init();

void dma_spi0_mngr_stop();

void dma_spi0_mngr_task_add(sXferTask *task);

void dma_spi0_mngr_tasks_start();

bool dma_spi0_mngr_is_running();

void dma_spi0_mngr_run();

void dma_spi0_mngr_finish();

#ifdef __cplusplus
}
#endif

#endif /* DRIVERS_SPI_DMA_SPI0_H_ */
