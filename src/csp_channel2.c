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

@(#) $Id: csp_channel2.c,v 1.1.1.1 2001/06/12 13:55:28 rickbeton Exp $
*/
//}}}  ----------------------------------------------------------------------

#include <stdio.h>
#include <csp.h>


//{{{  CSP_assert
/*----------------------------------------------------------------------------
   CSP_assert

   Tests a condition. If not met, the program aborts. Use this to confirm
   correct coding.
*/
void CSP_assert (int condition, const char* message, const char* file, int line)
{
    if (!condition)
    {
        fprintf (stderr, "CSP_assert failed: %s (%s:%d)\n", message, file, line);
        abort();
    }
}


//}}}
//{{{  CSP_chanAlloc
/*----------------------------------------------------------------------------
 */
Channel* CSP_chanAlloc (CSP_ChanType_t type, int discriminant)
{
    Channel* ch = (Channel*) malloc (sizeof(Channel));
    if (ch != NULL)
    {
        CSP_chanInit (ch, type, discriminant);
    }
    return ch;
}


//}}}
//{{{  CSP_chanInit
/*----------------------------------------------------------------------------
 */
void CSP_chanInit (Channel * c, CSP_ChanType_t type, int discriminant)
{
    int status;

    switch (type)
    {
        case CSP_ONE2ONE_CHANNEL:
            c->hasReadQueue  = 0;
            c->hasWriteQueue = 0;
            break;
        case CSP_ANY2ONE_CHANNEL:
            c->hasReadQueue  = 0;
            c->hasWriteQueue = 1;
            break;
        case CSP_ONE2ANY_CHANNEL:
            c->hasReadQueue  = 1;
            c->hasWriteQueue = 0;
            break;
        case CSP_ANY2ANY_CHANNEL:
            c->hasReadQueue  = 1;
            c->hasWriteQueue = 1;
            break;
        default:
            CSP_assert (0, "illegal parameter", __FILE__, __LINE__);
    }

    c->discriminant  = discriminant;
    c->full          = 0;
    c->alt           = NULL;
    c->guardEnabled  = 1;

    status = pthread_mutex_init (&(c->mutex), NULL);
    if (status != 0) { err_abort (status, "init mutex", __LINE__); }

    status = pthread_cond_init (&(c->fullSync), NULL);
    if (status != 0) { err_abort (status, "init full", __LINE__); }

    status = pthread_cond_init (&(c->emptySync), NULL);
    if (status != 0) { err_abort (status, "init empty", __LINE__); }

    if (c->hasWriteQueue)
    {
        status = pthread_mutex_init (&(c->writeQueue), NULL);
        if (status != 0) { err_abort (status, "init mutex", __LINE__); }
    }

    if (c->hasReadQueue)
    {
        status = pthread_mutex_init (&(c->readQueue), NULL);
        if (status != 0) { err_abort (status, "init mutex", __LINE__); }
    }
}


//}}}
//{{{  CSP_chanClose
/*----------------------------------------------------------------------------
 */
void CSP_chanClose (Channel * c)
{
    int status;

    status = pthread_mutex_destroy (&(c->mutex));
    if (status != 0) { err_abort (status, "destroy mutex", __LINE__); }

    status = pthread_cond_destroy (&(c->fullSync));
    if (status != 0) { err_abort (status, "destroy full", __LINE__); }

    status = pthread_cond_destroy (&(c->emptySync));
    if (status != 0) { err_abort (status, "destroy empty", __LINE__); }

    if (c->hasWriteQueue)
    {
        status = pthread_mutex_destroy (&(c->writeQueue));
        if (status != 0) { err_abort (status, "destroy mutex", __LINE__); }
    }

    if (c->hasReadQueue)
    {
        status = pthread_mutex_destroy (&(c->readQueue));
        if (status != 0) { err_abort (status, "destroy mutex", __LINE__); }
    }
}


//}}}
//{{{  CSP_chanDestroy
/*----------------------------------------------------------------------------
 */
void CSP_chanDestroy (Channel * c)
{
    CSP_chanClose (c);
    free (c);
}


//}}}
//{{{  CSP_chanReset
/*----------------------------------------------------------------------------
 */
int CSP_chanReset (Channel * c)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}


//}}}
//{{{  CSP_chan...Int
/*----------------------------------------------------------------------------
 */
void CSP_chanOutInt (Channel * c, int n)
{
    CSP_chanOutCopy (c, &n, sizeof (int));
}


/*----------------------------------------------------------------------------
 */
int CSP_chanInInt (Channel * c)
{
    int integer = 0;
    CSP_chanInCopy (c, &integer, sizeof (int));
    return integer;
}


//}}}
//{{{  CSP_chan...TimeFail
/*----------------------------------------------------------------------------
 */
int CSP_chanOutTimeFail (Channel * c, void * cp,
                         int cnt, int time)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}


/*----------------------------------------------------------------------------
 */
int CSP_chanInTimeFail (Channel * c, void * cp,
                        int cnt, int time)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}


//}}}
//{{{  CSP_chan...ChanFail
/*----------------------------------------------------------------------------
 */
int CSP_chanOutChanFail (Channel * c, void * cp,
                         int cnt, Channel * failchan)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}


/*----------------------------------------------------------------------------
 */
int CSP_chanInChanFail (Channel * c, void * cp,
                        int cnt, Channel * failchan)
{
    fprintf (stderr, "Not yet implemented. %s %d\n", __FILE__, __LINE__);
    exit (-1);
}


//}}}
