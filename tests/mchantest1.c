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

$Id: mchantest1.c,v 1.1 2001/06/12 14:33:39 rickbeton Exp $
*/

#include <process.h>
#include <csp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const int N = 10;
Channel * chan;

void hello (const char* msg)
{
    int i;
    char text[100];
    for (i = 0; i < N; i++)
    {
        sprintf (text, msg, i);
        CSP_chanOutCopy (chan, text, strlen(text)+1);
        usleep (drand48() * 100.0);
    }
}

void hello1 (void)
{
    hello ("hello1 %d.\n");
}

void hello2 (void)
{
    hello ("hello2 %d.\n");
}

void world (void)
{
    int i;
    char text[100];
    for (i = 0; i < (2*N); i++)
    {
        CSP_chanInCopy (chan, text, -100);
        printf (text);
    }
}

int main (int argc, char** argv)
{
    Process * w1, * w2, * b;

    printf ("+--\n");
    printf ("| This test has two writer threads and a reader thread on a common\n");
    printf ("| (shared) channel. A message is send %d times by each.\n", N);
    printf ("+--\n");

    chan = CSP_chanAlloc (CSP_ANY2ONE_CHANNEL, 0);
    w1 = ProcAlloc (hello1, 0, 0);
    w2 = ProcAlloc (hello2, 0, 0);
    b = ProcAlloc (world, 0, 0);
    ProcPar (w1, w2, b, NULL);
    ProcAllocClean (w1);
    ProcAllocClean (w2);
    ProcAllocClean (b);
    CSP_chanDestroy (chan);
    return 0;
}
