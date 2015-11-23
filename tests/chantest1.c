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

$Id: chantest1.c,v 1.1 2001/06/12 14:33:39 rickbeton Exp $
*/

#include <process.h>
#include <channel.h>
#include <stdio.h>

Channel * chan;

void hello (void)
{
    printf ("hello\n");
    ChanOutInt (chan, 2);
}

void world (void)
{
    int i = ChanInInt (chan);
    printf ("world %d\n", i);
}

int main (int argc, char** argv)
{
    Process * a, * b;

    printf ("+--\n");
    printf ("| This test has two threads and passes an integer between them using a channel.\n");
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
