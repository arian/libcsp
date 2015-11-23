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

This program starts a process to put out a simple message to stdout
*/

#include <stdio.h>
#include <stdlib.h>
#include <process.h>

/* The process is declared as a function */
void hello_proc (Process * p)
{
  p = p;
  printf ("Hello world.\n");
  return;
}

int main (void)
{
  Process * hello;

  /* Set up the new process */
  hello = ProcAlloc (hello_proc, 0, 0);
  if (hello == NULL)
  {
    printf ("Could not allocate process.\n");
    exit (EXIT_FAILURE);
  }

  /* Start it running */
  ProcRun (hello);

  /* Wait for the processes to join up again */
  ProcJoin (hello, NULL);

  /* Clean up all the space it used */
  ProcAllocClean (hello);

  exit (EXIT_SUCCESS);
}
