Summary: Simula to C translator. Mother of all OO-languages.
Name: cim
Version: 3.21
Release: 1
Source: ftp.ifi.uio.no:/pub/cim/cim-3.21.tar.gz
Copyright: distributable
Group: Development/Languages
Provides: cim
Requires: gcc
Packager: Sverre Hvammen Johansen <cim@ifi.uio.no>

%description
Cim compiles Simula code to C and uses a C compiler like gcc to
compile it further to machine-code. Simula was the first language
with object-oriented features. The Simula language has features for
quasi-paralell execution and a framework for doing simulations.

%prep
%setup -n cim-3.21

%build
CFLAGS=-O2 LDFLAGS=-s ./configure --prefix=/usr --enable-dump=yes
make all

%install
make install
/sbin/ldconfig

%files
/usr/bin/cim
/usr/bin/cim2latex
/usr/bin/cim2ps
/usr/bin/cimindent
/usr/include/cim.h
/usr/lib/libcim-atr.a
/usr/lib/libcim.a
/usr/lib/libcim.la
/usr/lib/libcim.so
/usr/lib/libcim.so.3
/usr/lib/libcim.so.3.0.0
/usr/man/man1/cim.1
/usr/info/cim.info

%doc README
# %doc IAFA-PACKAGE
%doc doc/SIMULA-HISTORY
%doc doc/cim.texi
%doc doc/cim.info

%post
/sbin/ldconfig

%postun
/sbin/ldconfig
