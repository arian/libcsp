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

@(#) $Id: process.h,v 1.1.1.1 2001/06/12 13:55:28 rickbeton Exp $
*/
/*}}}  ----------------------------------------------------------------------*/

#ifndef __process_h
#define __process_h

#include <csp.h>
#include <pthread.h>

#ifdef _SOLARIS
typedef int pthread_t;
#endif

typedef struct Process Process;
struct Process
{
    Process *link;        /* list of runnable/sleeping (MUST BE FIRST)*/
    //int wsret;            /* location of endp wp */
    //int *wbase;           /* pointer to bottom of workspace */
    //int wsize;            /* size of work space pointer */
    //int *wp;              /* inital wp */
    //int psize;            /* number of word sized parameters */
    void (*ofunc)();      /* pointer to function (process) */
    //Channel chan;         /* Used to indicate state */
    pthread_attr_t attr;
    pthread_t      threadId;
};

extern Process *_IMS_AllProc;

/*----------------------------------------------------------------------------
   ProcAlloc

   Allocates memory space for a process and initialises the allocated
   structure. Note: all processs must be allocated (by a call to ProcAlloc
   or ProcInit) before use.

   ProcAlloc takes as parameters a pointer to a function which is to be
   spawned as a CSP-process (i.e. typically a lightweight thread), the
   size of the workspace required by the process, and parameters to the
   function. It returns a pointer to an initialised process structure
   describing the process. The pointer is used to start the process by
   passing it to one of the process execution functions.

   If wsize is specified as zero, the default stack size is used. This is
   system dependent.

   psize specifies the number of words required on the stack initially for
   the function's parameters. If parameters are all word-sized (after
   default promotions have taken place) then psize should equal the number
   of parameters in the list. If parameters are not all word-sized, then
   psize must be the same as the number of words occupied. For example, if a
   structure is passed that occupies four words, and all other parameters
   are word-sized, then psize must be increased by four.

   ProcAlloc must have as its first parameter a pointer to a process
   structure. psize must not include this process pointer.

   Note: when using parameters larger than one word, allowance must be made
   for any default type promotions performed by the compiler by rounding
   up aggregate types to the nearest word.

   float variables cannot be passed directly as parameters because the
   promotion is to type double. In this case, and in all others where
   the parameter is large than a word, pointers should be used.

   ProcAlloc uses malloc to allocate stack space (allocated from the heap).
   If the call to malloc is unsuccessful, ProcAlloc returns a NULL pointer.
   All calls to ProcAlloc should be followed by a check for successful
   allocation and secure handling of a NULL result. The consequences of
   running an unallocated process are undefined.

   ProcAlloc calls the lower level function ProcInit to initialise the
   process structure.

   *func     : A pointer to the function to be created as a parallel process.
   int wsize : The amount of stack space required for the process, in bytes.
   int psize : The number of words taken up by the parameters.
   return    : A pointer to the process structure, or NULL if the
               allocation is unsuccessful.
*/
extern Process *ProcAlloc (void (* func)(), int wsize,
                           int psize, ...);

/*----------------------------------------------------------------------------
   ProcInit

   Takes as input a pointer to an existing Process structure and a pointer to
   the stack space to be used, and initialises the process stucture and
   workspace for the function according to its workspace and parameter space
   requirements. ProcInit is called by ProcAlloc to initialise the process
   structure.

   As with ProcAlloc, wsize specifies the number of words required on the
   stack initially for the function's parameters. If parameters are all word
   sized (after default promotions have taken place), then wsize should equal
   the number of parameters in the list. If parameters are not all word sized,
   then wsize must be the same as the number of words occupied. For example,
   if a structure is passed that occupies four words, and all other parameters
   are word-sized, then wsize must be increased by four.

   Note: when using parameters that consist of more than one word, take care
   to allow for any default type promotions performed by the compiler, and
   be sure to round up aggregate types to the nearest word.

   float variables cannot be passed directly as parameters because the
   promotion is to type double. In this case, and in all others where
   the parameter is large than a word, pointers should be used.

   ProcInit checks that enough space has been allocated for the function
   parameters, and that space has been allocated from the heap.
*/
extern int      ProcInit (Process * p, void (* func)(),
                          int * ws, int wsize,
                          int psize, ...);

/*----------------------------------------------------------------------------
   ProcAlloc
*/
extern void     ProcParam (Process * sf, ...);

/*----------------------------------------------------------------------------
   ProcAlloc
*/
extern int      ProcTime (void);

/*----------------------------------------------------------------------------
   ProcAlloc
*/
extern int      ProcGetPriority (void);

/*----------------------------------------------------------------------------
   ProcAlloc
*/
extern void     ProcReschedule (void);

/*----------------------------------------------------------------------------
   ProcAlloc
*/
extern void     ProcRun (Process * p);

/*----------------------------------------------------------------------------
   ProcAlloc
*/
extern void     ProcRunHigh (Process * p);

/*----------------------------------------------------------------------------
   ProcAlloc
*/
extern void     ProcRunLow (Process * p);

/*----------------------------------------------------------------------------
   ProcAlloc
*/
extern void     ProcPar (Process * p1, ...);

/*----------------------------------------------------------------------------
   ProcAlloc
*/
extern void     ProcParList (Process ** plist);

/*----------------------------------------------------------------------------
   ProcAlloc
*/
extern void     ProcPriPar (Process * phigh, Process * plow);

/*----------------------------------------------------------------------------
   ProcAlt

   Blocks the calling process until one of the channel parameters is
   ready to input. The index returned for the ready channel is an integer
   that indicates the position of the channel in the parameter list. The
   index numbers begin at zero for the first parameter.

   ProcAlt only returns when a channel is ready to input. It does not
   perform the input operation, which must be done by the code following
   the call to ProcAlt.

   Channel * c1 : The first in a NULL-terminated list of pointers to
                  channels.
   return       : An index into the parameter list for the ready channel.
*/
extern int      ProcAlt (Channel * c1, ...);

/*----------------------------------------------------------------------------
   ProcAltList

   As ProcAlt, but takes an array of pointers to channels. Returns -1
   if the clist array is empty.

   Channel ** clist : an array of pointers to channels, terminated by NULL.
   return           : An index into the parameter list for the ready
                      channel, or -1 if the first element in the array
                      is NULL.
*/
extern int      ProcAltList (Channel ** clist);

/*----------------------------------------------------------------------------
   ProcAlloc
*/
extern int      ProcSkipAlt (Channel * c1, ...);

/*----------------------------------------------------------------------------
   ProcAlloc
*/
extern int      ProcSkipAltList (Channel ** clist);

/*----------------------------------------------------------------------------
   ProcAlloc
*/
extern int      ProcTimerAlt (int time, Channel * c1, ...);

/*----------------------------------------------------------------------------
   ProcAlloc
*/
extern int      ProcTimerAltList (int time, Channel ** clist);

/*----------------------------------------------------------------------------
   ProcAlloc
*/
extern int      ProcTimePlus (const int a, const int b);

/*----------------------------------------------------------------------------
   ProcAlloc
*/
extern int      ProcTimeMinus (const int a, const int b);

/*----------------------------------------------------------------------------
   ProcAlloc
*/
extern int      ProcTimeAfter (const int a, const int b);

/*----------------------------------------------------------------------------
   ProcAfter
   Delays execution of the current process until a specified time.
   Time is expressed as an integer clock value.

   int time : the time after which the process will restart
*/
extern void     ProcAfter (int time);

/*----------------------------------------------------------------------------
   ProcAlloc
*/
extern void     ProcWait (int x);

/*----------------------------------------------------------------------------
   ProcAlloc
*/
extern void     ProcStop (void);

/*----------------------------------------------------------------------------
   ProcAlloc
*/
extern void     ProcInitClean (Process * p);

/*----------------------------------------------------------------------------
   ProcAllocClean

   Used to clean up after a process when it is known to have terminated.
   The process is denoted by the process pointer passed in as the
   argument, and must have been initially set up using ProcAlloc. It
   will not work correctly for processes set up using ProcInit and if
   used in such a case may produce undefined behaviour.

   ProcAllocClean removes the process structure pointed to by ts argument
   from the list of initialised processes and frees up any heap space
   used for the process structure and workspace.

   Caution: ProcAllocClean can only be used with synchronous processes,
   i.e. those started using ProcPar or ProcParList, and can be safely
   used only after the call to ProcPar or ProcParList has returned.
   Any other use of this function may give rise to undefined behaviour.

   Process * p : A pointer to a process structure.
*/
extern void     ProcAllocClean (Process * p);

/*----------------------------------------------------------------------------
   ProcAlloc
*/
extern int      ProcJoin (Process * p1, ...);

/*----------------------------------------------------------------------------
   ProcAlloc
*/
extern int      ProcJoinList (Process ** plist);

/*--------------------------------------------------------------------------*/

#define PROC_HIGH 0
#define PROC_LOW  1

#define CLOCKS_PER_SEC_HIGH     1000000
#define CLOCKS_PER_SEC_LOW      15625

#endif
