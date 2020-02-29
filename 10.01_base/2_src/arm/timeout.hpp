/* timeout.hpp: timer based on realtime and arbitrary timebase

 Copyright (c) 2020, Joerg Hoppe
 j_hoppe@t-online.de, www.retrocmp.com

 Permission is hereby granted, free of charge, to any person obtaining a
 copy of this software and associated documentation files (the "Software"),
 to deal in the Software without restriction, including without limitation
 the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 JOERG HOPPE BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


 29.02.2020   JH      entered beta phase
 */

#ifndef _TIMEOUT_HPP_
#define _TIMEOUT_HPP_


#include <time.h>

#include "logsource.hpp"



class timeout_c: public logsource_c {
private:
	struct timespec starttime;
	uint64_t duration_ns;
public:
	timeout_c();
	uint64_t get_resolution_ns(void) ;
	void start_ns(uint64_t duration_ns);
	void start_us(uint64_t duration_us) ;
	void start_ms(uint64_t duration_ms) ;
	uint64_t elapsed_ns(void);
	uint64_t elapsed_us(void);
	uint64_t elapsed_ms(void);
	bool reached(void);
	static void wait_ns(uint64_t duration_ns);
	static void wait_us(unsigned duration_us);
	static void wait_ms(unsigned duration_ms);

};


#endif
