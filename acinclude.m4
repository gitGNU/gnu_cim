dnl Copyright (C) 1997 Sverre Hvammen Johansen,
dnl Department of Informatics, University of Oslo.
dnl
dnl This program is free software; you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation; version 2.
dnl
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program; if not, write to the Free Software
dnl Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. */

AC_DEFUN(CIM_LINK_STATIC_FLAG,
[
AC_MSG_CHECKING(grepping libtool to find link_static_flag)
AC_CACHE_VAL(cim_cv_link_static_flag,[
eval `grep link_static_flag libtool|head -1`
cim_cv_link_static_flag=$link_static_flag
])
AC_MSG_RESULT($cim_cv_link_static_flag)
AC_DEFINE_UNQUOTED(LINK_STATIC_FLAG,"$cim_cv_link_static_flag")
])dnl

AC_DEFUN(CIM_PIC_FLAG,
[
AC_MSG_CHECKING(grepping libtool to find pic_flag)
AC_CACHE_VAL(cim_cv_pic_flag,[
eval `grep pic_flag libtool|head -1`
cim_cv_pic_flag=$pic_flag
])
AC_MSG_RESULT($cim_cv_pic_flag)
AC_DEFINE_UNQUOTED(PIC_FLAG,"$cim_cv_pic_flag")
])dnl

AC_DEFUN(CIM_WL_FLAG,
[
AC_MSG_CHECKING(grepping libtool to find wl_flag)
AC_CACHE_VAL(cim_cv_wl_flag,[
eval `grep wl= libtool|head -1`
cim_cv_wl_flag=$wl
])
AC_MSG_RESULT($cim_cv_wl_flag)
AC_DEFINE_UNQUOTED(WL_FLAG,"$cim_cv_wl_flag")
])dnl

AC_DEFUN(CIM_TRAP,
[

AC_MSG_CHECKING(signal SIGFPE)
AC_CACHE_VAL(cim_cv_sigfpe,
[AC_TRY_RUN([#if HAVE_SIGNAL_H
#include <signal.h>
#endif
int t(){}
main() {
signal(SIGFPE,t);
exit(0);
}],cim_cv_sigfpe=yes,cim_cv_sigfpe=no,cim_cv_sigfpe=yes)])
AC_MSG_RESULT($cim_cv_sigfpe)
if test $cim_cv_sigfpe = yes; then
  AC_DEFINE(HAVE_SIGFPE)
fi

AC_MSG_CHECKING(signal SIGSEGV)
AC_CACHE_VAL(cim_cv_sigsegv,
[AC_TRY_RUN([#if HAVE_SIGNAL_H
#include <signal.h>
#endif
int t(){}
main() {
signal(SIGSEGV,t);
exit(0);
}],cim_cv_sigsegv=yes,cim_cv_sigsegv=no,cim_cv_sigsegv=yes)])
AC_MSG_RESULT($cim_cv_sigsegv)
if test $cim_cv_sigsegv = yes; then
  AC_DEFINE(HAVE_SIGSEGV)
fi

AC_MSG_CHECKING(signal SIGILL)
AC_CACHE_VAL(cim_cv_sigill,
[AC_TRY_RUN([#if HAVE_SIGNAL_H
#include <signal.h>
#endif
int t(){}
main() {
signal(SIGILL,t);
exit(0);
}],cim_cv_sigill=yes,cim_cv_sigill=no,cim_cv_sigill=yes)])
AC_MSG_RESULT($cim_cv_sigill)
if test $cim_cv_sigill = yes; then
  AC_DEFINE(HAVE_SIGILL)
fi

AC_MSG_CHECKING(signal SIGTRAP)
AC_CACHE_VAL(cim_cv_sigtrap,
[AC_TRY_RUN([#if HAVE_SIGNAL_H
#include <signal.h>
#endif
int t(){}
main() {
signal(SIGTRAP,t);
exit(0);
}],cim_cv_sigtrap=yes,cim_cv_sigtrap=no,cim_cv_sigtrap=yes)])
AC_MSG_RESULT($cim_cv_sigtrap)
if test $cim_cv_sigtrap = yes; then
  AC_DEFINE(HAVE_SIGTRAP)
fi

AC_MSG_CHECKING(signal SIGSYS)
AC_CACHE_VAL(cim_cv_sigsys,
[AC_TRY_RUN([#if HAVE_SIGNAL_H
#include <signal.h>
#endif
int t(){}
main() {
signal(SIGSYS,t);
exit(0);
}],cim_cv_sigsys=yes,cim_cv_sigsys=no,cim_cv_sigsys=yes)])
AC_MSG_RESULT($cim_cv_sigsys)
if test $cim_cv_sigsys = yes; then
  AC_DEFINE(HAVE_SIGSYS)
fi

AC_MSG_CHECKING(signal SIGBUS)
AC_CACHE_VAL(cim_cv_sigbus,
[AC_TRY_RUN([#if HAVE_SIGNAL_H
#include <signal.h>
#endif
int t(){}
main() {
signal(SIGBUS,t);
exit(0);
}],cim_cv_sigbus=yes,cim_cv_sigbus=no,cim_cv_sigbus=yes)])
AC_MSG_RESULT($cim_cv_sigbus)
if test $cim_cv_sigbus = yes; then
  AC_DEFINE(HAVE_SIGBUS)
fi

])dnl

AC_DEFUN(CIM_BINARY_FILE,
[
AC_MSG_CHECKING(whether files must be opened in binary mode)
AC_CACHE_VAL(cim_cv_binary_file,
[AC_TRY_RUN([#include <stdio.h>
main() {
FILE *f;
f=fopen("conffile","w");
putc('\032',f);
fclose(f);
f=fopen("conffile","r");
if (getc(f)=='\032') return (0); else return(1);
}],cim_cv_binary_file=no,cim_cv_binary_file=yes,cim_cv_binary_file=no)])
rm -f conffile
AC_MSG_RESULT($cim_cv_binary_file)
if test $cim_cv_binary_file = yes; then
  AC_DEFINE(OPEN_FILE_IN_BINARY_MODE)
fi
])dnl

AC_DEFUN(CIM_DEFAULTS,
[
AC_MSG_CHECKING(assuming iso latin)
AC_CACHE_VAL(cim_cv_ISO_latin,[cim_cv_iso_latin=yes])
AC_MSG_RESULT($cim_cv_iso_latin)
if test $cim_cv_iso_latin = yes; then
  AC_DEFINE(ISO_LATIN)
fi

AC_MSG_CHECKING(assuming input line length)
AC_CACHE_VAL(cim_cv_input_line_length,[cim_cv_input_line_length=80])
AC_MSG_RESULT($cim_cv_input_line_length)
AC_DEFINE_UNQUOTED(INPUT_LINE_LENGTH,$cim_cv_input_line_length)

AC_MSG_CHECKING(assuming output line length)
AC_CACHE_VAL(cim_cv_output_line_length,[cim_cv_output_line_length=80])
AC_MSG_RESULT($cim_cv_output_line_length)
AC_DEFINE_UNQUOTED(OUTPUT_LINE_LENGTH,$cim_cv_output_line_length)

AC_MSG_CHECKING(assuming lines per page)
AC_CACHE_VAL(cim_cv_lines_per_page,[cim_cv_lines_per_page=60])
AC_MSG_RESULT($cim_cv_lines_per_page)
AC_DEFINE_UNQUOTED(LINES_PER_PAGE,$cim_cv_lines_per_page)

AC_MSG_CHECKING(assuming the size of the heap in kB)
AC_CACHE_VAL(cim_cv_dynmemsizekb,[cim_cv_dynmemsizekb=512])
AC_MSG_RESULT($cim_cv_dynmemsizekb)
AC_DEFINE_UNQUOTED(DYNMEMSIZEKB,$cim_cv_dynmemsizekb)

AC_MSG_CHECKING(assuming that dump is implementable)
AC_ARG_ENABLE(dump,
[  --enable-dump           Enable implementation of (un)dump],
[case "${enableval}" in
  yes) cim_dump=yes ;;
  no)  cim_dump=no ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-dump) ;;
esac],[cim_dump=no])
AC_MSG_RESULT($cim_dump)
if test $cim_dump = yes; then
  AC_DEFINE(DUMP)
fi

AC_MSG_CHECKING(assuming that floatingpoint conform to IEEE-754)
case "$target" in
  vax-*-*)
    AC_CACHE_VAL(cim_cv_float_ieee,[cim_cv_float_ieee=no])
                        ;;
  *-cray-unicos*)
	AC_CACHE_VAL(cim_cv_float_ieee,[cim_cv_float_ieee=no])
        ;;
  *-*-*)                
	AC_CACHE_VAL(cim_cv_float_ieee,[cim_cv_float_ieee=yes])
	;;
esac
AC_MSG_RESULT($cim_cv_float_ieee)
if test $cim_cv_float_ieee = yes; then
  AC_DEFINE(FLOAT_IEEE)
fi

AC_MSG_CHECKING(assuming that floatingpoint conform to VAX-format)
case "$target" in
  vax-*-*)
    AC_CACHE_VAL(cim_cv_float_vax,[cim_cv_float_vax=yes])
    ;;
  *-*-*)
    AC_CACHE_VAL(cim_cv_float_vax,[cim_cv_float_vax=no])
    ;;
esac
AC_MSG_RESULT($cim_cv_float_vax)
if test $cim_cv_float_vax = yes; then
  AC_DEFINE(FLOAT_VAX)
fi

AC_MSG_CHECKING(assuming value of max double)
if test $ac_cv_header_stdc = yes; then
  AC_CACHE_VAL(cim_cv_max_double,[cim_cv_max_double=DBL_MAX])
else
  AC_CACHE_VAL(cim_cv_max_double,[cim_cv_max_double=MAXDOUBLE])
fi
AC_MSG_RESULT($cim_cv_max_double)
AC_DEFINE_UNQUOTED(MAX_DOUBLE,$cim_cv_max_double)

AC_MSG_CHECKING(assuming value of min double)
if test $ac_cv_header_stdc = yes; then
  AC_CACHE_VAL(cim_cv_min_double,[cim_cv_min_double=DBL_MIN])
else
  AC_CACHE_VAL(cim_cv_min_double,[cim_cv_min_double=MINDOUBLE])
fi
AC_MSG_RESULT($cim_cv_min_double)
AC_DEFINE_UNQUOTED(MIN_DOUBLE,$cim_cv_min_double)

AC_MSG_CHECKING(assuming alignment)
AC_CACHE_VAL(cim_cv_alignment,[cim_cv_alignment=8])
AC_MSG_RESULT($cim_cv_alignment)
AC_DEFINE_UNQUOTED(ALIGNMENT,$cim_cv_alignment)

])dnl

