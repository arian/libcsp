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

@(#) $Id: process.c,v 1.1.1.1 2001/06/12 13:55:28 rickbeton Exp $
*/
/*}}}  ----------------------------------------------------------------------*/

#include <csp.h>
#include <process.h>
#include <channel.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <stdarg.h>

/*----------------------------------------------------------------------------
 */
static void* caller (void * arg)
{
    Process* p = (Process*) arg;

    p->ofunc();

    return NULL;
}

/*----------------------------------------------------------------------------
 */
static void setPriority (Process* p, int policy, int priority)
{
#ifdef _POSIX_THREAD_PRIORITY_SCHEDULING

    int max = sched_get_priority_max (policy);
    int min = sched_get_priority_min (policy);
    struct sched_param param;

    if (min == -1) { errno_abort ("sched_get_priority_min"); }
    if (max == -1) { errno_abort ("sched_get_priority_max"); }

    printf ("setPriority %d (%d .. %d)\n", priority, min, max);

    if (priority < min) { priority = min; }
    if (priority > max) { priority = max; }
    param.sched_priority = priority;
    status = pthread_attr_setschedparam (&(p->attr), &param);
    if (status != 0) { err_abort (status, "pthread_attr_setschedparam", __LINE__); }

#endif // _POSIX_THREAD_PRIORITY_SCHEDULING
}

/*----------------------------------------------------------------------------
 */
Process *ProcAlloc (void (* func)(), int wsize,
                    int psize, ...)
{
    Process* proc = (Process*) malloc (sizeof (Process));
    if (proc == NULL) { err_abort (0, "malloc failed", __LINE__); }

    ProcInit (proc, func, NULL, wsize, psize);
    return proc;
}

/*----------------------------------------------------------------------------
 */
int      ProcInit (Process * p, void (* func)(),
                   int * ws, int wsize,
                   int psize, ...)
{
    int status;

    p->ofunc = func;

    status = pthread_attr_init (&(p->attr)); /* initialize attr with default attributes */
    if (status != 0) { err_abort (status, "pthread_attr_init", __LINE__); }

    status = pthread_attr_setschedpolicy (&(p->attr), SCHED_RR);
    if (status != 0) { err_abort (status, "pthread_attr_setschedpolicy", __LINE__); }

    return 0;
}

/*----------------------------------------------------------------------------
 */
void     ProcParam (Process * sf, ...)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

/*----------------------------------------------------------------------------
 */
int      ProcTime (void)
{
    return 0; //CSP_procTime();
}

/*----------------------------------------------------------------------------
 */
int      ProcGetPriority (void)
{
    return 0; //CSP_procGetPriority();
}

/*----------------------------------------------------------------------------
 */
void     ProcReschedule (void)
{
    sched_yield();
}

/*----------------------------------------------------------------------------
 */
void     ProcRun (Process * p)
{
    int status;
    status = pthread_create (&(p->threadId), &(p->attr), caller, p);
    if (status != 0) { err_abort (status, "pthread_create", __LINE__); }
}

/*----------------------------------------------------------------------------
 */
void     ProcRunHigh (Process * p)
{
    int status;
    status = pthread_attr_setschedpolicy (&(p->attr), SCHED_FIFO);
    if (status != 0) { err_abort (status, "pthread_attr_setschedpolicy", __LINE__); }

    setPriority (p, SCHED_FIFO, 1);

    ProcRun (p);
}

/*----------------------------------------------------------------------------
 */
void     ProcRunLow (Process * p)
{
    //setPriority (p, SCHED_RR, 0); -- is default
    ProcRun (p);
}

/*----------------------------------------------------------------------------
 */
void     ProcPar (Process * p1, ...)
{
    va_list ap;
    Process * p;
    void * result;
    int status;

    // first start all the processes
    va_start(ap, p1);
    p = p1;
    while (p != NULL)
    {
        status = pthread_create (&(p->threadId), &(p->attr), caller, p);
        if (status != 0) { err_abort (status, "pthread_create", __LINE__); }
        p = va_arg (ap, Process*);
    }
    va_end(ap);

    // now wait for all to finish
    va_start(ap, p1);

    p = p1;
    while (p != NULL)
    {
        status = pthread_join (p->threadId, &result);
        if (status != 0) { err_abort (status, "pthread_join", __LINE__); }
        p = va_arg (ap, Process*);
    }
    va_end(ap);
}

/*----------------------------------------------------------------------------
 */
void     ProcParList (Process ** plist)
{
    int status;
    void * result;
    Process * p;

    // first start all the processes
    for (p = plist[0]; p != NULL; p++)
    {
        status = pthread_create (&(p->threadId), &(p->attr), caller, p);
        if (status != 0) { err_abort (status, "pthread_create", __LINE__); }
    }

    // now wait for all to finish
    for (p = plist[0]; p != NULL; p++)
    {
        status = pthread_join (p->threadId, &result);
        if (status != 0) { err_abort (status, "pthread_join", __LINE__); }
    }
}

/*----------------------------------------------------------------------------
 */
void     ProcPriPar (Process * phigh, Process * plow)
{
    ProcRunHigh (phigh);
    ProcRunLow  (plow);
    ProcJoin (phigh, plow, NULL);
}

/*----------------------------------------------------------------------------
   ProcAlt

   Blocks the calling process until one of the channel parameters is
   ready to input. The index returned for the ready channel is an integer
   that indicates the position of the channel in the parameter list. The
   index numbers begin at zero for the first parameter.

   ProcAlt only returns when a channel is ready to input. It does not
   perform the input operation, which must be done by the code following
   the call to ProcAlt.

   If more than one channel is ready to communicate, CSP alternation is
   defined to take an arbitrary selection from the ready channels.
   ProcAlt implements this as a priority list - channel c1 is given
   higher priority than c2, etc.

   Channel * c1 : The first in a NULL-terminated list of pointers to
                  channels.
   return       : An index into the parameter list for the ready channel.
 */
int      ProcAlt (Channel * c1, ...)
{
    va_list ap;
    Channel * c;
    Channel ** list;
    CSP_Alt_t alt;
    int nChans = 0, index;

    // count the number of channels
    va_start(ap, c1);
    c = c1;
    while (c != NULL)
    {
        nChans++;
        c = va_arg (ap, Channel*);
    }
    va_end(ap);

    if (nChans == 0) { return -1; }

    // allocate some space and copy the channels to the new list
    list = (Channel**) malloc (sizeof(Channel*) * nChans);
    if (list == NULL) { return -1; }

    va_start(ap, c1);
    c = c1;
    for (index = 0; index < nChans; index++)
    {
        list[index] = va_arg (ap, Channel*);
    }
    va_end(ap);

    // call main selector
    CSP_altInit (&alt);
    index = CSP_priAltSelect (&alt, list, nChans);
    CSP_altClose (&alt);

    free (list);
    return index;
}

/*----------------------------------------------------------------------------
   ProcAltList

   As ProcAltList, but takes an array of pointers to channels. Returns -1
   if the clist array is empty.

   Channel ** clist : an array of pointers to channels, terminated by NULL.
   return           : An index into the parameter list for the ready
                      channel, or -1 if the first element in the array
                      is NULL.
 */
int      ProcAltList (Channel ** clist)
{
    int index;
    CSP_Alt_t alt;

    // call main selector
    CSP_altInit (&alt);
    index = CSP_priAltSelect (&alt, clist, -1);
    CSP_altClose (&alt);

    return index;
}

/*----------------------------------------------------------------------------
 */
int      ProcSkipAlt (Channel * c1, ...)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

/*----------------------------------------------------------------------------
 */
int      ProcSkipAltList (Channel ** clist)
{
    return 0; //CSP_procSkipAltList (clist);
}

/*----------------------------------------------------------------------------
 */
int      ProcTimerAlt (int time, Channel * c1, ...)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

/*----------------------------------------------------------------------------
 */
int      ProcTimerAltList (int time, Channel ** clist)
{
    return 0; //CSP_procTimerAltList (time, clist);
}

/*----------------------------------------------------------------------------
 */
int      ProcTimePlus (const int a, const int b)
{
    return 0; //CSP_procTimePlus (a, b);
}

/*----------------------------------------------------------------------------
 */
int      ProcTimeMinus (const int a, const int b)
{
    return 0; //CSP_procTimeMinus (a, b);
}

/*----------------------------------------------------------------------------
 */
int      ProcTimeAfter (const int a, const int b)
{
    return 0; //CSP_procTimeAfter (a, b);
}

/*----------------------------------------------------------------------------
 */
void     ProcAfter (int x)
{
    //CSP_procAfter (x);
}

/*----------------------------------------------------------------------------
 */
void     ProcWait (int x)
{
    //CSP_procWait (x);
}

/*----------------------------------------------------------------------------
 */
void     ProcStop (void)
{
    //CSP_procStop();
}

/*----------------------------------------------------------------------------
 */
void     ProcInitClean (Process * p)
{
    int status;
    status = pthread_attr_destroy (&(p->attr));
    if (status != 0) { err_abort (status, "pthread_attr_destroy", __LINE__); }
}

/*----------------------------------------------------------------------------
 */
void     ProcAllocClean (Process * p)
{
    ProcInitClean (p);
    free (p);
}

/*----------------------------------------------------------------------------
 */
int      ProcJoin (Process * p1, ...)
{
    va_list ap;
    Process * p;
    void * result;
    int status;

    va_start(ap, p1);
    p = p1;
    while (p != NULL)
    {
        status = pthread_join (p->threadId, &result);
        if (status != 0) { err_abort (status, "pthread_mutex_join", __LINE__); }
        p = va_arg (ap, Process*);
    }

    va_end(ap);
    return 0;
}

/*----------------------------------------------------------------------------
 */
int      ProcJoinList (Process ** plist)
{
    int status;
    void * result;
    Process * p;

    for (p = plist[0]; p != NULL; p++)
    {
        status = pthread_join (p->threadId, &result);
        if (status != 0) { err_abort (status, "pthread_mutex_join", __LINE__); }
    }
    return 0;
}

