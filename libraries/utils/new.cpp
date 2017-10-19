/* Teensyduino Core Library
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2017 PJRC.COM, LLC.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * 1. The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "segger_wrapper.h"
#include "new.h"

//extern unsigned int __HeapLimit;
//extern unsigned int __HeapBase;
//
//static uint32_t total_alloc = 0;
//
//void * operator new(size_t size)
//{
//	void *p = calloc(size, 1);
//	if (p) {
//		total_alloc += size;
//		LOG_INFO("Alloc %u - %u / %u\r\n", size, total_alloc, __HeapLimit - __HeapBase);
//	} else {
//		LOG_INFO("MEM ERROR\r\n");
//	}
//	return p;
//}
//
//void * operator new[](size_t size)
//{
//	void *p = calloc(size, 1);
//	if (p) {
//		total_alloc += size;
//		LOG_INFO("Alloc %u - %u / %u\r\n", size, total_alloc, __HeapLimit - __HeapBase);
//	} else {
//		LOG_INFO("MEM ERROR\r\n");
//	}
//	return p;
//}
//
//void operator delete(void * ptr)
//{
//	free(ptr);
//}
//
//void operator delete[](void * ptr)
//{
//	free(ptr);
//}
//
//void operator delete(void * ptr, size_t size)
//{
//	total_alloc -= size;
//	free(ptr);
//}
//
//void operator delete[](void * ptr, size_t size)
//{
//	total_alloc -= size;
//	free(ptr);
//}

//int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);};
//void __cxa_guard_release (__guard *g) {*(char *)g = 1;};
//void __cxa_guard_abort (__guard *) {}; 

//void __cxa_pure_virtual(void) {};

