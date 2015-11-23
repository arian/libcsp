#{{{  ---------- CSP For Posix Threads ----------
#   Copyright (C) 2000 Richard Beton
#
#   This library is free software; you can redistribute it and/or
#   modify it under the terms of the GNU Lesser General Public
#   License as published by the Free Software Foundation; either
#   version 2.1 of the License, or (at your option) any later version.
#
#   This library is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#   Lesser General Public License for more details.
#
#   You should have received a copy of the GNU Lesser General Public
#   License along with this library; if not, write to the Free Software
#   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#   To contact the author refer to http://www.beton.freeserve.co.uk/libcsp/
#   or email richard.beton@physics.org.
#
# $Id: Makefile,v 1.1.1.1 2001/06/12 13:55:28 rickbeton Exp $
#}}}  ----------------------------------------------------------------------

#DEBUG = -g
CC = gcc $(DEBUG) -I. -Wall -D_REENTRANT
CSPOBJS = csp_channel.o csp_channel2.o csp_alt.o
#CSPOBJS = csp_channel.o csp_channel2.o csp_process.o csp_semaphor.o
IMSOBJS = process.o semaphor.o channel.o

#-------------------------------------------------------------------------------

#.c.o:
#       $(CC) -c $< -o $@

default: libcsp$(DEBUG).a libimscsp$(DEBUG).a

libcsp$(DEBUG).a: .depends $(CSPOBJS)
	ar cq $@ $(CSPOBJS)

libimscsp$(DEBUG).a: .depends $(IMSOBJS)
	ar cq $@ $(IMSOBJS)

clean:
	rm -f *.o *.a *.?~ .depends $(TESTS)
	rm -f depends.mk; touch depends.mk
	cd ../tests; $(MAKE) clean

test:
	cd ../tests; $(MAKE)

tarball:
	$(MAKE) clean
	cd ../..; tar cvf libcsp.tar csp-posix-threads
	gzip ../../libcsp.tar

#-------------------------------------------------------------------------------
# Sub rules
#-------------------------------------------------------------------------------

.depends:
	$(MAKE) depends
	@touch $@

#-------------------------------------------------------------------------------
# Detailed dependencies
#-------------------------------------------------------------------------------

# to make the dependency file up to date do 'touch depends.mk; make depends'
depends:
	$(CC) -M $(CSPOBJS:.o=.c) $(IMSOBJS:.o=.c) >depends.mk

include depends.mk

