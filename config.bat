@echo off
rem Configuration script for MSDOS
rem
rem Copyright (C) 1996 Sverre Hvammen Johansen, Stein Krogdahl and Terje Mjøs
rem Department of Informatics, University of Oslo.
rem
rem This program is free software; you can redistribute it and/or modify
rem it under the terms of the GNU General Public License as published by
rem the Free Software Foundation; version 2.
rem
rem This program is distributed in the hope that it will be useful,
rem but WITHOUT ANY WARRANTY; without even the implied warranty of
rem MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
rem GNU General Public License for more details.
rem
rem You should have received a copy of the GNU General Public License
rem along with this program; if not, write to the Free Software
rem Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
rem
rem   YOU'LL NEED THE FOLLOWING UTILITIES TO MAKE EMACS:
rem
rem   + msdos version 3 or better.
rem   + djgpp version 1.12maint1 or later (version 2.0 or later recommended).
rem   + make utility that allows breaking of the 128 chars limit on
rem     command lines.  ndmake (as of version 4.5) won't work due to a
rem     line length limit.  The make that comes with djgpp does work.
rem   + rm and mv (from GNU file utilities).
rem   + sed (you can use the port that comes with DJGPP).
rem
rem   You should be able to get all the above utilities from any SimTel
rem   repository, e.g. ftp.coast.net, in the directories
rem   "SimTel/vendors/djgpp" and "SimTel/vendors/gnu/gnuish/dos_only".  As
rem   usual, please use your local mirroring site to reduce trans-Atlantic
rem   traffic.
rem   ----------------------------------------------------------------------

set djgpp_ver=
Echo Checking whether 'sed' is available...
sed -e "w junk.$$$" <Nul
If Exist junk.$$$ Goto sedOk
Echo To configure 'Cim' you need to have 'sed'!
Goto End
:sedOk
Echo Checking whether 'rm' is available...
rm -f junk.$$$
If Not Exist junk.$$$ Goto rmOk
Echo To configure 'Cim' you need to have 'rm'!
Goto End
:rmOk
Echo Checking whether 'mv' is available...
rm -f junk.1 junk.2
echo foo >junk.1
mv junk.1 junk.2
If Exist junk.2 Goto mvOk
Echo To configure 'Cim' you need to have 'mv'!
rm -f junk.1
Goto End
:mvOk
Echo Checking whether 'gcc' is available...
echo main(){} >junk.c
gcc -c junk.c
if exist junk.o goto gccOk
Echo To configure 'Cim' you need to have 'gcc'!
rm -f junk.c
Goto End
:gccOk
rm -f junk.c
rm -f junk.o
Echo Checking what version of DJGPP is installed...
If Not "%DJGPP%" == "" goto djgppOk
Echo To compile 'Cim' under MS-DOS you MUST have DJGPP installed!
Goto End
:djgppOk
echo int main()           >junk.c
echo #ifdef __DJGPP__    >>junk.c
echo {return (__DJGPP__)*10;} >>junk.c
echo #else               >>junk.c
echo #ifdef __GO32__     >>junk.c
echo {return 10;}         >>junk.c
echo #else               >>junk.c
echo {return 0;}         >>junk.c
echo #endif              >>junk.c
echo #endif              >>junk.c
gcc -o junk junk.c
if not exist junk.exe coff2exe junk
junk
If ErrorLevel 10 Goto go32Ok
rm -f junk.c
rm -f junk
rm -f junk.exe
Echo To compile 'Cim' under MS-DOS you MUST have DJGPP installed!
Goto End
:go32Ok
set djgpp_ver=1
If ErrorLevel 20 set djgpp_ver=2
rm -f junk.c
rm -f junk
rm -f junk.exe
Echo Configuring for DJGPP Version %DJGPP_VER% ...
Rem   ----------------------------------------------------------------------
Echo Configuring...

rem   Create "config.h"
rm -f config.h
mv config.dos config.h
:src4

rem   Create "makefile" from "makefile.in".
mv makefile.dos makefile
rem   Edit "clex.c".
sed -e "/y\.tab\.h/s//y\_tab\.h/" <clex.c >junk.c
mv -f junk.c clex.c
rem   Move install
mv -f install install.unix
set djgpp_ver=




