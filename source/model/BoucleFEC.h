/*
 * BoucleFEC.h
 *
 *  Created on: 13 déc. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_MODEL_BOUCLEFEC_H_
#define SOURCE_MODEL_BOUCLEFEC_H_

#include <Boucle.h>

class BoucleFEC: virtual public BoucleInterface {
public:
	BoucleFEC();
	void init();

	bool isTime();
	void run();

protected:

};

#endif /* SOURCE_MODEL_BOUCLEFEC_H_ */
