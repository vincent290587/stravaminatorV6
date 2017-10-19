/*
 * Boucle.h
 *
 *  Created on: 19 oct. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_MODEL_BOUCLECRS_H_
#define SOURCE_MODEL_BOUCLECRS_H_

#include <stdint.h>
#include <stdbool.h>
#include "Boucle.h"


class BoucleCRS : public Boucle {
public:
	BoucleCRS();
	void init();

	bool isTime();
	void run();

protected:

};



#endif /* SOURCE_MODEL_BOUCLECRS_H_ */
