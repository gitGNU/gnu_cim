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

bin_PROGRAMS = cim

cim_SOURCES = feil.c genpar.c navn.c dekl.c gen.c genstr.c \
              extspec.c genexp.c lex.c sjekker.c getopt1.c \
              getopt.c parser.y filelist.c newstr.c cimcomp.c 

cim_LDADD = @ALLOCA@

noinst_HEADERS = cimcomp.h const.h dekl.h gen.h mell.h navn.h feil.h \
                 sjekker.h getopt.h parser.h filelist.h newstr.h lex.h \
                 extspec.h

# Variables controlling compilation of the generated C-code
SCC = $(CC)
SCFLAGS = $(CFLAGS)
SLDFLAGS = $(LDFLAGS)
SLIBS = $(LIBS)

INCLUDES = -I$(top_srcdir)

cimcomp.o: cimcomp.c Makefile
	$(COMPILE) '-DSCC="$(SCC)"' '-DSCFLAGS="$(SCFLAGS)"' '-DSLDFLAGS="$(SLDFLAGS)"' '-DSLIBS="$(SLIBS)"' '-DLIBDIR="$(libdir)"' '-DINCLUDEDIR="$(includedir)"' -c $(srcdir)/cimcomp.c
