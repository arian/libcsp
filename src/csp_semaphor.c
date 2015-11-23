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

@(#) $Id: csp_semaphor.c,v 1.1.1.1 2001/06/12 13:55:28 rickbeton Exp $
*/
/*}}}  ----------------------------------------------------------------------*/

#include <csp.h>
#include <stdio.h>

void SemWait (Semaphore * s)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

void SemSignal (Semaphore * s)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

void SemInit (Semaphore * s, int initvalue)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

Semaphore *SemAlloc (int initvalue)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}

