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

@(#) $Id: csp_channel.c,v 1.1.1.1 2001/06/12 13:55:28 rickbeton Exp $
-------------------------------------------------------------------------------
csp_channel: implementation of a channel with 'occam' semantics
using Posix so-called lightweight threads.
This file defines the tricky parts. See also csp_channel2.c
*/
/*}}}  ----------------------------------------------------------------------*/

#include <stdio.h>
#include <csp.h>

// DPRINTF - this can be used for development, but bear in mind that print
// statements can themselves alter timing behaviour.
#define DPRINTF(x)
//#define DPRINTF(x) printf x

/*{{{  static lock/unlock*/
/*----------------------------------------------------------------------------
 */
static void mutexLock (pthread_mutex_t * mutex, int line)
{
    int status;
    status = pthread_mutex_lock (mutex);
    if (status != 0) { err_abort (status, "lock mutex", line); }
}


/*----------------------------------------------------------------------------
 */
static void mutexUnlock (pthread_mutex_t * mutex, int line)
{
    int status;
    status = pthread_mutex_unlock (mutex);
    if (status != 0) { err_abort (status, "unlock mutex", line); }
}

/*----------------------------------------------------------------------------
 */
static void condSignal (pthread_cond_t * cond, int line, const char * msg)
{
    int status;

    DPRINTF (("-%s signal-\n", msg));
    status = pthread_cond_signal (cond);
    if (status != 0) { err_abort (status, "cond signal", line); }
    DPRINTF (("-%s signal done-\n", msg));
}

/*----------------------------------------------------------------------------
 */
static void condWait (pthread_cond_t * cond,
                      pthread_mutex_t * mutex,
                      int line, const char * msg)
{
    int status;

    DPRINTF (("-%s wait loop-\n", msg));
    status = pthread_cond_wait (cond, mutex);
    if (status != 0) { err_abort (status, "cond wait", line); }
}

/*}}}  */
/*{{{  static checkAlt */
/*----------------------------------------------------------------------------
 */
static void checkAlt (Channel * c, const char * msg)
{
    if (c->alt != NULL)
    {
        mutexLock (&(c->alt->altMx), __LINE__);

        c->alt->selectedChannel = c;
        condSignal (&(c->alt->altSync), __LINE__, msg);

        mutexUnlock (&(c->alt->altMx), __LINE__);

        DPRINTF (("-%s signalled alt-\n", msg));
    }
}


//////////////////////////////////////////////////////////////////////////////


/*}}}  */
/*{{{  CSP_chan...Move */
/*----------------------------------------------------------------------------
 */
void CSP_chanOutMove (Channel * c, void ** cp)
{
    if (c->hasWriteQueue)
    {
        mutexLock (&(c->writeQueue), __LINE__);
        DPRINTF (("out locked writeQueue\n"));
    }

    mutexLock (&(c->mutex), __LINE__);
    DPRINTF (("out locked mutex\n"));

    checkAlt (c, "checkAlt CSP_chanOutMove");

    c->buffer  = *cp;
    *cp        = NULL;
    c->full    = 1;

    condSignal (&(c->fullSync), __LINE__, "signal full");

    while (c->full)
    {
        condWait (&(c->emptySync), &(c->mutex), __LINE__, "out wait empty");
    }

    mutexUnlock (&(c->mutex), __LINE__);

    if (c->hasWriteQueue)
    {
        mutexUnlock (&(c->writeQueue), __LINE__);
        DPRINTF (("out unlocked writeQueue\n"));
    }

    DPRINTF (("---out done---\n"));
}


/*----------------------------------------------------------------------------
 */
void CSP_chanInMove (Channel * c, void ** cp)
{
    if (c->hasReadQueue)
    {
        mutexLock (&(c->readQueue), __LINE__);
        DPRINTF (("in locked readQueue\n"));
    }

    mutexLock (&(c->mutex), __LINE__);
    while (! c->full)
    {
        condWait (&(c->fullSync), &(c->mutex), __LINE__, "in wait full");
    }

    *cp = c->buffer;

    c->full = 0;
    condSignal (&(c->emptySync), __LINE__, "in signal empty");
    mutexUnlock (&(c->mutex), __LINE__);

    if (c->hasReadQueue)
    {
        mutexUnlock (&(c->readQueue), __LINE__);
        DPRINTF (("in unlocked readQueue\n"));
    }
    DPRINTF (("---in done---\n"));
}


/*}}}  */
/*{{{  CSP_chan...Copy */
/*----------------------------------------------------------------------------
 */
void CSP_chanOutCopy (Channel * c, void * cp, int cnt)
{
    if (c->hasWriteQueue)
    {
        mutexLock (&(c->writeQueue), __LINE__);
        DPRINTF (("out locked writeQueue\n"));
    }

    mutexLock (&(c->mutex), __LINE__);
    DPRINTF (("out locked mutex\n"));

    checkAlt (c, "checkAlt CSP_chanOutCopy");

    c->buffer  = cp;
    c->integer = cnt;
    c->full    = 1;

    condSignal (&(c->fullSync), __LINE__, "signal full");

    while (c->full)
    {
        condWait (&(c->emptySync), &(c->mutex), __LINE__, "out wait empty");
    }

    mutexUnlock (&(c->mutex), __LINE__);

    if (c->hasWriteQueue)
    {
        mutexUnlock (&(c->writeQueue), __LINE__);
        DPRINTF (("out unlocked writeQueue\n"));
    }

    DPRINTF (("---out done---\n"));
}


/*----------------------------------------------------------------------------
 */
int CSP_chanInCopy (Channel * c, void * cp, int cnt)
{
    int n;
    if (c->hasReadQueue)
    {
        mutexLock (&(c->readQueue), __LINE__);
        DPRINTF (("in locked readQueue\n"));
    }

    mutexLock (&(c->mutex), __LINE__);
    while (! c->full)
    {
        condWait (&(c->fullSync), &(c->mutex), __LINE__, "in wait full");
    }

    if (cnt > 0)
    {
        CSP_assert (cnt == c->integer, "structure sizes differ",
                    __FILE__, __LINE__);
        n = c->integer;
    }
    else
    {
        cnt = -cnt;
        if (cnt > c->integer)
        {
            n = c->integer;
        }
        else
        {
            n = cnt;
        }
    }

    memcpy (cp, c->buffer, n);

    c->full = 0;
    condSignal (&(c->emptySync), __LINE__, "in signal empty");
    mutexUnlock (&(c->mutex), __LINE__);

    if (c->hasReadQueue)
    {
        mutexUnlock (&(c->readQueue), __LINE__);
        DPRINTF (("in unlocked readQueue\n"));
    }
    DPRINTF (("---in done---\n"));
    return n;
}


/*}}}  */
/*{{{  CSP_priAltSelect*/
/*----------------------------------------------------------------------------
   CSP_priAltSelect

   Blocks the calling process until one of the channel parameters is
   ready to input. The index returned for the ready channel is an integer
   that indicates the position of the channel in the parameter list. The
   index numbers begin at zero for the first parameter.

   CSP_priAltSelect only returns when a channel is ready to input. It does not
   perform the input operation, which must be done by the code following
   the call to CSP_priAltSelect.

   If more than one channel is ready to communicate, CSP alternation is
   defined to take an arbitrary selection from the ready channels.
   CSP_priAltSelect implements this as a priority list - channel c1 is given
   higher priority than c2, etc.

   The set of channels is specified either as an array, including its size,
   or as a null-terminated list. In the latter case, the number of channels
   nChans should be set to -1.

   CSP_Alt_t * alt : The alternative structure
   Channel * clist : The first in an array of pointers to channels.
   nChans          : the size of clist, or -1
   return          : An index into the parameter list for the ready
                     channel, or -1 if the first element in the array
                     is NULL.
*/
int CSP_priAltSelect (CSP_Alt_t * alt, Channel ** clist, int nChans)
{
    int i;
    int selected = -1;
    Channel * c = clist[0];

    if (c == NULL) { return -1; }

    // if nChans is negative, list is NULL terminated
    if (nChans < 0)
    {
        c = clist[0];
        for (i = 0; c != NULL; i++, c++) { }
        nChans = i;
    }

    // check all channels are one2one
    for (i = 0; i < nChans; i++)
    {
        c = clist[i];
        if (c->hasReadQueue)
        {
            CSP_assert (0, "only one2one and any2one channels are supported",
                        __FILE__, __LINE__);
         }
    }

    CSP_assert (nChans < 0 || nChans == i, "nChans - invalid value",
                __FILE__, __LINE__);

    // attempt to take all the mutex locks
    // and test for ready guards in the process
    for (i = 0; i < nChans; i++)
    {
        c = clist[i];
        mutexLock (&(c->mutex), __LINE__);
        DPRINTF (("alt1 locked mutex %d\n", i));

        if (c->full && c->guardEnabled)
        {
            // found waiting guard
            selected = i;
            mutexUnlock (&(c->mutex), __LINE__);
            DPRINTF (("alt2 unlocked mutex; selected %d\n", i));
            for (i--; i >= 0; i--)
            {
                c = clist[i];
                mutexUnlock (&(c->mutex), __LINE__);
                DPRINTF (("alt3 unlocked mutex %d\n", i));
            }
            return selected;
        }
    }

    //
    // At this point, we know that no guards are ready.
    // We hold the meetMx mutex in every channel.
    //

    mutexLock (&(alt->altMx), __LINE__);
    DPRINTF (("alt4 locked alt mutex\n"));

    // insert alt into all channels
    // and release all the meetMx mutexes
    for (i = 0; i < nChans; i++)
    {
        c = clist[i];
        c->alt = alt;
        mutexUnlock (&(c->mutex), __LINE__);
        DPRINTF (("alt5 unlocked mutex %d\n", i));
    }

    condWait (&(alt->altSync), &(alt->altMx), __LINE__, "alt");

    // at this point, alting thread has been signalled and
    // alt->selectedChannel is now defined

    // determine selected channel and remove alt from all channels
    for (i = 0; i < nChans; i++)
    {
        c = clist[i];
        if (c == alt->selectedChannel)
        {
            selected = i;
            DPRINTF (("alt6 selected %d\n", i));
        }

        mutexLock (&(c->mutex), __LINE__);
        DPRINTF (("alt7 locked mutex %d\n", i));
        c->alt = NULL;
        mutexUnlock (&(c->mutex), __LINE__);
        DPRINTF (("alt8 unlocked mutex %d\n", i));
    }

    mutexUnlock (&(alt->altMx), __LINE__);
    DPRINTF (("alt9 unlocked alt mutex\n"));

    return selected;
}


/*}}}  */
