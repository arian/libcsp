//{{{  ---------- CSP For Posix Threads ----------
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

@(#) $Id: csp_alt.c,v 1.1.1.1 2001/06/12 13:55:28 rickbeton Exp $
*/
//}}}  ----------------------------------------------------------------------

#include <stdio.h>
#include <csp.h>
#include <stdarg.h>


//{{{  CSP_altInit
/*----------------------------------------------------------------------------
   CSP_altInit

   Initialises the alternative structure before it can be used in
   CSP_priAltSelect.

   CSP_Alt_t * alt  : pointer to an alternative structure
*/
void CSP_altInit (CSP_Alt_t * alt)
{
    int status;

    status = pthread_mutex_init (&(alt->altMx), NULL);
    if (status != 0) { err_abort (status, "init mutex", __LINE__); }

    status = pthread_cond_init (&(alt->altSync), NULL);
    if (status != 0) { err_abort (status, "init meet", __LINE__); }
}


//}}}  
//{{{  CSP_altClose
/*----------------------------------------------------------------------------
   CSP_altClose

   Shuts down the alternative structure after use.

   CSP_Alt_t * alt  : pointer to an alternative structure
*/
void CSP_altClose (CSP_Alt_t * alt)
{
    int status;

    status = pthread_mutex_destroy (&(alt->altMx));
    if (status != 0) { err_abort (status, "destroy mutex", __LINE__); }

    status = pthread_cond_destroy (&(alt->altSync));
    if (status != 0) { err_abort (status, "destroy meet", __LINE__); }
}


//}}}  
//{{{  CSP_priAlt
/*----------------------------------------------------------------------------
   CSP_priAlt

   Blocks the calling process until one of the channel parameters is
   ready to input. The index returned for the ready channel is an integer
   that indicates the position of the channel in the parameter list. The
   index numbers begin at zero for the first parameter.

   CSP_priAlt only returns when a channel is ready to input. It does not
   perform the input operation, which must be done by the code following
   the call to CSP_priAlt.

   If more than one channel is ready to communicate, CSP alternation is
   defined to take an arbitrary selection from the ready channels.
   CSP_priAlt implements this as a priority list - channel c1 is given
   higher priority than c2, etc.

   Channel * c1 : The first in a NULL-terminated list of pointers to
                  channels.
   return       : An index into the parameter list for the ready channel.
*/
//int CSP_priAlt (Channel * c1, ...)
//{
//    va_list ap;
//    Channel * c;
//    Channel ** list;
//    int nChans = 0, index;
//
//    // count the number of channels
//    va_start(ap, c1);
//    c = c1;
//    while (c != NULL)
//    {
//        nChans++;
//        c = va_arg (ap, Channel*);
//    }
//    va_end(ap);
//
//    if (nChans == 0) { return -1; }
//
//    // allocate some space and copy the channels to the new list
//    list = (Channel**) malloc (sizeof(Channel*) * nChans);
//    if (list == NULL) { return -1; }
//
//    va_start(ap, c1);
//    c = c1;
//    for (index = 0; index < nChans; index++)
//    {
//        list[index] = va_arg (ap, Channel*);
//    }
//    va_end(ap);
//
//    // call main selector
//    index = priAltSelect (list, nChans);
//
//    free (list);
//    return index;
//}
//
//
//}}}  
//{{{  CSP_priAltList
/*----------------------------------------------------------------------------
   CSP_priAltList

   As CSP_priAlt, but takes an array of pointers to channels. Returns -1
   if the clist array is empty.

   Channel ** clist : an array of pointers to channels, terminated by NULL.
   return           : An index into the parameter list for the ready
                      channel, or -1 if the first element in the array
                      is NULL.
*/
//int CSP_priAltList (Channel ** clist)
//{
//    // count the number of channels
//    int nChans = 0;
//    Channel* c = clist[0];
//    while (c != NULL)
//    {
//        nChans++;
//        c++;
//    }
//
//    // call main selector
//    return priAltSelect (clist, nChans);
//}
//
//
//}}}  
