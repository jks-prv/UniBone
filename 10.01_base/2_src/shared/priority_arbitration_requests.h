/* priority_arbitration_requests.h: data struct to describe requests
                       Datastructures common to ARM and PRU.

   Copyright (c) 2019, Joerg Hoppe
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

    28-jun-2019	JH	begin

    PRU handles all 5 requests in parlalel.
    4x INMTR BR4-BR7
    1x DMA NPR.
    Several ARM devcie may raise same BR|NPR level, ARM must serialize this to PRU.

    1. ARM sets a REQUEST by
	filling the RQUEST struct and perhaps DMA data
    	doing AMR2PRO_PRIORITY_ARBITRATION_REQUEST,
    2. PRU sets BR4567|NPR lines according to open requests
    3. PRU monitors IN GRANT lines BG4567,NPG.
      IN state of idle requests is forwarded to BG|NPG OUT liens,
      to be processed by other UNIBUS cards.
      BG*|NPG IN state line of active request cleares BR*|NPR line,
      sets SACK, and starts INTR or DMA state machine.
    4. INTR or DMA sent a signal on compelte to PRU.
    	PRU may then start next request on same (completed) BR*|NPR level.

*/

// bits BR*/NPR interrupts in buslatch 0 and 1
// bit # is index into arbitration_request[] array.
#define PRIORITY_ARBITRATION_BIT_B4	0x01
#define PRIORITY_ARBITRATION_BIT_B5	0x02
#define PRIORITY_ARBITRATION_BIT_B6	0x04
#define PRIORITY_ARBITRATION_BIT_B7	0x08
#define PRIORITY_ARBITRATION_BIT_NP	0x10
#define PRIORITY_ARBITRATION_BIT_MASK	0x1f



// info for one of 5 busrequests (BR4,5,6,7,NPR) generated by ARM
typedef struct {
	// INTR BR* and DMA/NPR
	uint8_t	priority_bit ;  // PRIORITY_ARBITRATION_BIT_* for buslatch 0
	uint32_t data_transfer_complete_signal_tag ; // value returned with signal on complete

	// only INTR: interrupt register state to be set atomically with BR line
	uint8_t	iopage_register_handle ;
	uint16_t iopage_register_value ;
	uint16_t vector; // interrupt vector to be transferred
	// DMA transaction data in mailbox_dma_t struct
} priority_arbitration_request_t ;

// list of all requests, some possible active
typedef struct {
	// active BR/NPR signals, buslatch 0,	PRIORITY_ARBITRATION_*
	uint8_t active_request_bits ;
	// index by bit # in "pirity b:it
	// BR4->[0], .. BR7 -> [3], NPR->[4]
	// Bit # in GRANT line is index here
	priority_arbitration_request_t priority_arbitration_request[5] ;
} priority_arbitration_requests_t ;

