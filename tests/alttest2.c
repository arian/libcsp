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

$Id: alttest2.c,v 1.1 2001/06/12 14:33:39 rickbeton Exp $
*/

#include <process.h>
#include <csp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define Nloops 10
#define Nchans 5

Channel * clist[Nchans];
int delay = 0;

void hello (const char* msg, Channel * c)
{
    int i;
    char text[100];
    for (i = 0; i < Nloops; i++)
    {
        sprintf (text, msg, i);
        CSP_chanOutCopy (c, text, strlen(text)+1);
        usleep (drand48() * delay);
    }
}

void hello0 (void)
{
    hello ("hello0 %d.\n", clist[0]);
}

void hello1 (void)
{
    hello ("hello1 %d.\n", clist[1]);
}

void hello2 (void)
{
    hello ("hello2 %d.\n", clist[2]);
}

void hello3 (void)
{
    hello ("hello3 %d.\n", clist[3]);
}

void hello4 (void)
{
    hello ("hello4 %d.\n", clist[4]);
}

void world (void)
{
    int i, selected;
    char text[100];
    CSP_Alt_t alt;

    CSP_altInit (&alt);
    for (i = 0; i < (Nchans * Nloops); i++)
    {
        selected = CSP_priAltSelect (&alt, clist, Nchans);
        if (selected >= 0)
        {
            CSP_chanInCopy (clist[selected], text, -100);
            printf (text);
        }
        else
        {
            printf ("-1\n");
        }
    }
    CSP_altClose (&alt);
}

int main (int argc, char** argv)
{
    int i;
    Process *w0, *w1, *w2, *w3, *w4, *b;

    printf ("+--\n");
    printf ("| This test has five source threads and one mux thread.\n");
    printf ("| The source threads loop %d times to output a string on a channel.\n", Nloops);
    printf ("| The mux thread prints out all these strings.\n");
    printf ("| Usage: %s [<delay>]\n", argv[0]);
    printf ("+--\n");

    if (argc > 1) { delay = atoi (argv[1]); }
    for (i = 0; i < Nchans; i++)
    {
        clist[i] = CSP_chanAlloc (CSP_ONE2ONE_CHANNEL, 0);
    }

    w0 = ProcAlloc (hello0, 0, 0);
    w1 = ProcAlloc (hello1, 0, 0);
    w2 = ProcAlloc (hello2, 0, 0);
    w3 = ProcAlloc (hello3, 0, 0);
    w4 = ProcAlloc (hello4, 0, 0);
    b  = ProcAlloc (world,  0, 0);
    
    ProcPar (w0, w1, w2, w3, w4, b, NULL);
    
    ProcAllocClean (w0);
    ProcAllocClean (w1);
    ProcAllocClean (w2);
    ProcAllocClean (w3);
    ProcAllocClean (w4);
    ProcAllocClean (b);

    for (i = 0; i < Nchans; i++)
    {
        CSP_chanDestroy (clist[i]);
    }
    return 0;
}
