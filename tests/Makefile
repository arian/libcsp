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
# $Id: Makefile,v 1.1 2001/06/12 14:33:39 rickbeton Exp $
#}}}  ----------------------------------------------------------------------

SRCDIR = ../src
#DEBUG = -g
CC = gcc $(DEBUG) -I$(SRCDIR) -Wall -pthread
LIBFILES = $(SRCDIR)/libcsp$(DEBUG).a
LIBOPT = -L$(SRCDIR) -lcsp$(DEBUG)
TARGETS = hello hello2proc mutextest \
	  chantest1 chantest2 chantest3 \
	  mchantest1 alttest1 alttest2 pingpong pc

default: .depends donelibs $(TARGETS)

hello: .depends hello.c $(LIBFILES)
	$(CC) hello.c -o $@ $(LIBOPT)

hello2proc: .depends hello2proc.c $(LIBFILES)
	$(CC) hello2proc.c -o $@ $(LIBOPT)

mutextest: .depends mutextest.c $(LIBFILES)
	$(CC) mutextest.c -o $@ $(LIBOPT)

chantest1: .depends chantest1.c $(LIBFILES)
	$(CC) chantest1.c -o $@ $(LIBOPT)

chantest2: .depends chantest2.c $(LIBFILES)
	$(CC) chantest2.c -o $@ $(LIBOPT)

chantest3: .depends chantest3.c $(LIBFILES)
	$(CC) chantest3.c -o $@ $(LIBOPT)

mchantest1: .depends mchantest1.c $(LIBFILES)
	$(CC) mchantest1.c -o $@ $(LIBOPT)

alttest1: .depends alttest1.c $(LIBFILES)
	$(CC) alttest1.c -o $@ $(LIBOPT)

alttest2: .depends alttest2.c $(LIBFILES)
	$(CC) alttest2.c -o $@ $(LIBOPT)

pingpong: .depends pingpong.c $(LIBFILES)
	$(CC) pingpong.c -o $@ $(LIBOPT)

pc: .depends pc.c $(LIBFILES)
	$(CC) pc.c -o $@ $(LIBOPT)

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

