/*
 * spi_scheduler.c
 *
 *  Created on: 8 déc. 2017
 *      Author: Vincent
 */

#include <stdbool.h>
#include "segger_wrapper.h"
#include "spi_scheduler.h"

static sXferTask m_tasks[5];
static uint8_t m_tasks_nb;
static uint8_t m_cur_task;
static xferTaskState m_state;

extern bool isSpiTransferCompleted;

void dma_spi0_mngr_init() {

	m_state = E_XFER_MNGR_IDLE;
	m_cur_task = 0;
	m_tasks_nb = 0;

	isSpiTransferCompleted = true;
}

void dma_spi0_mngr_stop() {

	if (m_state == E_XFER_MNGR_RUN) {
		// post hook
		(*m_tasks[m_cur_task].p_post_func)(m_tasks[m_cur_task].user_data);
	}

	m_state = E_XFER_MNGR_IDLE;
	m_cur_task = 0;

	isSpiTransferCompleted = true;
}

void dma_spi0_mngr_task_add(sXferTask *task) {

	memcpy(m_tasks+m_tasks_nb, task, sizeof(sXferTask));
	m_tasks_nb++;

	LOG_INFO("Xfer task added\r\n");

}

void dma_spi0_mngr_tasks_start() {

	if (m_state == E_XFER_MNGR_RUN) {
		dma_spi0_mngr_stop();
	}

	m_cur_task = 0;
	isSpiTransferCompleted = true;

	if (m_tasks_nb) {
		m_state = E_XFER_MNGR_RUN;

		// start pre-hook of first task
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
	while (m_state == E_XFER_MNGR_RUN && isSpiTransferCompleted) {

		int res = (*m_tasks[m_cur_task].p_xfer_func)(m_tasks[m_cur_task].user_data);

		if (res == 0) {
			// the task is finished
			(*m_tasks[m_cur_task].p_post_func)(m_tasks[m_cur_task].user_data);

			// check if all tasks finished
			if (m_cur_task+1 < m_tasks_nb) {

				// start of next task
				m_cur_task++;
				(*m_tasks[m_cur_task].p_pre_func)(m_tasks[m_cur_task].user_data);

			} else {
				m_state = E_XFER_MNGR_IDLE;
			}

		}
	}
}

void dma_spi0_mngr_finish() {

	if (m_state != E_XFER_MNGR_RUN) return;

	// TODO
//	sleep();

	// make sure we fill the DMA buffers every time with a while
	while (m_state == E_XFER_MNGR_RUN) {

		if (isSpiTransferCompleted) dma_spi0_mngr_run();

	}

	// TODO
//	run();

}
