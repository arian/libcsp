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

@(#) $Id: csp.h,v 1.1.1.1 2001/06/12 13:55:28 rickbeton Exp $
*/
/*}}}  ----------------------------------------------------------------------*/

#ifndef _included_csp_h
#define _included_csp_h

#include <pthread.h>
#include <string.h>

/* CSP_ChanType_t enumerates the allowed channel multiplexing factors */
typedef enum
{
        CSP_ONE2ONE_CHANNEL,
        CSP_ANY2ONE_CHANNEL,
        CSP_ONE2ANY_CHANNEL,
        CSP_ANY2ANY_CHANNEL
} CSP_ChanType_t;


/* CSP_Alt_t is used to set up alternation */
struct CSP_Alt_t
{
    pthread_mutex_t        altMx;
    pthread_cond_t         altSync;
    int                    lastSelected;
    struct CSP_Channel_t * selectedChannel;

};


/* Channel is the main occam channel structure */
struct CSP_Channel_t
{
    int                full;
    pthread_mutex_t    readQueue;
    pthread_mutex_t    writeQueue;
    pthread_mutex_t    mutex;
    pthread_cond_t     fullSync;
    pthread_cond_t     emptySync;
    struct CSP_Alt_t * alt;

    int     hasReadQueue;  /* one2many, many2many */
    int     hasWriteQueue; /* many2one, many2many */
    int     discriminant;  /* run-time type consistency checking */
    int     integer;       /* transferred data */
    void *  buffer;        /* ditto */
    int     guardEnabled;  /* alt guard enable/disable */
};

typedef struct CSP_Alt_t     CSP_Alt_t;

typedef struct CSP_Channel_t Channel;


/*----------------------------------------------------------------------------
   err_abort
   Handles errors. By default, a message is printed and the application
   exits with an error code set.
*/

#ifndef err_abort
#define err_abort(code,message,line) do { \
    char* info = strerror(code); \
    if (info != NULL) \
    { fprintf (stderr, "%s at \"%s\":%d %s\n", message, __FILE__, line, info);}\
    else \
    { fprintf (stderr, "%s at \"%s\":%d %d\n", message, __FILE__, line, code);}\
    abort(); \
} while (0)
#endif

#ifndef errno_abort
#define errno_abort(message) do { \
    char* info = strerror(errno); \
    if (info != NULL) \
    { fprintf (stderr, "%s at \"%s\":%d %s\n", message, __FILE__, __LINE__, info); } \
    else \
    { fprintf (stderr, "%s at \"%s\":%d %d\n", message, __FILE__, __LINE__, errno); } \
    abort(); \
} while (0)
#endif

/*----------------------------------------------------------------------------
   CSP_assert

   Tests a condition. If not met, the program aborts. Use this to confirm
   correct coding.
*/
extern void CSP_assert (int condition, const char* message,
                        const char* file, int line);

/*----------------------------------------------------------------------------
   CSP_chanAlloc

   Allocates a new channel on the heap and initialises the channel.
*/
extern Channel * CSP_chanAlloc (CSP_ChanType_t type, int discriminant);

/*----------------------------------------------------------------------------
   CSP_chanInit

   Initialises the channel pointed to by c.
*/
extern void CSP_chanInit (Channel * c, CSP_ChanType_t type,
                          int discriminant);

/*----------------------------------------------------------------------------
   CSP_chanClose

   Closes the channel pointed to by c. The channel cannot be used
   thereafter. The memory pointed to by c is NOT freed by this call.
*/
extern void CSP_chanClose (Channel * c);

/*----------------------------------------------------------------------------
   CSP_chanDestroy

   Closes the channel pointed to by c. The channel cannot be used
   thereafter. The memory pointed to by c IS freed by this call.
 */
extern void CSP_chanDestroy (Channel * c);

/*----------------------------------------------------------------------------
   CSP_chanReset

   Resets a channel t the value NotProcess_p and returns the process
   descriptor of the channel waiting to communicate, or NotProcess_p.
   If the value returned is NotProcess_p, no process was waiting on
   the channel, and any communication on that channel had completed
   successfully.

   Returns either NotProcess_p or a process descriptor.
*/
extern int CSP_chanReset (Channel * c);

/*----------------------------------------------------------------------------
   CSP_chanOutMove

   Outputs a data structure on the channel chan. The structure is held
   in the pointer pointed to by cp. After calling this function, *cp
   will have been set to NULL, i.e. the writing thread can no longer
   use the data structure.
*/
extern void CSP_chanOutMove (Channel * chan, void ** cp);

/*----------------------------------------------------------------------------
   CSP_chanInMove

   Inputs data structure on the specified channel. The structure is held
   in the pointer pointed to by cp. The original value of this pointer
   (*cp) is lost. After calling this function, the reading thread now
   has the data structure: it has effectively been 'moved' from the
   writer to the reader, although no memory movement actually took place.
*/
extern void CSP_chanInMove (Channel * chan, void ** cp);

/*----------------------------------------------------------------------------
   CSP_chanOutCopy

   Outputs cnt bytes of data on the channel chan. The data is taken from
   the array pointed to by cp.
*/
extern void CSP_chanOutCopy (Channel * chan, void * cp, int cnt);

/*----------------------------------------------------------------------------
   CSP_chanInCopy

   Inputs data on the specified channel and stores them in the array
   pointed to by cp. The data is a copy of that sent by the writing thread.
   If cnt is positive, it specifies how many bytes are expected. If cnt
   is negative, it sets the maximum number of bytes allowed, beyond which
   the message is truncated.

   Return: the number of bytes actually read.
*/
extern int CSP_chanInCopy (Channel * chan, void * cp, int cnt);

/*----------------------------------------------------------------------------
   CSP_chanOutInt

   Outputs a single integer on a channel.
*/
extern void CSP_chanOutInt (Channel * c, int n);

/*----------------------------------------------------------------------------
   CSP_chanInInt

   Inputs a single integer from a channel.
*/
extern int CSP_chanInInt (Channel * c);

/*----------------------------------------------------------------------------
   CSP_chanOutTimeFail

   Used to timeout channel communication. It outputs cnt bytes of data on
   the specified channel from the array pointed to by cp,
   or aborts if the tick-count reaches the specified count. Typically
   is is used to notify delay on a link so that communication can be
   routed elsewhere.

   Returns zero if communication completes, one if timeout occurs before
   the communication completes.
*/
/* not yet implemented
extern int CSP_chanOutTimeFail (Channel * chan, void * cp,
                                int cnt, int time);
*/
/*----------------------------------------------------------------------------
   CSP_chanOutChanFail

   Used to perform reliable channel communication on a link. This function
   outputs cnt bytes of data on the specified channel from the array
   pointed to by cp. It can be aborted by an integer (and only an integer)
   passed on failchan. Typically, failchan is a channel from a process
   that is monitoring the integrity of the link.

   Returns zero if communication completes, one if communication is aborted
   by a message on the failure channel.
*/
/* not yet implemented
extern int CSP_chanOutChanFail (Channel * chan, void * cp,
                                int cnt, Channel * failchan);
*/
/*----------------------------------------------------------------------------
   CSP_chanInTimeFail

   Used to timeout channel communication. It inputs cnt bytes of data on
   the specified channel and stores them in the array pointed to by cp,
   or aborts if the tick-count reaches the specified count. Typically
   is is used to notify delay on a link so that communication can be
   routed elsewhere.

   Returns zero if communication completes, one if timeout occurs before
   the communication completes.
*/
/* not yet implemented
extern int CSP_chanInTimeFail (Channel * chan, void * cp,
                               int cnt, int time);
*/
/*----------------------------------------------------------------------------
   CSP_chanInChanFail

   Used to perform reliable channel communication on a link. This function
   inputs cnt bytes of data on the specified channel into the array
   pointed to by cp. It can be aborted by an integer (and only an integer)
   passed on failchan. Typically, failchan is a channel from a process
   that is monitoring the integrity of the link.

   Returns zero if communication completes, one if communication is aborted
   by a message on the failure channel.
*/
/* not yet implemented
extern int CSP_chanInChanFail (Channel * chan, void * cp,
                               int cnt, Channel * failchan);
*/
/*----------------------------------------------------------------------------
   CSP_altInit

   Initialises the alternative structure before it can be used in
   CSP_priAltSelect.

   CSP_Alt_t * alt  : pointer to an alternative structure
*/
extern void CSP_altInit (CSP_Alt_t * alt);

/*----------------------------------------------------------------------------
   CSP_altClose

   Shuts down the alternative structure after use.

   CSP_Alt_t * alt  : pointer to an alternative structure
*/
extern void CSP_altClose (CSP_Alt_t * alt);

/*----------------------------------------------------------------------------
   CSP_priAltSelect

   Blocks the calling process until one of the channel parameters is
   ready to input. The index returned for the ready channel is an integer
   that indicates the position of the channel in the parameter list. The
   index numbers begin at zero for the first parameter.

   CSP_priAltSelect only returns when a channel is ready to input. It does not
   perform the input operation, which must be done by the code following
   the call to CSP_priAltSelect.

   If more than one channel is ready to communicate, CSP alternation is
   defined to take an arbitrary selection from the ready channels.
   CSP_priAltSelect implements this as a priority list - the channel with
   the lowest index has the highest priority and vice versa.

   The set of channels is specified either as an array, including its size,
   or as a null-terminated list. In the latter case, the number of channels
   nChans should be set to -1.

   CSP_Alt_t * alt : The alternative structure
   Channel * clist : The first in an array of pointers to channels.
   nChans          : the size of clist, or -1
   return          : An index into the parameter list for the ready
                     channel, or -1 if the first element in the array
                     is NULL.
*/
extern int CSP_priAltSelect (CSP_Alt_t * alt, Channel ** clist, int nChans);

/*----------------------------------------------------------------------------
   CSP_priAlt

   As CSP_priAltSelect, but takes any number of pointers to channels.
   Returns -1 if clist is NULL.

   Channel * clist : The first in a NULL-terminated list of pointers to
                     channels.
   return          : An index into the parameter list for the ready channel.
*/
extern int      CSP_priAlt (Channel * clist, ...);

/*--------------------------------------------------------------------------*/

#define NotProcess_p    0x80000000

#endif // _included_channel_h
