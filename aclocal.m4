dnl aclocal.m4 generated automatically by aclocal 1.2

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
eval `grep link_static_flag libtool`
cim_cv_link_static_flag=$link_static_flag
])
AC_MSG_RESULT($cim_cv_link_static_flag)
AC_DEFINE_UNQUOTED(LINK_STATIC_FLAG,"$cim_cv_link_static_flag")
])dnl

AC_DEFUN(CIM_PIC_FLAG,
[
AC_MSG_CHECKING(grepping libtool to find pic_flag)
AC_CACHE_VAL(cim_cv_pic_flag,[
eval `grep pic_flag libtool`
cim_cv_pic_flag=$pic_flag
])
AC_MSG_RESULT($cim_cv_pic_flag)
AC_DEFINE_UNQUOTED(PIC_FLAG,"$cim_cv_pic_flag")
])dnl

AC_DEFUN(CIM_WL_FLAG,
[
AC_MSG_CHECKING(grepping libtool to find wl_flag)
AC_CACHE_VAL(cim_cv_wl_flag,[
eval `grep wl= libtool`
cim_cv_wl_flag=$pic_flag
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
AC_CACHE_VAL(cim_cv_dump,[cim_cv_dump=no])
AC_MSG_RESULT($cim_cv_dump)
if test $cim_cv_dump = yes; then
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


# Do all the work for Automake.  This macro actually does too much --
# some checks are only needed if your package does certain things.
# But this isn't really a big deal.

# serial 1

dnl Usage:
dnl AM_INIT_AUTOMAKE(package,version, [no-define])

AC_DEFUN(AM_INIT_AUTOMAKE,
[AC_REQUIRE([AM_PROG_INSTALL])
PACKAGE=[$1]
AC_SUBST(PACKAGE)
VERSION=[$2]
AC_SUBST(VERSION)
dnl test to see if srcdir already configured
if test "`cd $srcdir && pwd`" != "`pwd`" && test -f $srcdir/config.status; then
  AC_MSG_ERROR([source directory already configured; run "make distclean" there first])
fi
ifelse([$3],,
AC_DEFINE_UNQUOTED(PACKAGE, "$PACKAGE")
AC_DEFINE_UNQUOTED(VERSION, "$VERSION"))
AM_SANITY_CHECK
AC_ARG_PROGRAM
dnl FIXME This is truly gross.
missing_dir=`cd $ac_aux_dir && pwd`
AM_MISSING_PROG(ACLOCAL, aclocal, $missing_dir)
AM_MISSING_PROG(AUTOCONF, autoconf, $missing_dir)
AM_MISSING_PROG(AUTOMAKE, automake, $missing_dir)
AM_MISSING_PROG(AUTOHEADER, autoheader, $missing_dir)
AM_MISSING_PROG(MAKEINFO, makeinfo, $missing_dir)
AC_PROG_MAKE_SET])


# serial 1

AC_DEFUN(AM_PROG_INSTALL,
[AC_REQUIRE([AC_PROG_INSTALL])
test -z "$INSTALL_SCRIPT" && INSTALL_SCRIPT='${INSTALL_PROGRAM}'
AC_SUBST(INSTALL_SCRIPT)dnl
])

#
# Check to make sure that the build environment is sane.
#

AC_DEFUN(AM_SANITY_CHECK,
[AC_MSG_CHECKING([whether build environment is sane])
# Just in case
sleep 1
echo timestamp > conftestfile
# Do `set' in a subshell so we don't clobber the current shell's
# arguments.  Must try -L first in case configure is actually a
# symlink; some systems play weird games with the mod time of symlinks
# (eg FreeBSD returns the mod time of the symlink's containing
# directory).
if (
   set X `ls -Lt $srcdir/configure conftestfile 2> /dev/null`
   if test "$@" = "X"; then
      # -L didn't work.
      set X `ls -t $srcdir/configure conftestfile`
   fi
   test "[$]2" = conftestfile
   )
then
   # Ok.
   :
else
   AC_MSG_ERROR([newly created file is older than distributed files!
Check your system clock])
fi
rm -f conftest*
AC_MSG_RESULT(yes)])

dnl AM_MISSING_PROG(NAME, PROGRAM, DIRECTORY)
dnl The program must properly implement --version.
AC_DEFUN(AM_MISSING_PROG,
[AC_MSG_CHECKING(for working $2)
# Run test in a subshell; some versions of sh will print an error if
# an executable is not found, even if stderr is redirected.
# Redirect stdin to placate older versions of autoconf.  Sigh.
if ($2 --version) < /dev/null > /dev/null 2>&1; then
   $1=$2
   AC_MSG_RESULT(found)
else
   $1="$3/missing $2"
   AC_MSG_RESULT(missing)
fi
AC_SUBST($1)])

# Like AC_CONFIG_HEADER, but automatically create stamp file.

AC_DEFUN(AM_CONFIG_HEADER,
[AC_PREREQ([2.12])
AC_CONFIG_HEADER([$1])
dnl When config.status generates a header, we must update the stamp-h file.
dnl This file resides in the same directory as the config header
dnl that is generated.  We must strip everything past the first ":",
dnl and everything past the last "/".
AC_OUTPUT_COMMANDS(changequote(<<,>>)dnl
ifelse(patsubst(<<$1>>, <<[^ ]>>, <<>>), <<>>,
<<test -z "<<$>>CONFIG_HEADERS" || echo timestamp > patsubst(<<$1>>, <<^\([^:]*/\)?.*>>, <<\1>>)stamp-h<<>>dnl>>,
<<am_indx=1
for am_file in <<$1>>; do
  case " <<$>>CONFIG_HEADERS " in
  *" <<$>>am_file "*<<)>>
    echo timestamp > `echo <<$>>am_file | sed -e 's%:.*%%' -e 's%[^/]*$%%'`stamp-h$am_indx
    ;;
  esac
  am_indx=`expr "<<$>>am_indx" + 1`
done<<>>dnl>>)
changequote([,]))])

# Check to see if we're running under Cygwin32, without using
# AC_CANONICAL_*.  If so, set output variable EXEEXT to ".exe".
# Otherwise set it to "".

dnl AM_CYGWIN32()
dnl You might think we can do this by checking for a cygwin32-specific
dnl cpp define.  We can't, because cross-compilers that target
dnl cygwin32 don't use the .exe suffix.  I don't know why.
AC_DEFUN(AM_CYGWIN32,
[AC_CACHE_CHECK(for Cygwin32 environment, am_cv_cygwin32,
[cat > conftest.$ac_ext << 'EOF'
int main () {
/* Nothing.  */
return 0; }
EOF
if AC_TRY_EVAL(ac_link) && test -s conftest.exe; then
   am_cv_cygwin32=yes
else
   am_cv_cygwin32=no
fi
rm -f conftest*])
EXEEXT=
test "$am_cv_cygwin32" = yes && EXEEXT=.exe
AC_SUBST(EXEEXT)])


# serial 9 AM_PROG_LIBTOOL
AC_DEFUN(AM_PROG_LIBTOOL,
[AC_REQUIRE([AC_CANONICAL_HOST])
AC_REQUIRE([AC_PROG_CC])
AC_REQUIRE([AC_PROG_RANLIB])
AC_REQUIRE([AM_PROG_LD])
AC_REQUIRE([AC_PROG_LN_S])

# Always use our own libtool.
LIBTOOL='$(top_builddir)/libtool'
AC_SUBST(LIBTOOL)

dnl Allow the --disable-shared flag to stop us from building shared libs.
AC_ARG_ENABLE(shared,
[  --enable-shared         build shared libraries [default=yes]],
test "$enableval" = no && libtool_shared=" --disable-shared",
libtool_shared=)

dnl Allow the --disable-static flag to stop us from building static libs.
AC_ARG_ENABLE(static,
[  --enable-static         build static libraries [default=yes]],
test "$enableval" = no && libtool_static=" --disable-static",
libtool_static=)

libtool_flags="$libtool_shared$libtool_static"
test "$silent" = yes && libtool_flags="$libtool_flags --silent"
test "$ac_cv_prog_gcc" = yes && libtool_flags="$libtool_flags --with-gcc"
test "$ac_cv_prog_gnu_ld" = yes && libtool_flags="$libtool_flags --with-gnu-ld"

# Some flags need to be propagated to the compiler or linker for good
# libtool support.
[case "$host" in
*-*-irix6*)
  for f in '-32' '-64' '-cckr' '-n32' '-mips1' '-mips2' '-mips3' '-mips4'; do
    if echo " $CC $CFLAGS " | egrep -e "[ 	]$f[	 ]" > /dev/null; then
      LD="${LD-ld} $f"
    fi
  done
  ;;

*-*-sco3.2v5*)
  # On SCO OpenServer 5, we need -belf to get full-featured binaries.
  CFLAGS="$CFLAGS -belf"
  ;;
esac]

# Actually configure libtool.  ac_aux_dir is where install-sh is found.
CC="$CC" CFLAGS="$CFLAGS" CPPFLAGS="$CPPFLAGS" \
LD="$LD" RANLIB="$RANLIB" LN_S="$LN_S" \
${CONFIG_SHELL-/bin/sh} $ac_aux_dir/ltconfig \
$libtool_flags --no-verify $ac_aux_dir/ltmain.sh $host \
|| AC_MSG_ERROR([libtool configure failed])
])

# AM_PROG_LD - find the path to the GNU or non-GNU linker
AC_DEFUN(AM_PROG_LD,
[AC_ARG_WITH(gnu-ld,
[  --with-gnu-ld           assume the C compiler uses GNU ld [default=no]],
test "$withval" = no || with_gnu_ld=yes, with_gnu_ld=no)
if test "$with_gnu_ld" = yes; then
  AC_MSG_CHECKING([for GNU ld])
else
  AC_MSG_CHECKING([for non-GNU ld])
fi
AC_CACHE_VAL(ac_cv_path_LD,
[case "$LD" in
  /*)
  ac_cv_path_LD="$LD" # Let the user override the test with a path.
  ;;
  *)
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}:"
  for ac_dir in $PATH; do
    test -z "$ac_dir" && ac_dir=.
    if test -f "$ac_dir/ld"; then
      ac_cv_path_LD="$ac_dir/ld"
      # Check to see if the program is GNU ld.  I'd rather use --version,
      # but apparently some GNU ld's only accept -v.
      # Break only if it was the GNU/non-GNU ld that we prefer.
      if "$ac_cv_path_LD" -v 2>&1 < /dev/null | egrep '(GNU ld|with BFD)' > /dev/null; then
	test "$with_gnu_ld" = yes && break
      else
        test "$with_gnu_ld" != yes && break
      fi
    fi
  done
  IFS="$ac_save_ifs"
  ;;
esac])
LD="$ac_cv_path_LD"
if test -n "$LD"; then
  AC_MSG_RESULT($LD)
else
  AC_MSG_RESULT(no)
fi
test -z "$LD" && AC_MSG_ERROR([no acceptable ld found in \$PATH])
AC_SUBST(LD)
AM_PROG_LD_GNU
])

AC_DEFUN(AM_PROG_LD_GNU,
[AC_CACHE_CHECK([whether we are using GNU ld], ac_cv_prog_gnu_ld,
[# I'd rather use --version here, but apparently some GNU ld's only accept -v.
if $LD -v 2>&1 </dev/null | egrep '(GNU ld|with BFD)' > /dev/null; then
  ac_cv_prog_gnu_ld=yes
else
  ac_cv_prog_gnu_ld=no
fi])
])

