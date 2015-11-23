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

$Id: chantest3.c,v 1.1 2001/06/12 14:33:39 rickbeton Exp $
*/

#include <csp.h>
#include <channel.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>

Channel * chan;
char* message = "Twas brillig, and the slithy toves\n\
Did gyre and gimble in the wabe:\n\
All mimsy were the borogoves,\n\
And the mome raths outgrabe.\n\
\n\
\"Beware the Jabberwock, my son!\n\
 The jaws that bite, the claws that catch!\n\
 Beware the Jubjub bird, and shun\n\
 The frumious Bandersnatch!\"\n\
\n\
He took his vorpal sword in hand:\n\
Long time the manxome foe he sought --\n\
So rested he by the Tumtum tree,\n\
And stood awhile in thought.\n\
\n\
And, as in uffish thought he stood,\n\
The Jabberwock, with eyes of flame,\n\
Came whiffling through the tulgey wood,\n\
And burbled as it came!\n\
\n\
One, two! One, two! And through and through\n\
The vorpal blade went snicker-snack!\n\
He left it dead, and with its head\n\
He went galumphing back.\n\
\n\
\"And, has thou slain the Jabberwock?\n\
 Come to my arms, my beamish boy!\n\
 O frabjous day! Callooh! Callay!\"\n\
 He chortled in his joy.\n\
\n\
`Twas brillig, and the slithy toves\n\
Did gyre and gimble in the wabe;\n\
All mimsy were the borogoves,\n\
And the mome raths outgrabe.\n";

int repeats = 1;

void hello (void)
{
    int i,j;
    for (j = 0; j < repeats; j++)
    {
        for (i = 0; message[i] != NULL; i++)
        {
            CSP_chanOutInt (chan, message[i]);
        }
        CSP_chanOutInt (chan, 0xFF);
    }
}

void world (void)
{
    int j;
    unsigned char c;
    for (j = 1; j < repeats; j++)
    {
        c = CSP_chanInInt (chan);
        while (c != 0xFF)
        {
            c = CSP_chanInInt (chan);
        }
        putchar ('.'); fflush (stdout);
    }
    putchar ('\n');

    c = CSP_chanInInt (chan);
    while (c != 0xFF)
    {
        putchar (c);
        fflush (stdout);
        c = CSP_chanInInt (chan);
    }
}

int main (int argc, char** argv)
{
    Process * a, * b;
    printf ("+--\n");
    printf ("| This test repeatedly sends characters one by one along a channel.\n");
    printf ("| The receiving thread puts the characters to stdout.\n");
    printf ("| Usage: %s [<n-repeats>]\n", argv[0]);
    printf ("+--\n");

    if (argc > 1) { repeats = atoi (argv[1]); }
    chan = ChanAlloc();
    a = ProcAlloc (hello, 0, 0);
    b = ProcAlloc (world, 0, 0);
    ProcPar (a, b, NULL);
    ProcAllocClean (a);
    ProcAllocClean (b);
    ChanDestroy (chan);
    return 0;
}
