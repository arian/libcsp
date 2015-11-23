/*  CSP For Posix Threads
    Copyright (C) 2000 Richard Beton

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

$Id: commstime.c,v 1.1.1.1 2001/06/12 13:55:27 rickbeton Exp $
*/

/*
 * Command Line Arguments:
 *     commstime <loops per iterations> [<number of iterations>]
 * Number of iterations is unbounded unless specified.
 */

#include <process.h>
#include <csp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int nLoops = 1000;
int nIterations = -1;

/*{{{  BBlock_prefix*/
void BBlock_prefix (int first, Channel * in, Channel * out)
{
    int value;
    CSP_chanOutInt (out, first);
    while (1)
    {
        value = CSP_chanInInt (in);
        CSP_chanOutInt (out, value);
    }
}

/*}}}  */
/*{{{  BBlock_delta*/
void BBlock_delta (Channel * in, Channel * out1, Channel * out2)
{
    int value;
    while (1)
    {
        value = CSP_chanInInt (in);

        /* in a classic occam 'delta' process, these outputs would
         * be in parallel ('PAR') */
        CSP_chanOutInt (out1, value);
        CSP_chanOutInt (out2, value);
    }
}

/*}}}  */
/*{{{  BBlock_succ*/
void BBlock_succ (Channel * in, Channel * out)
{
    int value;
    while (1)
    {
        value = CSP_chanInInt (in);
        CSP_chanOutInt (out, value + 1);
    }
}

/*}}}  */
/*{{{  BBlock_consume*/
void BBlock_consume (int nLoops, Channel * in)
{
    int i;
    float loop;
    time_t starttime, endtime;
    /*{{{  warm-up loop*/
    for (i = 0; i < 16; i++)
    {
        int value;
        value = CSP_chanInInt (in);
    }
    /*}}}  */

    while (nIterations != 0)
    {
        time (&starttime);
        for (i = 0; i < nLoops; i++)
        {
            int value;
            value = CSP_chanInInt (in);
        }
        time (&endtime);
        printf( "Finished %d loops.\n", nLoops );
        if (nLoops >= 1000)
        {
            loop = ((float) (endtime - starttime)) / ((float) nLoops);
            printf( "Time per loop = %1.0fus\n", loop/1.0e-6 );
            printf( "Context switch = %1.1fus\n\n", loop/8.0e-6 );
        }
        if (nIterations > 0) { nIterations--; }
    }

    exit(0); /* ok to terminate explicitly because several
                threads are in infinite loops */
}

/*}}}  */
/*{{{  main program*/

Channel *a, *b, *c, *d;

void proc_prefix (void)  { BBlock_prefix (0, b, a); }
void proc_delta (void)   { BBlock_delta (a, c, d); }
void proc_succ (void)    { BBlock_succ (c, b); }
void proc_consume (void) { BBlock_consume (nLoops, d); }

int main (int argc, char** argv)
{
    Process *p1, *p2, *p3, *p4;

    if (argc > 1)
    {
        sscanf (argv[1], "%d", &nLoops);
    }
    if (argc > 2)
    {
        sscanf (argv[2], "%d", &nIterations);
    }

    a = CSP_chanAlloc (CSP_ONE2ONE_CHANNEL, 0);
    b = CSP_chanAlloc (CSP_ONE2ONE_CHANNEL, 0);
    c = CSP_chanAlloc (CSP_ONE2ONE_CHANNEL, 0);
    d = CSP_chanAlloc (CSP_ONE2ONE_CHANNEL, 0);

    p1 = ProcAlloc (proc_prefix, 0, 0);
    p2 = ProcAlloc (proc_delta, 0, 0);
    p3 = ProcAlloc (proc_succ, 0, 0);
    p4 = ProcAlloc (proc_consume, 0, 0);

    ProcPar (p1, p2, p3, p4, NULL);

    ProcAllocClean (p1);
    ProcAllocClean (p2);
    ProcAllocClean (p3);
    ProcAllocClean (p4);

    CSP_chanDestroy (a);
    CSP_chanDestroy (b);
    CSP_chanDestroy (c);
    CSP_chanDestroy (d);
    return 0;
}

/*}}}  */
