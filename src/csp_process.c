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

@(#) $Id: csp_process.c,v 1.1.1.1 2001/06/12 13:55:28 rickbeton Exp $
*/
/*}}}  ----------------------------------------------------------------------*/

#include <csp.h>
#include <stdio.h>
#include <csp.h>

Process *ProcAlloc (void (* func)(), int wsize,
                    int psize, ...)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

int      ProcInit (Process * p, void (* func)(),
                   int * ws, int wsize,
                   int psize, ...)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

void     ProcParam (Process * sf, ...)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

int      ProcTime (void)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

int      ProcGetPriority (void)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

void     ProcReschedule (void)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

void     ProcRun (Process * p)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

void     ProcRunHigh (Process * p)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

void     ProcRunLow (Process * p)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

void     ProcPar (Process * p1, ...)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

void     ProcParList (Process ** plist)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

void     ProcPriPar (Process * phigh, Process * plow)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

int      ProcAlt (Channel * c1, ...)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

int      ProcAltList (Channel ** clist)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

int      ProcSkipAlt (Channel * c1, ...)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

int      ProcSkipAltList (Channel ** clist)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

int      ProcTimerAlt (int time, Channel * c1, ...)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

int      ProcTimerAltList (int time, Channel ** clist)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

int      ProcTimePlus (const int a, const int b)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

int      ProcTimeMinus (const int a, const int b)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

int      ProcTimeAfter (const int a, const int b)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

void     ProcAfter (int x)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

void     ProcWait (int x)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

void     ProcStop (void)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

void     ProcInitClean (Process * p)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

void     ProcAllocClean (Process * p)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

int      ProcJoin (Process * p1, ...)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

int      ProcJoinList (Process ** plist)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

