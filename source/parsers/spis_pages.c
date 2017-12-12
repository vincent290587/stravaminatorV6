/*
 * spis_pages.c
 *
 *  Created on: 6 déc. 2017
 *      Author: Vincent
 */

#include "utils.h"
#include "spis_pages.h"
#include "mk64f_parser.h"


/**
 *
 * @param info
 */
bool spis_decode_lns(uint8_t* p_rx_buf, sLnsInfo* info) {

	if (p_rx_buf[TX_BUFF_FLAGS_POS] & (1 << TX_BUFF_FLAGS_LNS_BIT)) {

		info->lat = decode_uint32 (p_rx_buf + TX_BUFF_LNS_START +  0);
		info->lon = decode_uint32 (p_rx_buf + TX_BUFF_LNS_START +  4);
		info->ele = decode_uint32 (p_rx_buf + TX_BUFF_LNS_START +  8);
		info->secj = decode_uint32 (p_rx_buf + TX_BUFF_LNS_START +  12);

		return true;
	}
	return false;
}

/**
 *
 * @param info
 */
bool spis_decode_hrm(uint8_t* p_rx_buf, sHrmInfo* info) {

	if (p_rx_buf[TX_BUFF_FLAGS_POS] & (1 << TX_BUFF_FLAGS_HRM_BIT)) {

		info->bpm = p_rx_buf[TX_BUFF_HRM_START];
		info->rr = decode_uint16 (p_rx_buf + TX_BUFF_HRM_START + 1);

		return true;
	}
	return false;
}

/**
 *
 * @param info
 */
bool spis_decode_bsc(uint8_t* p_rx_buf, sBscInfo* info) {

	if (p_rx_buf[TX_BUFF_FLAGS_POS] & (1 << TX_BUFF_FLAGS_BSC_BIT)) {

		info->cadence = decode_uint32 (p_rx_buf + TX_BUFF_BSC_START + 0);
		info->speed   = decode_uint32 (p_rx_buf + TX_BUFF_BSC_START + 4);

		return true;
	}
	return false;
}

/**
 *
 * @param info
 */
bool spis_decode_fec(uint8_t* p_rx_buf, sFecInfo* info) {

	if (p_rx_buf[TX_BUFF_FLAGS_POS] & (1 << TX_BUFF_FLAGS_FEC_BIT)) {

		info->el_time = decode_uint16 (p_rx_buf + TX_BUFF_FEC_START + 0);
		info->speed   = decode_uint16 (p_rx_buf + TX_BUFF_FEC_START + 2);
		info->power   = decode_uint16 (p_rx_buf + TX_BUFF_FEC_START + 4);

		return true;
	}
	return false;
}

/**
 *
 * @param rx_buf
 * @param input
 */
void spis_encode_page0(sSpisRxInfo *input, uint8_t *rx_buf) {

	rx_buf[RX_BUFF_PAGE_POS]       = eSpiRxPage0;

	rx_buf[RX_BUFF_BAT_START]      = input->pages.page0.batt_info.soc;

	encode_uint16(rx_buf + RX_BUFF_BAT_START + 1U, input->pages.page0.batt_info.mv);

	rx_buf[RX_BUFF_GLA_START]      = input->pages.page0.glasses_info.led;
	rx_buf[RX_BUFF_GLA_START + 1U] = input->pages.page0.glasses_info.av_ent;
	rx_buf[RX_BUFF_GLA_START + 2U] = input->pages.page0.glasses_info.av_dec;

	rx_buf[RX_BUFF_NEO_START]      = input->pages.page0.neo_info.event_type;
	rx_buf[RX_BUFF_NEO_START + 1U] = input->pages.page0.neo_info.on_time;
	rx_buf[RX_BUFF_NEO_START + 2U] = input->pages.page0.neo_info.rgb[0];
	rx_buf[RX_BUFF_NEO_START + 3U] = input->pages.page0.neo_info.rgb[1];
	rx_buf[RX_BUFF_NEO_START + 4U] = input->pages.page0.neo_info.rgb[2];

	rx_buf[RX_BUFF_FEC_START]      = input->pages.page0.fec_info.type;

	if (input->pages.page0.fec_info.type == eFecControlTargetPower) {
		encode_uint16 (rx_buf + RX_BUFF_FEC_START + 1U, input->pages.page0.fec_info.data.power_control.target_power_w);
	} else if (eFecControlSlope) {
		encode_uint32 (rx_buf + RX_BUFF_FEC_START + 1U,
				(uint32_t)input->pages.page0.fec_info.data.slope_control.slope_ppc);
		encode_uint32 (rx_buf + RX_BUFF_FEC_START + 5U,
				(uint32_t)input->pages.page0.fec_info.data.slope_control.rolling_resistance);
	}

	rx_buf[RX_BUFF_BACK_START]      = input->pages.page0.back_info.freq;
	rx_buf[RX_BUFF_BACK_START + 1U] = input->pages.page0.back_info.state;
}

/**
 *
 * @param rx_buf
 * @param input
 */
void spis_encode_page1(sSpisRxInfo *input, uint8_t *rx_buf) {
	rx_buf[RX_BUFF_PAGE_POS] = eSpiRxPage1;
}

