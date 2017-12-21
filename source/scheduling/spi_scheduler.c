/*
 * spi_scheduler.c
 *
 *  Created on: 8 déc. 2017
 *      Author: Vincent
 */

#include <stdint.h>
#include <stdbool.h>
#include "millis.h"
#include "segger_wrapper.h"
#include "spi_scheduler.h"
#include "power_manager.h"

static sXferTask m_tasks[SPI_SCHEDULER_MAX_NB_TASKS];
static uint8_t m_tasks_nb;
static uint8_t m_cur_task;
static xferTaskState m_state;

extern bool isSpiTransferCompleted;

static   bool isXferStarted = false;
volatile bool isXferDone    = true;

static uint32_t last_task_start = 0;

void dma_spi0_mngr_callback() {
	isXferDone = true;
}

void dma_spi0_mngr_init() {

	m_state = E_XFER_MNGR_IDLE;
	m_cur_task = 0;
	m_tasks_nb = 0;

	isXferStarted = false;
	isXferDone    = true;

}

void dma_spi0_mngr_stop() {

	if (m_state == E_XFER_MNGR_RUN) {
		// post hook
		if (m_tasks[m_cur_task].p_post_func)
			(*m_tasks[m_cur_task].p_post_func)(m_tasks[m_cur_task].user_data);
	}

	m_state = E_XFER_MNGR_IDLE;
	m_cur_task = 0;

	isXferStarted = false;
	isXferDone    = true;
}

void dma_spi0_mngr_task_add(sXferTask *task) {

	if (m_tasks_nb < SPI_SCHEDULER_MAX_NB_TASKS) {
		memcpy(m_tasks+m_tasks_nb, task, sizeof(sXferTask));
		m_tasks_nb++;

		LOG_INFO("Xfer task added\r\n");
	} else {
		LOG_INFO("Xfer task buffer full\r\n");
	}

}

void dma_spi0_mngr_tasks_start() {

	if (m_state == E_XFER_MNGR_RUN) {
//		dma_spi0_mngr_stop();
		return;
	}

	m_cur_task = 0;
	isXferStarted = false;
	isXferDone    = true;

	if (m_tasks_nb) {
		m_state = E_XFER_MNGR_RUN;

		// start pre-hook of first task
		if (m_tasks[m_cur_task].p_pre_func)
			(*m_tasks[m_cur_task].p_pre_func)(m_tasks[m_cur_task].user_data);
	}

	// init transfers
	dma_spi0_mngr_run();
}

bool dma_spi0_mngr_is_running() {

	if (m_state == E_XFER_MNGR_RUN) {
		return true;
	}
	return false;
}

void dma_spi0_mngr_run() {

	// make sure we fill the DMA buffers every time with a while
	if (m_state == E_XFER_MNGR_RUN && isXferDone && !isXferStarted) {

		isXferDone = false;

		assert(m_tasks[m_cur_task].p_xfer_func);
		int res = (*m_tasks[m_cur_task].p_xfer_func)(m_tasks[m_cur_task].user_data);

		last_task_start = millis();

		LOG_INFO("Xfer task started %u\r\n", res);

		isXferStarted = true;

	} else if (m_state == E_XFER_MNGR_RUN && isXferDone && isXferStarted) {

		isXferStarted = false;

		LOG_INFO("Xfer task finished in %u ms\r\n", millis() - last_task_start);

		// the task is finished
		if (m_tasks[m_cur_task].p_post_func)
			(*m_tasks[m_cur_task].p_post_func)(m_tasks[m_cur_task].user_data);

		// check if all tasks finished
		if (m_cur_task+1 < m_tasks_nb) {

			// start of next task
			m_cur_task++;
			if (m_tasks[m_cur_task].p_pre_func)
				(*m_tasks[m_cur_task].p_pre_func)(m_tasks[m_cur_task].user_data);

		} else {
			m_state = E_XFER_MNGR_IDLE;
			m_cur_task = 0;

			// TODO careful here
			m_tasks_nb = 0;

			LOG_INFO("Xfer all tasks finished\r\n");
		}

	}
}

void dma_spi0_mngr_finish() {

	if (m_state != E_XFER_MNGR_RUN) return;

	while (m_state == E_XFER_MNGR_RUN) {

		dma_spi0_mngr_run();

		while (m_state == E_XFER_MNGR_RUN && !isXferDone && isXferStarted) {

			sleep();

		}

	}


}
