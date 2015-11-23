/*{{{  ---------- CSP For Posix Threads ----------*/
/*  Copyright (C) 2000 Richard Beton

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    To contact the author refer to http://www.beton.freeserve.co.uk/libcsp/
    or email richard.beton@physics.org.

@(#) $Id: channel.h,v 1.1.1.1 2001/06/12 13:55:27 rickbeton Exp $
*/
/*}}}  ----------------------------------------------------------------------*/

#ifndef _included_channel_h
#define _included_channel_h

#include <csp.h>

/*----------------------------------------------------------------------------
   ChanAlloc

   Allocates and initialises a channel.

   Returns a pointer to an initialised channel, or NULL if the space
   could not be allocated.
*/
extern Channel      *ChanAlloc (void);

extern void          ChanDestroy (Channel * c);

/*----------------------------------------------------------------------------
   ChanOut

   Outputs cnt bytes of data on the channel chan. The data is taken from
   the array pointed to by cp.
*/
extern void          ChanOut (Channel * chan, void * cp, int cnt);

/*----------------------------------------------------------------------------
   ChanIn

   Inputs cnt bytes of data on the specified channel and stores them
   in the array pointed to by cp.
*/
extern void          ChanIn (Channel * chan, void * cp, int cnt);

/*----------------------------------------------------------------------------
   ChanInit

   Initialises the channel pointed to by c to the value NotProcess_p, defined
   below.
*/
extern void          ChanInit (Channel * c);

/*----------------------------------------------------------------------------
   ChanReset

   Resets a channel t the value NotProcess_p and returns the process
   descriptor of the channel waiting to communicate, or NotProcess_p.
   If the value returned is NotProcess_p, no process was waiting on
   the channel, and any communication on that channel had completed
   successfully.

   Returns either NotProcess_p or a process descriptor.
*/
extern int           ChanReset (Channel * c);

/*----------------------------------------------------------------------------
   ChanOutChar

   Outputs a single byte on a channel.
*/
extern void          ChanOutChar (Channel * c, unsigned char ch);

/*----------------------------------------------------------------------------
   ChanOutInt

   Outputs a single integer on a channel.
*/
extern void          ChanOutInt (Channel * c, int n);

/*----------------------------------------------------------------------------
   ChanInChar

   Inputs a single byte from a channel.
*/
extern unsigned char ChanInChar (Channel * c);

/*----------------------------------------------------------------------------
   ChanInInt

   Inputs a single integer from a channel.
*/
extern int           ChanInInt (Channel * c);

/*----------------------------------------------------------------------------
   ChanOutTimeFail

   Used to timeout channel communication. It outputs cnt bytes of data on
   the specified channel from the array pointed to by cp,
   or aborts if the tick-count reaches the specified count. Typically
   is is used to notify delay on a link so that communication can be
   routed elsewhere.

   Returns zero if communication completes, one if timeout occurs before
   the communication completes.
*/
extern int           ChanOutTimeFail (Channel * chan, void * cp,
                                      int cnt, int time);

/*----------------------------------------------------------------------------
   ChanOutChanFail

   Used to perform reliable channel communication on a link. This function
   outputs cnt bytes of data on the specified channel from the array
   pointed to by cp. It can be aborted by an integer (and only an integer)
   passed on failchan. Typically, failchan is a channel from a process
   that is monitoring the integrity of the link.

   Returns zero if communication completes, one if communication is aborted
   by a message on the failure channel.
*/
extern int           ChanOutChanFail (Channel * chan, void * cp,
                                      int cnt, Channel * failchan);

/*----------------------------------------------------------------------------
   ChanInTimeFail

   Used to timeout channel communication. It inputs cnt bytes of data on
   the specified channel and stores them in the array pointed to by cp,
   or aborts if the tick-count reaches the specified count. Typically
   is is used to notify delay on a link so that communication can be
   routed elsewhere.

   Returns zero if communication completes, one if timeout occurs before
   the communication completes.
*/
extern int           ChanInTimeFail (Channel * chan, void * cp,
                                     int cnt, int time);

/*----------------------------------------------------------------------------
   ChanInChanFail

   Used to perform reliable channel communication on a link. This function
   inputs cnt bytes of data on the specified channel into the array
   pointed to by cp. It can be aborted by an integer (and only an integer)
   passed on failchan. Typically, failchan is a channel from a process
   that is monitoring the integrity of the link.

   Returns zero if communication completes, one if communication is aborted
   by a message on the failure channel.
*/
extern int           ChanInChanFail (Channel * chan, void * cp,
                                     int cnt, Channel * failchan);

//////////////////////////////////////////////////////////////////////////////
// NOT IMPLEMENTED YET
//
//extern void          DirectChanIn (Channel * c, void * cp,
//                                   int count);
//extern unsigned char DirectChanInChar (Channel * c);
//extern int           DirectChanInInt (Channel * c);
//extern void          DirectChanOut (Channel * c, void * cp,
//                                    int count);
//extern void          DirectChanOutChar (Channel * c, unsigned char ch);
//extern void          DirectChanOutInt (Channel * c, int n);
//
//////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// Transputer-Specific Declarations - Not used elsewhere //
///////////////////////////////////////////////////////////

/* Remember that code intended for breakpoint debugging may not
   work if the following physical link values are used.          */

#if _PTYPE == '2' || _PTYPE == '3'

/* Addresses for physical links on 16 bit transputers */

#define LINK0OUT        ((Channel *) 0x8000)
#define LINK1OUT        ((Channel *) 0x8002)
#define LINK2OUT        ((Channel *) 0x8004)
#define LINK3OUT        ((Channel *) 0x8006)
#define LINK0IN         ((Channel *) 0x8008)
#define LINK1IN         ((Channel *) 0x800a)
#define LINK2IN         ((Channel *) 0x800c)
#define LINK3IN         ((Channel *) 0x800e)

/* Address of event pin on 16 bit transputers */

#define EVENT           ((Channel *) 0x8010)

#define NotProcess_p    0x8000

#else

/* Address for physical links on 32 bit transputers */

#define LINK0OUT        ((Channel *) 0x80000000)
#define LINK1OUT        ((Channel *) 0x80000004)
#define LINK2OUT        ((Channel *) 0x80000008)
#define LINK3OUT        ((Channel *) 0x8000000c)
#define LINK0IN         ((Channel *) 0x80000010)
#define LINK1IN         ((Channel *) 0x80000014)
#define LINK2IN         ((Channel *) 0x80000018)
#define LINK3IN         ((Channel *) 0x8000001c)

/* Address of event pin on 32 bit transputers */
#define EVENT           ((Channel *) 0x80000020)

#define NotProcess_p    0x80000000

#endif


#endif // _included_channel_h
