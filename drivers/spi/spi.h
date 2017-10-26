/*
 * ls027.h
 *
 *  Created on: 29 sept. 2017
 *      Author: Vincent
 */

#ifndef SRC_SPI_H_
#define SRC_SPI_H_


#include "fsl_dspi.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#if !defined(WIN32)
#include <stdbool.h>
#endif


typedef struct {
	uint8_t* masterTxData;
	uint8_t* masterRxData;
	uint16_t spi_tx_data_length;
	uint32_t configFlags;
} spi_transfer_settings;

/////////    FUNCTIONS

#if defined(__cplusplus)
extern "C" {
#endif /* _cplusplus */

void spi_init(dspi_master_config_t* masterConfig);

void spi_start_transfer(spi_transfer_settings* spi_settings, bool is_last_byte);

#if defined(__cplusplus)
}
#endif /* _cplusplus */

#endif /* SRC_SPI_H_ */
