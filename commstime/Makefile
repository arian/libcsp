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
# $Id: Makefile,v 1.1.1.1 2001/06/12 13:55:27 rickbeton Exp $
#}}}  ----------------------------------------------------------------------

SRCDIR = ../src
#DEBUG = -g
CC = gcc $(DEBUG) -I$(SRCDIR) -Wall -pthread -lrt -D_REENTRANT
LIBFILES = $(SRCDIR)/libcsp$(DEBUG).a $(SRCDIR)/libimscsp$(DEBUG).a
LIBOPT = -L$(SRCDIR) -limscsp$(DEBUG) -lcsp$(DEBUG)
TARGETS = commstime

default: .depends donelibs $(TARGETS) occam.commstime

commstime: .depends commstime.c $(LIBFILES)
	$(CC) commstime.c -o $@ $(LIBOPT)

# you can compile the occam version by installing Kroc
# and defining KROC_HOME
occam.commstime: commstime.occ
	if [ "$$KROC_HOME" != "" ]; then $(MAKE) .occ; fi
.occ:
	kroc -I$$KROC_HOME/course/lib -L$$KROC_HOME/course/lib \
	     commstime.occ -lcourse -o occam.commstime

donelibs:
	cd $(SRCDIR); $(MAKE)

clean:
	rm -f *.o $(TARGETS) .depends
	rm -f depends.mk; touch depends.mk

veryclean: clean
	rm -f *.?~
	cd $(SRCDIR); $(MAKE) clean

.depends:
	$(MAKE) depends
	@touch $@

#-------------------------------------------------------------------------------
# Detailed dependencies
#-------------------------------------------------------------------------------

# to make the dependency file up to date do 'touch depends.mk; make depends'
depends:
	$(CC) -M $(TARGETS:=.c) >depends.mk

include depends.mk

