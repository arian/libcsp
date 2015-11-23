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

@(#) $Id: channel.c,v 1.1.1.1 2001/06/12 13:55:27 rickbeton Exp $
*/
/*}}}  ----------------------------------------------------------------------*/

#include <csp.h>
#include <channel.h>
#include <stdio.h>
#include <stdlib.h>

void ChanOut (Channel * chan, void * cp, int cnt)
{
    CSP_chanOutCopy (chan, cp, cnt);
}

void ChanIn (Channel * chan, void * cp, int cnt)
{
    CSP_chanInCopy (chan, cp, cnt);
}

Channel *ChanAlloc (void)
{
    Channel * ch = (Channel*) malloc (sizeof(Channel));
    if (ch != NULL)
    {
        CSP_chanInit (ch, CSP_ONE2ONE_CHANNEL, 0);
    }
    return ch;
}

void ChanInit (Channel * c)
{
    CSP_chanInit (c, CSP_ONE2ONE_CHANNEL, 0);
}

void ChanDestroy (Channel * c)
{
    CSP_chanClose (c);
    free (c);
}

int ChanReset (Channel * c)
{
    return CSP_chanReset (c);
}

void ChanOutChar (Channel * c, unsigned char ch)
{
    CSP_chanOutCopy (c, &ch, sizeof (unsigned char));
}

void ChanOutInt (Channel * c, int n)
{
    CSP_chanOutCopy (c, &n, sizeof (int));
}

unsigned char ChanInChar (Channel * c)
{
    unsigned char ch;
    CSP_chanInCopy (c, &ch, sizeof (unsigned char));
    return ch;
}

int ChanInInt (Channel * c)
{
    int integer = 0;
    CSP_chanInCopy (c, &integer, sizeof (int));
    return integer;
}

int ChanOutTimeFail (Channel * chan, void * cp,
                     int cnt, int time)
{
    return CSP_chanOutTimeFail (chan, cp, cnt, time);
}

int ChanOutChanFail (Channel * chan, void * cp,
                     int cnt, Channel * failchan)
{
    return CSP_chanOutChanFail (chan, cp, cnt, failchan);
}

int ChanInTimeFail (Channel * chan, void * cp,
                    int cnt, int time)
{
    return CSP_chanInTimeFail (chan, cp, cnt, time);
}

int ChanInChanFail (Channel * chan, void * cp,
                    int cnt, Channel * failchan)
{
    return CSP_chanInChanFail (chan, cp, cnt, failchan);
}

