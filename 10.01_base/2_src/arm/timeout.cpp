/* timeout.cpp: timer based on realtime and arbitrary timebase

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





For timing, most thread ust wait (time) in hteir worker or else where

Delays is given in physical "real world" time.

I fdevice are used by simualted CPU, timebase must switch  to "emualted" time,
derived from CPU cycle executiion.
So if the CPU thread is stopped by the proiess scheduler, all timeous wait also.
This is important, else (seen from emualted CPU) devvices would seem to get 
"speed bursts". This may lead to for example for serial transmission OVERRUN conditions.


Implemenatiaon
 threads use wait()
wait(time_ns) has a mode flag:
real_time/emulated_time

if "real_time", waiting is fan with nano_sleep()
"emualted_time" is given by a 64bit emualted_e
(unit "ens" means "emualted anoseconds) ns nanosecond counter.
It is bumped up by the CPU on code execution,
or and some abriotrary intervals for CPU WAIT.

emualted_wait(delay_pause) {
	// cal wait timestmp
	signal_time_ens  =  dealy_pause_ns + now_ens ;
	// insert segnal & trogger tiem into list
	this.schedule_wackeup(signal_time_ens) ;
	
	waitfor(this.signal) ;    // Linux signal wait
	
	
}

schedule_wackeup(signal_time_ens) {
	create signal
	entry = new  enry(singal
	entry-signal_time_ens = singal:time_ens
	entry.signal= pthead_newsignal() ;
	srotedlist_add_by _time(entry) ;
}


emualted_ns counter is steped with
calss step(delta_ens)
	now_end += delta_nsens ;
	ofr (itaerate sorted signal lsit)
		if enty.signal_time <= now:en
			singal(entry.singal) ; // triggers waitfor() in waiting thread
			remove entry ;


On each step() it is schecked wheter a waiting thread is now signaled to continue.


*/

#include <assert.h>


#include "utils.hpp"
#include "timeout.hpp"

/*** time measuring ***/

timeout_c::timeout_c() {
	log_label = "TO";
}

uint64_t timeout_c::get_resolution_ns() {
	struct timespec res;
	clock_getres(CLOCK_MONOTONIC, &res);
	return BILLION * res.tv_sec + res.tv_nsec;
}

void timeout_c::start_ns(uint64_t duration_ns) {
	this->duration_ns = duration_ns;
	clock_gettime(CLOCK_MONOTONIC, &starttime);
}

void timeout_c::start_us(uint64_t duration_us) {
	start_ns(duration_us * 1000);
}

void timeout_c::start_ms(uint64_t duration_ms) {
	start_ns(duration_ms * MILLION);
}

uint64_t timeout_c::elapsed_ns(void) {
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	uint64_t result = (uint64_t) BILLION * (now.tv_sec - starttime.tv_sec)
			+ (uint64_t) now.tv_nsec - starttime.tv_nsec;
	return result;
}

uint64_t timeout_c::elapsed_us(void) {
	return elapsed_ns() / 1000;
}

uint64_t timeout_c::elapsed_ms(void) {
	return elapsed_ns() / MILLION;
}

bool timeout_c::reached() {
	return (elapsed_ns() > duration_ns);
}

/***
	Tests indicate that any nano_sleep() often causes delays of 60-80 µs
***/

// wait a number of nanoseconds, resolution in 0.1 millisecs
void timeout_c::wait_ns(uint64_t duration_ns) {
	struct timespec ts = { (long) (duration_ns / BILLION), (long) (duration_ns % BILLION) };
	int res = nanosleep(&ts, NULL);
	assert(res == 0);
//		DEBUG("nanosleep() return a %d", res);
}

// wait a number of milliseconds
void timeout_c::wait_ms(unsigned duration_ms) {
	wait_ns(MILLION * duration_ms);
}

void timeout_c::wait_us(unsigned duration_us) {
	wait_ns(1000L * duration_us);
}


