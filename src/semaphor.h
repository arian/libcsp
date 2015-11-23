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

@(#) $Id: semaphor.h,v 1.1.1.1 2001/06/12 13:55:28 rickbeton Exp $
*/
/*}}}  ----------------------------------------------------------------------*/

#ifndef __semaphore_h
#define __semaphore_h

#include <csp.h>

typedef struct Semaphore Semaphore;


typedef struct _IMS_SemPacket _IMS_SemPacket;
typedef struct _IMS_Sim_Semaphore _IMS_Sim_Semaphore;

struct _IMS_SemPacket
{
     Channel          __post;   /* synchronisation channel */
     _IMS_SemPacket  *__next;   /* pointer to next packet in sem chain */
};

struct _IMS_Sim_Semaphore
{
     unsigned int    __value;       /* value of semaphore */
     _IMS_SemPacket  *__head;       /* Head of list of waiting processes */
     _IMS_SemPacket  *__tail;       /* Tail of list of waiting processes */
};

struct Semaphore
{
     unsigned int       __value;    /* value of semaphore */
     _IMS_SemPacket    *__head;     /* Head of list of waiting processes */
     _IMS_SemPacket    *__tail;     /* Tail of list of waiting processes */
     _IMS_Sim_Semaphore __lo_sem;   /* low priority exclusion semaphore */
     _IMS_Sim_Semaphore __hi_sem;   /* high priority exclusion semaphore */
     int                __lo_using; /* flag to indicate use by low pri process */
     Channel            __hi_chan;  /* high priority exclusion channel */
};

#define SEMAPHOREINIT(initvalue) \
              {(initvalue),NULL,NULL,{1,NULL,NULL},{1,NULL,NULL}, \
               0,(Channel)NotProcess_p}


extern void       SemWait (Semaphore * /*s*/);
extern void       SemSignal (Semaphore * /*s*/);
extern void       SemInit (Semaphore * /*s*/, int /*initvalue*/);
extern Semaphore *SemAlloc (int /*initvalue*/);

#endif
