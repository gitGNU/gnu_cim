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

cim_SOURCES = error.c pargen.c name.c dekl.c linegen.c strgen.c \
              extspec.c transcall.c expgen.c lex.c getopt1.c \
              getopt.c parser.y filelist.c newstr.c cimcomp.c \
              mellbuilder.c expbuilder.c sentbuilder.c sentchecker.c \
              expchecker.c computeconst.c sentgen.c obstack.c \
              mapline.c senttrans.c salloc.c passes.c dump.c

cim_LDADD = @ALLOCA@

noinst_HEADERS = cimcomp.h const.h dekl.h gen.h name.h error.h \
                 getopt.h parser.h filelist.h newstr.h lex.h \
                 extspec.h mellbuilder.h builder.h checker.h \
                 obstack.h mapline.h expmacros.h trans.h salloc.h \
                 passes.h dump.h

AM_YFLAGS= -d

# Variables controlling compilation of the generated C-code
SCC = $(CC)
SCFLAGS = $(CFLAGS)
SLDFLAGS = $(LDFLAGS)
SLIBS = $(LIBS)

INCLUDES = -I$(top_srcdir)

cimcomp.o: cimcomp.c Makefile
	$(COMPILE) '-DSCC="$(SCC)"' '-DSCFLAGS="$(SCFLAGS)"' '-DSLDFLAGS="$(SLDFLAGS)"' '-DSLIBS="$(SLIBS)"' '-DLIBDIR="$(libdir)"' '-DINCLUDEDIR="$(includedir)"' -c $(srcdir)/cimcomp.c
