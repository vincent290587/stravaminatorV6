/*
 * usb_parser.h
 *
 *  Created on: 18 oct. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_PARSERS_USB_PARSER_H_
#define SOURCE_PARSERS_USB_PARSER_H_


#if defined(__cplusplus)
extern "C" {
#endif /* _cplusplus */

void usb_parser_decode(char c);

void usb_send_chars(const char *buffer);

void usb_printf(const char *format, ...);

#if defined(__cplusplus)
}
#endif /* _cplusplus */

#endif /* SOURCE_PARSERS_USB_PARSER_H_ */
