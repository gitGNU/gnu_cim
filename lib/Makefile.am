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

# Ranlib should not be run when creating libcim-atr.a
RANLIB=:

CIM = ../src/cim
CIMFLAGS = -H -L -L$(srcdir) -I$(srcdir) -S

lib_LIBRARIES = libcim-atr.a
lib_LTLIBRARIES = libcim.la

libcim_la_LDFLAGS = -version-info 1:0:0

libcim_la_SOURCES = sysin.c sysout.c syserr.c copytexttoc.c addroffirstchar.c \
              addroffirstelem.c copytextarrtoc.c copyarrtoc.c mod.c rem.c \
              signr.c signdr.c signi.c signdi.c signdx.c entier.c intrea.c \
              powii.c powri.c pow.c addepsilon.c subepsilon.c absr.c absi.c \
              sqrt.c sin.c cos.c tan.c cotan.c arcsin.c arccos.c arctan.c \
              arctan2.c sinh.c cosh.c tanh.c ln.c log10.c exp.c mint.c \
              minc.c minr.c mini.c maxt.c maxc.c maxr.c maxi.c simulaid.c \
              datetime.c cputime.c clocktime.c lowerbound.c upperbound.c \
              draw.c randint.c uniform.c normal.c negexp.c poisson.c \
              erlang.c discrete.c linear.c histd.c histo.c terror.c \
              filename.c isopen.c setaccess.c leftshift.c field.c setpos.c \
              pos.c more.c length.c dlocation.c dendfile.c dlocked.c \
              dcheckpoint.c dunlock.c dlock.c dopen.c dclose.c dlocate.c \
              dlastloc.c dmaxloc.c dinimage.c doutimage.c ddeleteimage.c \
              iendfile.c iopen.c iclose.c iinimage.c iinrecord.c iinchar.c \
              ilastitem.c iintext.c iinint.c iinreal.c iinfrac.c oopen.c \
              oclose.c ooutimage.c ooutrecord.c obreakoutimage.c ooutchar.c \
              oouttext.c ooutint.c ooutfix.c ooutreal.c ooutfrac.c pline.c \
              ppage.c popen.c pclose.c plinesperpage.c pspacing.c peject.c \
              poutimage.c poutrecord.c bytesize.c ibendfile.c ibopen.c \
              ibclose.c ibinbyte.c ibintext.c obopen.c obclose.c \
              oboutbyte.c obouttext.c dbendfile.c dblocation.c dbmaxloc.c \
              dblocked.c dbunlock.c dblock.c dbopen.c dbclose.c dblastloc.c \
              dblocate.c dbinbyte.c dboutbyte.c dbintext.c dbouttext.c \
              tsglob.c start.c printfilline.c error.c warning.c trace.c \
              systemerror.c initier.c ss.c rs.c cp.c cpp.c cpb.c ccb.c ep.c \
              epp.c oa.c cprb.c cprbb.c b.c be.c resume.c exchange.c \
              detach.c call.c goto.c enddecl.c inner.c endclass.c ca.c in.c \
              rin.c is.c gbc.c ct.c eth.c getav.c getcbv.c getrv.c getta.c \
              gettv.c getproc.c getlab.c geta.c getsa.c dump.c argc.c \
              argv.c return.c tconstant.c tstart.c tlength.c tmain.c \
              tpos.c tmore.c tsub.c tstrip.c tsetpos.c tgetchar.c tgetint.c \
              tgetreal.c tgetfrac.c tputchar.c tputint.c tputfix.c \
              tputreal.c tputfrac.c copy.c conc.c blanks.c textvalassign.c \
              textassign.c eqrtext.c eqtext.c lttext.c letext.c char.c \
              isochar.c rank.c isorank.c digit.c letter.c lowten.c \
              decimalmark.c upcase.c lowcase.c simfile.c simenvir.c \
              xmalloc.c simset.c simulation.c

BUILT_SOURCES = simset.c simulation.c

CLEANFILES = simset.shl simulation.shl simset.mif simulation.mif
MAINTAINER_CLEANFILES = simset.c simset.h simset.atr \
                        simulation.c simulation.h simulation.atr

maintainer-clean-libLIBRARIES:
	test -z "$(lib_LIBRARIES)" || rm -f $(lib_LIBRARIES)

clean-libLIBRARIES:

libcim_atr_a_SOURCES = 

libcim_atr_a_LIBADD = $(srcdir)/simenvir.atr $(srcdir)/simfile.atr \
                      $(srcdir)/simset.sim $(srcdir)/simulation.sim \
                      $(srcdir)/simset.atr $(srcdir)/simulation.atr

noinst_HEADERS = epsilon.h file.h random.h text.h trigno.h align.h \
                 simset.h simulation.h

include_HEADERS = cim.h

INCLUDES = -I$(top_srcdir)

EXTRA_DIST = simset.sim simulation.sim simfile.atr simenvir.atr \
             simulation.atr simset.atr libcim-atr.a

SUFFIXES = .sim .atr

clean-libLIBRARIES:

maintainer-clean-libLIBRARIES:
	test -z "$(lib_LIBRARIES)" || rm -f $(lib_LIBRARIES)

simulation.c simulation.h simulation.atr: simulation.sim simset.atr
	cd $(srcdir); $(CIM) $(CIMFLAGS) $(srcdir)/simulation.sim

simset.c simset.h simset.atr: simset.sim cim.h simfile.atr simenvir.atr
	cd $(srcdir); $(CIM) $(CIMFLAGS) $(srcdir)/simset.sim

