/*
 * BoucleFEC.h
 *
 *  Created on: 13 déc. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_MODEL_BOUCLEFEC_H_
#define SOURCE_MODEL_BOUCLEFEC_H_

#include <model/BoucleInterface.h>

class BoucleFEC: virtual public BoucleInterface {
public:
	BoucleFEC();
	void init();

	bool isTime();
	void run();

protected:
	uint32_t m_last_run_time;
};

#endif /* SOURCE_MODEL_BOUCLEFEC_H_ */
