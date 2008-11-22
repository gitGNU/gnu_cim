Summary: Simula to C translator. Mother of all OO-languages.
Name: cim
Version: 3.37
Release: 1
Source: ftp.ifi.uio.no:/pub/cim/cim-3.37.tar.gz
Copyright: GPL
Group: Development/Languages
Provides: cim
Requires: gcc
Packager: Sverre Hvammen Johansen <cim@ifi.uio.no>
BuildRoot: /tmp/cim-root

%description
Cim compiles Simula code to C and uses a C compiler like gcc to
compile it further to machine-code. Simula was the first language
with object-oriented features. The Simula language has features for
quasi-paralell execution and a framework for doing simulations.

%prep
%setup -n cim-3.37

%build
CFLAGS=-O2 LDFLAGS=-s ./configure --prefix=$RPM_BUILD_ROOT/usr --enable-dump=yes
#CFLAGS=-O2 LDFLAGS=-s ./configure --prefix=/usr --enable-dump=yes
(cd src; make libdir=/usr/lib includedir=/usr/include)
make all

%install
make install
/sbin/ldconfig

%files
# %defattr(-, root, root)
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

%doc README doc/SIMULA-HISTORY AUTHORS COPYING ChangeLog NEWS TODO

%post
/sbin/ldconfig

%postun
/sbin/ldconfig
