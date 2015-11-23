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

$Id: mutextest.c,v 1.1 2001/06/12 14:33:39 rickbeton Exp $
//
// This test merely plays with a mutex.
// It verifies that two integers can be changed atomically
// by two different threads.
//
// Note that the application runs indefinitely, unless a non-atomic
// change has occured.
*/

#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct
{
    pthread_mutex_t mutex;
    int a;
    int b;

} LockThingy;

LockThingy lockThingy;
double delay = 10.0;

void hello (void)
{
    int status;
    unsigned int count = 0;
    while (1)
    {
        // 0 to 2 second sleep
        usleep (drand48() * delay);

        status = pthread_mutex_lock (&(lockThingy.mutex));
        if (status != 0) { err_abort (status, "lock mutex", __LINE__); }

        if (count > 100)
        {
            printf ("hello... %d, %d\n", lockThingy.a, lockThingy.b);
            count = 0;
        }
        if (lockThingy.a != lockThingy.b)
        {
            printf ("BANG!\n");
            exit(1);
        }
        lockThingy.a = lockThingy.b = 11;

        status = pthread_mutex_unlock (&(lockThingy.mutex));
        if (status != 0) { err_abort (status, "lock mutex", __LINE__); }
        count++;
    }
}

void world (void)
{
    int status;
    while (1)
    {
        // 0 to 2 second sleep
        usleep (drand48() * delay);

        status = pthread_mutex_lock (&(lockThingy.mutex));
        if (status != 0) { err_abort (status, "lock mutex", __LINE__); }

        //printf ("world... %d, %d\n", lockThingy.a, lockThingy.b);
        if (lockThingy.a != lockThingy.b)
        {
            printf ("BANG!\n");
            exit(1);
        }

        lockThingy.a = lockThingy.b = 55;

        status = pthread_mutex_unlock (&(lockThingy.mutex));
        if (status != 0) { err_abort (status, "lock mutex", __LINE__); }
    }
}

int main (int argc, char** argv)
{
    Process * a, * b;
    int status;

    status = pthread_mutex_init (&(lockThingy.mutex), NULL);
    if (status != 0) { err_abort (status, "init mutex", __LINE__); }
    lockThingy.a = lockThingy.b = 0;

    a = ProcAlloc (hello, 0, 0);
    b = ProcAlloc (world, 0, 0);
    ProcPar (a, b, NULL);
    ProcAllocClean (a);
    ProcAllocClean (b);

    status = pthread_mutex_destroy (&(lockThingy.mutex));
    if (status != 0) { err_abort (status, "destroy mutex", __LINE__); }
    return 0;
}
