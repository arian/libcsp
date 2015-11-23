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

$Id: chantest2.c,v 1.1 2001/06/12 14:33:39 rickbeton Exp $
*/

#include <csp.h>
#include <channel.h>
#include <process.h>
#include <stdio.h>

Channel * chan;
char* message = "hello world \260\n";

void hello (void)
{
    int i;
    for (i = 0; message[i] != NULL; i++)
    {
        CSP_chanOutInt (chan, message[i]);
    }
    CSP_chanOutInt (chan, 0xFF);
    printf ("(hello done)\n");
}

void world (void)
{
    unsigned char c = CSP_chanInInt (chan);
    while (c != 0xFF)
    {
        //printf ("%d %c\n", c, c);
        putchar (c);
        fflush (stdout);
        c = CSP_chanInInt (chan);
    }
    printf ("%d\n", c);
    printf ("(world done)\n");
}

int main (int argc, char** argv)
{
    Process * a, * b;

    printf ("+--\n");
    printf ("| This test has two threads and passes a stream of integers between them\n");
    printf ("| using a channel.\n");
    printf ("+--\n");

    chan = ChanAlloc();
    a = ProcAlloc (hello, 0, 0);
    b = ProcAlloc (world, 0, 0);
    ProcPar (a, b, NULL);
    ProcAllocClean (a);
    ProcAllocClean (b);
    ChanDestroy (chan);
    return 0;
}
