## Process this file with automake to create Makefile.in
## $Id: $

# Copyright (C) 1997 Sverre Hvammen Johansen,
# Department of Informatics, University of Oslo.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; version 2.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

CIM=$(top_builddir)/src/cim
CIMFLAGS=-L -L$(top_srcdir)/lib -I$(top_srcdir)/lib 

TESTS = test

EXTRA_DIST = test.sim

SUFFIXES = .sim

test: stamp

stamp: test.sim
	$(CIM) $(CIMFLAGS) -c -e -t $(srcdir)/test.sim
	$(LIBTOOL) --tag=CC --mode=link $(CC) $(CFLAGS) -o test test.o ../lib/libcim.la -lm

clean-local:
	-rm test test.o test.c test.shl
