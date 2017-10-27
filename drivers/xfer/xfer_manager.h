/*
 * xfer_manager.h
 *
 *  Created on: 27 oct. 2017
 *      Author: Vincent
 */

#ifndef DRIVERS_XFER_XFER_MANAGER_H_
#define DRIVERS_XFER_XFER_MANAGER_H_

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



#ifdef __cplusplus
}
#endif

#endif /* DRIVERS_XFER_XFER_MANAGER_H_ */
