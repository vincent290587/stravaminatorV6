/*
 * Boucle.h
 *
 *  Created on: 19 oct. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_MODEL_BOUCLEINTERFACE_H_
#define SOURCE_MODEL_BOUCLEINTERFACE_H_

#include <stdint.h>
#include <stdbool.h>


class BoucleInterface {
public:
	BoucleInterface() {}

	void tasks() { if (this->isTime()) this->run(); }

	virtual bool isTime() {return false;}
	virtual void run() {}

protected:


};


#endif /* SOURCE_MODEL_BOUCLEINTERFACE_H_ */
