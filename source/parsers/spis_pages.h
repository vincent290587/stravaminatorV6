/*
 * spis_pages.h
 *
 *  Created on: 6 déc. 2017
 *      Author: Vincent
 */

#ifndef SPIS_PAGES_H_
#define SPIS_PAGES_H_

#include <stdint.h>
#include <stdbool.h>
#include "mk64f_parser.h"

#define TX_BUFF_FLAGS_POS         0U

#define TX_BUFF_FLAGS_LNS_BIT     0U
#define TX_BUFF_FLAGS_HRM_BIT     1U
#define TX_BUFF_FLAGS_BSC_BIT     2U
#define TX_BUFF_FLAGS_FEC_BIT     4U

#define TX_BUFF_FLAGS_SIZE        1U
#define TX_BUFF_LNS_SIZE          16U
#define TX_BUFF_HRM_SIZE          3U
#define TX_BUFF_BSC_SIZE          8U
#define TX_BUFF_FEC_SIZE          6U

#define TX_BUFF_HRM_START         (TX_BUFF_FLAGS_SIZE)
#define TX_BUFF_BSC_START         (TX_BUFF_HRM_START+TX_BUFF_HRM_SIZE)
#define TX_BUFF_LNS_START         (TX_BUFF_BSC_START+TX_BUFF_BSC_SIZE)
#define TX_BUFF_FEC_START         (TX_BUFF_LNS_START+TX_BUFF_LNS_SIZE)
#define TX_BUFF_TOT_SIZE          (TX_BUFF_FEC_START+TX_BUFF_FEC_SIZE)

#define RX_BUFF_PAGE_POS          0U

#define RX_BUFF_PAGE_SIZE         1U
#define RX_BUFF_BAT_SIZE          3U
#define RX_BUFF_GLA_SIZE          3U
#define RX_BUFF_NEO_SIZE          5U
#define RX_BUFF_FEC_SIZE          9U
#define RX_BUFF_BACK_SIZE         2U

#define RX_BUFF_BAT_START         (RX_BUFF_PAGE_SIZE)
#define RX_BUFF_GLA_START         (RX_BUFF_BAT_START + RX_BUFF_BAT_SIZE)
#define RX_BUFF_NEO_START         (RX_BUFF_GLA_START + RX_BUFF_GLA_SIZE)
#define RX_BUFF_FEC_START         (RX_BUFF_NEO_START + RX_BUFF_NEO_SIZE)
#define RX_BUFF_BACK_START        (RX_BUFF_FEC_START + RX_BUFF_FEC_SIZE)
#define RX_BUFF_TOT_SIZE          (RX_BUFF_BACK_START+ RX_BUFF_BACK_SIZE)

#ifdef __cplusplus
extern "C" {
#endif

// DECODE

void spis_encode_page0(sSpisRxInfo *input, uint8_t *rx_buf);

void spis_encode_page1(sSpisRxInfo *input, uint8_t *rx_buf);

// ENCODE

bool spis_decode_lns(uint8_t* p_rx_buf, sLnsInfo* info);

bool spis_decode_hrm(uint8_t* p_rx_buf, sHrmInfo* info);

bool spis_decode_bsc(uint8_t* p_rx_buf, sBscInfo* info);

bool spis_decode_fec(uint8_t* p_rx_buf, sFecInfo* info);

#ifdef __cplusplus
}
#endif

#endif /* SPIS_PAGES_H_ */
