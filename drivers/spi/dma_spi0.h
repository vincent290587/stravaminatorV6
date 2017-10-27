/*
 * dma_spi0.h
 *
 *  Created on: 26 oct. 2017
 *      Author: Vincent
 */

#ifndef DRIVERS_SPI_DMA_SPI0_H_
#define DRIVERS_SPI_DMA_SPI0_H_

#include "spi.h"

#define SPI0_TX_DMA_CHANNEL 4U
#define SPI0_IN_DMA_CHANNEL 5U
#define SPI0_RX_DMA_CHANNEL 6U

typedef void (*pre_hook_func)(void*);
typedef void (*post_hook_func)(void*);
typedef int  (*xfer_func)(void*);

typedef struct {
	xfer_func p_xfer_func;
	pre_hook_func p_pre_func;
	post_hook_func p_post_func;
	void *user_data;
} sXferTask;

typedef enum {
	E_XFER_MNGR_IDLE,
	E_XFER_MNGR_RUN,
} xferTaskState;

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
