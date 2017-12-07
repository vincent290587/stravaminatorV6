/*
 * spi_scheduler.h
 *
 *  Created on: 8 déc. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_SCHEDULING_SPI_SCHEDULER_H_
#define SOURCE_SCHEDULING_SPI_SCHEDULER_H_

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

#endif /* SOURCE_SCHEDULING_SPI_SCHEDULER_H_ */
