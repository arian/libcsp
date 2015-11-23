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
# $Id: Makefile,v 1.2 2001/06/12 14:33:39 rickbeton Exp $
#}}}  ----------------------------------------------------------------------

#DEBUG = -g

#-------------------------------------------------------------------------------

default:
	@touch src/depends.mk tests/depends.mk commstime/depends.mk
	cd src;   $(MAKE) DEBUG=$(DEBUG)
	@echo
	cd tests; $(MAKE) DEBUG=$(DEBUG)
	@echo
	cd commstime; $(MAKE) DEBUG=$(DEBUG)

clean:
	cd src;   $(MAKE) clean
	@echo
	cd tests; $(MAKE) clean
	@echo
	cd commstime; $(MAKE) clean

# tarball: only works with Gnu tar (usually in /usr/local/bin)
tarball:
	$(MAKE) clean
	rm -f `date '+libcsp-%Y-%m-%d.tar.gz'`
	cd ..; tar cvf `date '+libcsp/libcsp-%Y-%m-%d.tar'` \
	       --exclude \*/CVS --exclude \*.pps --exclude \*.tar.gz \
	       libcsp
	gzip `date '+libcsp-%Y-%m-%d.tar'`

