dnl Copyright (C) 2003-2008, 2010, 2011-2014
dnl Rocky Bernstein <rocky@gnu.org>
dnl
dnl This program is free software: you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation, either version 3 of the License, or
dnl (at your option) any later version.
dnl
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program.  If not, see <http://www.gnu.org/licenses/>.

define(RELEASE_NUM, 94)
define(CDIO_VERSION_STR, 0.$1git)

AC_PREREQ(2.61)

AC_INIT([libcdio], [CDIO_VERSION_STR(RELEASE_NUM)], [https://savannah.gnu.org/bugs/?group=libcdio])
AC_CONFIG_SRCDIR(src/cd-info.c)
AM_INIT_AUTOMAKE([foreign subdir-objects])

AC_CANONICAL_HOST
AC_CONFIG_HEADERS(config.h)
AC_CONFIG_MACRO_DIR([m4])

# Enable silent build rules by default (Automake v1.11 or later).
# Disable by either passing --disable-silent-rules to configure or passing V=1 to make
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])],
         [AC_SUBST([AM_DEFAULT_VERBOSITY], [1])])

LIBCDIO_VERSION_NUM=`echo RELEASE_NUM | cut -d . -f 1 | tr -d a-z`
AC_SUBST(LIBCDIO_VERSION_NUM)

AM_MISSING_PROG(HELP2MAN, help2man, $missing_dir)
AM_MAINTAINER_MODE

AM_SANITY_CHECK

AC_ARG_WITH(cd-drive,
	AC_HELP_STRING([--without-cd-drive], [don't build program cd-drive (default with)]),
enable_cd_drive="${withval}", enable_cd_drive=yes)

AC_ARG_WITH(cd-info,
	AC_HELP_STRING([--without-cd-info], [don't build program cd-info (default with)]),
enable_cd_info="${withval}", enable_cd_info=yes)

AC_ARG_WITH(cdda-player,
	AC_HELP_STRING([--without-cdda-player], [don't build program cdda-player (default with)]),
enable_cdda_player="${withval}", enable_cdda_player=yes)

AC_ARG_WITH(cd-read,
	AC_HELP_STRING([--without-cd-read], [don't build program cd-read (default with)]),
enable_cd_read="${withval}", enable_cd_read=yes)

AC_ARG_WITH(iso-info,
	AC_HELP_STRING([--without-iso-info], [don't build program iso-info (default with)]),
enable_iso_info="${withval}", enable_iso_info=yes)

AC_ARG_WITH(iso-read,
	AC_HELP_STRING([--without-iso-read], [don't build program iso-read (default with)]),
enable_iso_read="${withval}", enable_iso_read=yes)

AC_ARG_WITH(versioned-libs,
	AC_HELP_STRING([--without-versioned-libs], [build versioned library symbols (default enabled if you have GNU ld)]),
enable_versioned_libs="${withval}", enable_versioned_libs=yes)

AC_ARG_ENABLE([cxx],
	AC_HELP_STRING([--disable-cxx], [Disable C++ bindings (default enabled)]))
AM_CONDITIONAL([ENABLE_CXX_BINDINGS], [test "x$enable_cxx" != "xno"])

AC_ARG_ENABLE(cpp-progs,
	AC_HELP_STRING([--enable-cpp-progs], [make C++ example programs (default enabled)]))
AM_CONDITIONAL(ENABLE_CPP, test "x$enable_cpp_progs" = "xyes")

AC_ARG_ENABLE(example-progs,
	AC_HELP_STRING([--disable-example-progs], [Don't build libcdio sample programs]))

AM_CONDITIONAL(BUILD_EXAMPLES, test "x$enable_example_progs" != "xno")

dnl We use C
AC_PROG_CC
AM_PROG_CC_C_O

if test "x$enable_cxx" != "xno" ; then
  AC_PROG_CXX
else
  am__fastdepCXX_TRUE='#'
  am__fastdepCXX_FALSE=
fi

dnl For iso_read test
AC_CHECK_PROGS(ISO_MAKING_COMMAND, genisoimage mkisofs)

dnl Checks for programs.
AC_AIX

cd_drivers='cdrdao, BIN/CUE, NRG'

WARN_CFLAGS=''
if test "x$GCC" != "xyes"; then
  AC_MSG_WARN([
  *** non GNU CC compiler detected.
  *** This package has not been tested very well with non GNU compilers
  *** you should try to use 'gcc' for compiling this package.])
else
  # FIXME: add in:
  #   -W -Wshadow -Wundef
  CHECK_CFLAGS="
	-Wall
	-Wcast-align
	-Wchar-subscripts
	-Wdisabled-optimization
	-Wend-labels
	-Winline
	-Wmissing-declarations
	-Wmissing-prototypes
	-Wnested-externs
	-Wno-sign-compare
	-Wpointer-arith
	-Wstrict-prototypes
	-Wunused
	-Wwrite-strings
"
  for OPT in $CHECK_CFLAGS; do
    SAVE_CFLAGS="$CFLAGS"
    CFLAGS="$CFLAGS $OPT"
    AC_MSG_CHECKING([whether $CC understands $OPT])
    AC_TRY_COMPILE([], [], has_option=yes, has_option=no,)
    CFLAGS="$SAVE_CFLAGS"
    AC_MSG_RESULT($has_option)
    if test "x$has_option" = "xyes"; then
        WARN_CFLAGS="$WARN_CFLAGS $OPT"
    fi
    unset has_option
    unset SAVE_CFLAGS
  done
fi
CFLAGS="$CFLAGS $WARN_CFLAGS"

# We use Perl for documentation and regression tests
AC_PATH_PROG(PERL, perl, false)
AC_SUBST(PERL)
AM_CONDITIONAL(HAVE_PERL, test "x$PERL" != "xfalse")

# We use a diff in regression testing
AC_PATH_PROG(DIFF, diff, no)
DIFF_OPTS=

if test "x$DIFF" = "xno"; then
   AC_PATH_PROG(DIFF, cmp, no)
else
  # Try for GNU diff options.
  # MSDOG output uses \r\n rather than \n in tests
  for diff_opt in -w --unified ; do
    if $DIFF $diff_opt ./configure ./configure > /dev/null 2>&1; then
      AC_MSG_RESULT([adding $diff_opt to diff in regression tests])
      DIFF_OPTS="$DIFF_OPTS $diff_opt"
    fi
  done
fi
AC_SUBST(DIFF)
AC_SUBST(DIFF_OPTS)
AC_SUBST(RM)

dnl check for large file support
AC_SYS_LARGEFILE

dnl we need to define _FILE_OFFSET_BITS or _LARGE_FILES on the compiler command
dnl line because otherwise the system headers risk being included before
dnl problems
if test "x$ac_cv_sys_largefiles" = "xyes"; then
    if test "x$ac_cv_sys_file_offset_bits" = "x64"; then
        LIBCDIO_LARGEFILE_FLAGS="-D_FILE_OFFSET_BITS=64 -D_LARGE_FILES"
    else
        LIBCDIO_LARGEFILE_FLAGS="-D_LARGE_FILES"
    fi
    if test "x$ac_cv_sys_largefile_source" != "xno"; then
        LIBCDIO_LARGEFILE_FLAGS="$LIBDDIO_LARGEFILE_FLAGS -D_LARGEFILE_SOURCE=$ac_cv_sys_largefile_source"
    fi
    CPPFLAGS="$CPPFLAGS $LIBCDIO_LARGEFILE_FLAGS"
fi

AC_DEFINE(LIBCDIO_CONFIG_H, 1,
  [Is set when libcdio's config.h has been included. Applications wishing to sue their own config.h values (such as set by the application's configure script can define this before including any of libcdio's headers.])

dnl headers

AC_HEADER_STDC
AC_CHECK_HEADERS(errno.h fcntl.h glob.h limits.h pwd.h stdbool.h)
AC_CHECK_HEADERS(stdarg.h stdbool.h stdio.h sys/cdio.h sys/param.h \
		 sys/time.h sys/timeb.h sys/utsname.h)

## FreeBSD 4 has getopt in unistd.h. So we include that before
## getopt.h
AC_CHECK_HEADERS(unistd.h getopt.h)

AC_SUBST(SBPCD_H)
AC_SUBST(UCDROM_H)
AC_SUBST(TYPESIZES)

## Check compiler-related things
AC_C_BIGENDIAN
AC_C_CONST
AC_C_INLINE

## ISOC99_PRAGMA
AC_MSG_CHECKING([whether $CC supports ISOC99 _Pragma()])
AC_TRY_COMPILE([],[_Pragma("pack(1)")], [
  ISOC99_PRAGMA=yes
  AC_DEFINE(HAVE_ISOC99_PRAGMA, [], [Supports ISO _Pragma() macro])
],ISOC99_PRAGMA=no)
AC_MSG_RESULT($ISOC99_PRAGMA)

## Check for Windows header files
AC_CHECK_HEADERS([ntddcdrm.h ddk/ntddcdrm.h \
                  ntddscsi.h ddk/ntddscsi.h ddk/scsi.h],
[], [], [AC_INCLUDES_DEFAULT
#include <windows.h>
])

##
## Check for S_ISSOCK() and S_ISLNK() macros
##
AC_MSG_CHECKING(for S_ISLNK() macro)
AC_LINK_IFELSE([AC_LANG_PROGRAM([
#ifdef HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif
],[return S_ISLNK(0);])],
[ AC_MSG_RESULT(yes); AC_DEFINE(HAVE_S_ISLNK, [], [Define this <sys/stat.h> defines S_ISLNK()]) ],
[ AC_MSG_RESULT(no) ])

AC_MSG_CHECKING([for S_ISSOCK() macro])
AC_LINK_IFELSE([AC_LANG_PROGRAM([
#ifdef HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif
],[return S_ISSOCK(0);])],
[ AC_MSG_RESULT(yes); AC_DEFINE(HAVE_S_ISSOCK, [], [Define this <sys/stat.h> defines S_ISSOCK()]) ],
[ AC_MSG_RESULT(no) ])

AC_MSG_CHECKING([for struct timespec])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
],[struct timespec ts;])],
[ AC_MSG_RESULT(yes); AC_DEFINE(HAVE_STRUCT_TIMESPEC, [], [Define this if you have struct timespec]) ],
[ AC_MSG_RESULT(no) ])

dnl empty_array_size
AC_MSG_CHECKING([how to create empty arrays])

empty_array_size="xxxx"
AC_TRY_COMPILE([],[struct { int foo; int bar[]; } doo;], empty_array_size="")

if test "x$empty_array_size" = "xxxx"; then
 AC_TRY_COMPILE([],[struct { int foo; int bar[0]; } doo;], empty_array_size="0")
fi

if test "x$empty_array_size" = "xxxx"; then
  AC_MSG_ERROR([compiler is unable to creaty empty arrays])
else
  AC_DEFINE_UNQUOTED(EMPTY_ARRAY_SIZE, $empty_array_size,
	[what to put between the brackets for empty arrays])
  changequote(`,')
  msg="[${empty_array_size}]"
  changequote([,])
  AC_MSG_RESULT($msg)
fi

dnl Enable the creation of shared libraries under Win32.
AC_LIBTOOL_WIN32_DLL

dnl AM_PROG_LIBTOOL tests whether we have GNU ld
dnl this must come before checking --with-versioned-libs
dnl which requires GNU ld.
AM_PROG_LIBTOOL

dnl system

# FIXME:
# I believe some OS's require -lm, but I don't recall for what function
# When we find it, put it in below instead of "cos".
AC_CHECK_LIB(m, cos, [LIBS="$LIBS -lm"; COS_LIB="-lm"])
AC_SUBST(COS_LIB)

# Do we have GNU ld? If we don't, we can't build versioned symbols.
if test "x$with_gnu_ld" != "xyes"; then
   AC_MSG_WARN([I don't see GNU ld. I'm going to assume --without-versioned-libs])
   enable_versioned_libs='no'
fi
# We also need GNU make to build versioned symbols.
if test "x$enable_versioned_libs" = "xyes"; then
  if test -n "$MAKE" ; then
    $MAKE --version 2>/dev/null >/dev/null
    if test "$?" -ne 0 ; then
       AC_MSG_ERROR(Either set MAKE variable to GNU make or use
   --without-versioned-libs option)
    fi
  else
    make --version 2>/dev/null >/dev/null
    if test "$?" -ne 0 ; then
       AC_MSG_ERROR(Either set MAKE variable to GNU make or use
   --without-versioned-libs option)
    fi
  fi
fi

AM_CONDITIONAL(CYGWIN, test "x$CYGWIN" = "xyes")
AM_CONDITIONAL(BUILD_CD_DRIVE, test "x$enable_cd_drive" = "xyes")
AM_CONDITIONAL(BUILD_CDINFO, test "x$enable_cd_info" = "xyes")
AM_CONDITIONAL(BUILD_CD_READ, test "x$enable_cd_read" = "xyes")
AM_CONDITIONAL(BUILD_ISO_INFO, test "x$enable_iso_info" = "xyes")
AM_CONDITIONAL(BUILD_ISO_READ, test "x$enable_iso_read" = "xyes")
AM_CONDITIONAL(BUILD_CDINFO_LINUX, test "x$enable_cd_info_linux" = "xyes")
AM_CONDITIONAL(BUILD_CDIOTEST, test "x$enable_cdiotest" = "xyes")
AM_CONDITIONAL(BUILD_VERSIONED_LIBS, test "x$enable_versioned_libs" = "xyes")
AM_CONDITIONAL(DISABLE_CPP, test "x$disable_cpp" = "xyes")

dnl Checks for header files.

LIBCDIO_CDDA_LIBS='$(top_builddir)/lib/cdda_interface/libcdio_cdda.la'
LIBCDIO_CFLAGS='-I$(top_srcdir)/lib/driver -I$(top_builddir)/include -I$(top_srcdir)/include/'
LIBCDIO_LIBS='$(top_builddir)/lib/driver/libcdio.la'
LIBCDIO_DEPS="$LIBCDIO_LIBS"
LIBCDIOPP_LIBS='$(top_builddir)/lib/cdio++/libcdio++.la'
LIBISO9660PP_LIBS='$(top_builddir)/lib/cdio++/libiso9660++.la'
LIBISO9660_CFLAGS='-I$(top_srcdir)/lib/iso9660/'
LIBISO9660_LIBS='$(top_builddir)/lib/iso9660/libiso9660.la'
LIBUDF_CFLAGS='-I$(top_srcdir)/lib/udf/'
LIBUDF_LIBS='$(top_builddir)/lib/udf/libudf.la'
AC_SUBST(LIBCDIO_CDDA_LIBS)
AC_SUBST(LIBISO9660PP_LIBS)
AC_SUBST(LIBCDIO_LIBS)
AC_SUBST(LIBCDIOPP_LIBS)
AC_SUBST(LIBCDIO_DEPS)
AC_SUBST(LIBISO9660_LIBS)
AC_SUBST(LIBUDF_LIBS)

AC_MSG_CHECKING([Checking what sed uses for extended regular expressions])
test -n "$SED" || SED=$ac_path_SED
SED_EXTENDED_RE_FLAG='-r'
result=$(echo 'define' | $SED -r -e 's/^define/foo/' 2>/dev/null)
if [[ $? -ne 0 ]] ; then
    result=$(echo 'define' | $SED -E -e 's/^define/foo/' 2>/dev/null)
    if [[ $? -eq 0 ]] ; then
        SED_EXTENDED_RE_FLAG='-E'
    else
        AC_MSG_WARN([Don't have SED that understand extended RE's. Some minor compilation issues may fail.])
        SED_EXTENDED_RE_FLAG=''
    fi
fi
AC_MSG_RESULT($SED_EXTENDED_RE_FLAG)
AC_SUBST(SED_EXTENDED_RE_FLAG)

dnl Libtool flag for strict linkage
LT_NO_UNDEFINED=

dnl regression tests need native-style absolute paths.
dnl native_abs_top_srcdir is used here.
case $srcdir in
  .)  # We are building in place.
   native_abs_top_srcdir=$ac_pwd ;;
  [\\/]* | ?:[\\/]* )  # Absolute name.
    native_abs_top_srcdir=$srcdir ;;
  *) # Relative name.
    native_abs_top_srcdir=$ac_pwd/$srcdir ;;
esac
AC_SUBST(native_abs_top_srcdir)

case $host_os in
     aix*)
       ## Don't use AIX driver until starts to really work
       ## cd_drivers="${cd_drivers}, AIX"
       ## AC_DEFINE([HAVE_AIX_CDROM], [1],
       ##     [Define 1 if you have AIX CD-ROM support])
       ;;
     darwin[[6-9]].*|darwin1[[0-4]].*)
       AC_CHECK_HEADERS(IOKit/IOKitLib.h CoreFoundation/CFBase.h,
                        [have_iokit_h="yes"])
       if test "x$have_iokit_h" = "xyes" ; then
         AC_DEFINE([HAVE_DARWIN_CDROM], [1],
	      [Define 1 if you have Darwin OS X-type CD-ROM support])
	 DARWIN_PKG_LIB_HACK="-Wl,-framework,CoreFoundation -Wl,-framework,IOKit"

	 dnl Prior to Mac OS X 10.4 (Tiger), DiskArbitration was a private framework.
	 dnl It's now public, and it's needed to do cd/dvd unmount/eject.
	 AC_MSG_CHECKING([for DiskArbitration framework])
	 ac_save_LIBS="$LIBS" LIBS="$LIBS -framework CoreFoundation -framework DiskArbitration"
	 AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <DiskArbitration/DiskArbitration.h>]], [[]])],
	      [have_diskarbitration_framework=yes], [have_diskarbitration_framework=no])
	 LIBS="$ac_save_LIBS"
	 AC_MSG_RESULT([$have_diskarbitration_framework])
	 if test "x$have_diskarbitration_framework" = "xyes"; then
	     AC_DEFINE([HAVE_DISKARBITRATION], 1, [Define 1 if you have the Apple DiskArbitration framework])
	     DARWIN_PKG_LIB_HACK="$DARWIN_PKG_LIB_HACK -Wl,-framework,DiskArbitration"
	 fi

	 AC_SUBST(DARWIN_PKG_LIB_HACK)
	 LIBCDIO_LIBS="$LIBCDIO_LIBS $DARWIN_PKG_LIB_HACK"
         cd_drivers="${cd_drivers}, Darwin"
       fi
     ;;
     linux*|uclinux)
        AC_CHECK_HEADERS(linux/version.h linux/major.h)
        AC_CHECK_HEADERS(linux/cdrom.h, [have_linux_cdrom_h="yes"])
	if test "x$have_linux_cdrom_h" = "xyes"; then
	   AC_TRY_COMPILE(,[
#include <linux/cdrom.h>
struct cdrom_generic_command test;
int has_timeout=sizeof(test.timeout);],
            [AC_DEFINE([HAVE_LINUX_CDROM_TIMEOUT], [1],
                   [Define 1 if timeout is in cdrom_generic_command struct])])
	   AC_DEFINE([HAVE_LINUX_CDROM], [1],
                     [Define 1 if you have Linux-type CD-ROM support])
	   cd_drivers="${cd_drivers}, GNU/Linux"
	fi
	;;
     bsdi*)
        AC_CHECK_HEADERS(dvd.h, [have_bsdi_dvd_h="yes"])
	if test "x$have_bsdi_dvd_h" = "xyes" ; then
     	  AC_DEFINE([HAVE_BSDI_CDROM], [1],
                    [Define 1 if you have BSDI-type CD-ROM support])
	 LIBS="$LIBS -ldvd -lcdrom"
         LIBCDIO_LIBS="$LIBCDIO_LIBS  -lcdrom"
	  cd_drivers="${cd_drivers}, BSDI"
	fi
	;;
     sunos*|sun*|solaris*)
     	AC_DEFINE([HAVE_SOLARIS_CDROM], [1],
                  [Define 1 if you have Solaris CD-ROM support])
	  cd_drivers="${cd_drivers}, Solaris"
	;;
     cygwin*)
	AC_DEFINE([CYGWIN], [1],
                  [Define 1 if you are compiling using cygwin])
     	AC_DEFINE([HAVE_WIN32_CDROM], [1],
                  [Define 1 if you have MinGW CD-ROM support])
 	LIBS="$LIBS -lwinmm"
 	LT_NO_UNDEFINED="-no-undefined"
  	cd_drivers="${cd_drivers}, MinGW"
     	AC_DEFINE([NEED_TIMEZONEVAR], [1],
                  [Define 1 if you need timezone defined to get timzone
		   defined as a variable. In cygwin it is a function too])
	;;
     mingw*)
        AC_MSG_CHECKING(for MinGW-w64 headers)
        AC_COMPILE_IFELSE([AC_LANG_PROGRAM([
#include <stdio.h>
#if !defined(__MINGW64_VERSION_MAJOR)
# error not mingw-w64 headers
#endif])],
          [ AC_MSG_RESULT(yes)
            MINGW_W64_HEADERS=yes
            AC_DEFINE(MINGW_W64_HEADERS, [], [Define this if mingw-w64 headers are used]) ],
          [ AC_MSG_RESULT(no)
            MINGW_W64_HEADERS=no
          ])
        # regression tests need native-style absolute paths.
        # $native_srcdir is used here.
	native_abs_top_srcdir=$(cd $srcdir; /bin/bash.exe -c 'builtin pwd -W')

	AC_CHECK_HEADERS(windows.h)
	AC_DEFINE([MINGW32], [1],
                  [Define 1 if you are compiling using MinGW])
	MINGW32=1
	AC_DEFINE([HAVE_WIN32_CDROM], [1],
                  [Define 1 if you have MinGW CD-ROM support])
	LIBS="$LIBS -lwinmm"
	LT_NO_UNDEFINED="-no-undefined"
	cd_drivers="${cd_drivers}, MinGW "
	## Cross-platform LFS support in MinGW requires an override of off_t
	if test "x$enable_largefile" != "xno"; then
	  AC_MSG_RESULT([enabling support for large files (_FILE_OFFSET_BITS=64)])
          if test "x$MINGW_W64_HEADERS" = "xyes"
          then
	    LIBCDIO_CFLAGS+=' -D_FILE_OFFSET_BITS=64 '
	    LIBISO9660_CFLAGS+=' -D_FILE_OFFSET_BITS=64 '
          else
	    LIBCDIO_CFLAGS+=' -D_FILE_OFFSET_BITS=64 -D_OFF_T_DEFINED -D_OFF_T_ -D_off_t=off64_t -Doff_t=off64_t'
	    LIBISO9660_CFLAGS+=' -D_FILE_OFFSET_BITS=64 -D_OFF_T_DEFINED -D_OFF_T_ -D_off_t=off64_t -Doff_t=off64_t'
          fi
	fi
	joliet_supported="yes"
	if test "x$enable_cdda_player" = "xyes"; then
	  AC_MSG_RESULT([disabling cdda_player (not supported on this platform)])
	fi
	enable_cdda_player="no"
	;;
     freebsd[[4-9]].*|freebsd10.*|dragonfly*|kfreebsd*)
     	AC_DEFINE([HAVE_FREEBSD_CDROM], [1],
                  [Define 1 if you have FreeBSD CD-ROM support])
 	LIBS="$LIBS -lcam"
  	cd_drivers="${cd_drivers}, FreeBSD "
	;;
     netbsd*|openbsd*)
     	AC_DEFINE([HAVE_NETBSD_CDROM], [1],
                  [Define 1 if you have NetBSD CD-ROM support])
 	# LIBS="$LIBS -lcam"
  	cd_drivers="${cd_drivers}, NetBSD "
	;;
     os2*)
       AC_DEFINE([HAVE_OS2_CDROM], [1],
                  [Define 1 if you have OS/2 CD-ROM support])
       LT_NO_UNDEFINED="-no-undefined"
       LDFLAGS="$LDFLAGS -Zbin-files"
       cd_drivers="${cd_drivers}, OS2 "
       ;;
     *)
	AC_MSG_WARN([Don't have OS CD-reading support for ${host_os}...])
	AC_MSG_WARN([Will use generic support.])
	;;
esac

AC_SUBST(LIBCDIO_CFLAGS)
AC_SUBST(LIBISO9660_CFLAGS)
AC_SUBST(LT_NO_UNDEFINED)

AC_MSG_CHECKING(extern long timezone variable)
AC_LINK_IFELSE([AC_LANG_SOURCE([[
#ifdef NEED_TIMEZONEVAR
#define timezonevar 1
#endif

#include <time.h>
extern long timezone;
int main(int argc, char **argv) {
  long test_timezone = timezone;
  return 0;
}
]])
], [AC_MSG_RESULT(yes);
   AC_DEFINE([HAVE_TIMEZONE_VAR], 1,
	[Define if you have an extern long timenzone variable.])],
   [AC_MSG_RESULT(no)])
dnl

AC_SUBST(LINUX_CDROM_TIMEOUT)
AC_SUBST(DARWIN_PKG_LIB_HACK)
AC_SUBST(HAVE_BSDI_CDROM)
AC_SUBST(HAVE_DARWIN_CDROM)
AC_SUBST(HAVE_FREEBSD_CDROM)
AC_SUBST(HAVE_LINUX_CDROM)
AC_SUBST(HAVE_SOLARIS_CDROM)
AC_SUBST(HAVE_WIN32_CDROM)
AC_SUBST(HAVE_OS2_CDROM)

LIBCDIO_SOURCE_PATH="`pwd`"
AC_DEFINE_UNQUOTED(LIBCDIO_SOURCE_PATH, "$LIBCDIO_SOURCE_PATH",
	[Full path to libcdio top_sourcedir.])
AC_SUBST(LIBCDIO_SOURCE_PATH)

AC_CHECK_FUNCS( [chdir drand48 fseeko fseeko64 ftruncate geteuid getgid \
		 getuid getpwuid gettimeofday lseek64 lstat memcpy memset mkstemp rand \
		 seteuid setegid snprintf setenv strndup unsetenv tzset sleep \
		 _stati64 usleep vsnprintf readlink realpath gmtime_r localtime_r] )

# check for timegm() support
AC_CHECK_FUNC(timegm, AC_DEFINE(HAVE_TIMEGM,1,
		      [Define 1 if timegm is available]))

AC_CHECK_MEMBER([struct tm.tm_gmtoff],
                [AC_DEFINE(HAVE_TM_GMTOFF, 1,
                        [Define if struct tm has the tm_gmtoff member.])],
                 ,
                 [#include <time.h>])

if test "x$ac_cv_member_struct_tm_tm_gmtoff" = "xyes"; then
  AC_MSG_CHECKING([whether time.h defines daylight and timezone variables])
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([
#include <time.h>
],[return (timezone != 0) + daylight;])],
  [AC_DEFINE(HAVE_DAYLIGHT, 1,
   [Define if time.h defines extern long timezone and int daylight vars.])
   has_daylight=yes
   ],[has_daylight=no])
  AC_MSG_RESULT($has_daylight)

  AC_MSG_CHECKING([whether time.h defines tzname variable])
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([
#include <time.h>
],[return (tzname != NULL);])],
  [AC_DEFINE(HAVE_TZNAME, 1,
   [Define if time.h defines extern extern char *tzname[2] variable])
   has_tzname=yes
   ],[has_tzname=no])
   AC_MSG_RESULT($has_tzname)
fi

AC_ARG_ENABLE(joliet,
	AS_HELP_STRING([--disable-joliet], [don't include Joliet extension support (default enabled)]),
	[enable_joliet=$enableval],
	[enable_joliet=yes])
if test "x${enable_joliet}" != "xno"; then
  AM_ICONV
  AM_LANGINFO_CODESET
  if test "x$am_cv_func_iconv" = "xyes"; then
     joliet_supported="yes"
  fi
  if test "x$joliet_supported" = "xyes"; then
     AC_DEFINE(HAVE_JOLIET, [1],
     [Define 1 if you want ISO-9660 Joliet extension support.
     On most platforms, this requires libiconv to be installed.])
     HAVE_JOLIET=1
  else
     AC_MSG_ERROR([You must have iconv installed.])
  fi
fi
AC_SUBST(HAVE_JOLIET)

AC_ARG_ENABLE(rock,
	AC_HELP_STRING([--enable-rock], [include Rock-Ridge extension support (default enabled)]),
	enable_rock=$enableval, enable_rock=yes)
if test "x${enable_rock}" != "xno"; then
   AC_DEFINE(HAVE_ROCK, [1],
   [Define 1 if you want ISO-9660 Rock-Ridge extension support.])
   HAVE_ROCK=1
fi
AC_SUBST(HAVE_ROCK)
AC_SUBST(MINGW32)
AM_CONDITIONAL(ENABLE_ROCK, test "x$enable_rock" = "xyes")

AC_ARG_ENABLE(cddb,
	AC_HELP_STRING([--enable-cddb], [include CDDB lookups in cd_info (default enabled)]),
	enable_cddb=$enableval, enable_cddb=check)
if test "x$enable_cddb" != "xno"; then
   PKG_CHECK_MODULES(CDDB, libcddb >= 1.0.1, [
    HAVE_CDDB=yes
    AC_DEFINE(HAVE_CDDB, [], [Define this if you have libcddb installed])
    ],
    [AC_MSG_WARN([new enough libcddb not found. CDDB access disabled. Get libcddb from http://libcddb.sourceforge.net])
    HAVE_CDDB=no])
    AC_CHECK_LIB(socket, connect)
    AC_CHECK_FUNC(gethostbyname, , AC_CHECK_LIB(nsl, gethostbyname))
fi


AC_SUBST(CDDB_LIBS)

# We have to disable cdda_player unless we have either ncurses.h or
# curses.h
have_ncurses_h='no'
if test "x$enable_cdda_player" = "xyes"; then
    enable_cdda_player='no'
    AC_CHECK_HEADERS(ncurses.h,
		   [enable_cdda_player='yes'; have_ncurses_h='yes'],
		   [AC_CHECK_HEADERS(curses.h,
		                    [enable_cdda_player='yes'],
				    [AC_CHECK_HEADERS(ncurses/ncurses.h,
				                      enable_cdda_player='yes')
						      ])])
fi

AC_DEFINE(HAVE_KEYPAD, [], [Define this if your libcurses has keypad])
if test "x$enable_cdda_player" = "xyes"; then
    if test "x$have_ncurses_h" = "xyes"; then
      AC_CHECK_LIB(ncurses, mvprintw,
        [LIBCURSES=ncurses; CDDA_PLAYER_LIBS="$CDDA_PLAYER_LIBS -lncurses"],
        AC_CHECK_LIB(curses, mvprintw,
                     [LIBCURSES=curses;
                       CDDA_PLAYER_LIBS="$CDDA_PLAYER_LIBS -lcurses"],
                     [AC_MSG_WARN([Will not build cdda-player - did not find libncurses or libcurses])
                     enable_cdda_player=no]))
    else
        AC_CHECK_LIB(curses, mvprintw,
                     [LIBCURSES=curses;
                     CDDA_PLAYER_LIBS="$CDDA_PLAYER_LIBS -lcurses"],
                     [AC_MSG_WARN([Will not build cdda-player - did not find libcurses])])
    fi
    if test "x$enable_cdda_player" = "xyes"; then
      AC_CHECK_LIB($LIBCURSES, keypad, [HAVE_KEYPAD=yes])
    fi
fi

AM_CONDITIONAL(BUILD_CDDA_PLAYER, test "x$enable_cdda_player" = "xyes")
AC_SUBST(CDDA_PLAYER_LIBS)

AC_ARG_ENABLE(vcd_info,
	AC_HELP_STRING([--enable-vcd-info], [include Video CD Info from libvcd]),
		enable_vcd_info=${enableval},
		enable_vcd_info=no)
if test "x$enable_vcd_info" = "xyes"; then
  PKG_CHECK_MODULES(VCDINFO, libvcdinfo >= 0.7.21,
  [AC_DEFINE([HAVE_VCDINFO],1,
  [Define this if you have libvcdinfo installed])],
  [AC_MSG_WARN(a new enough libvcdinfo not found.
VCD info display in cd-info disabled.
libvcdinfo is part of vcdimager. Get it from http://vcdimager.org)
   enable_vcd_info=no])
fi

AC_SUBST(VCDINFO_LIBS)
AC_SUBST(VCDINFO_CFLAGS)

dnl
dnl Newest automake workaround - needed for multi-language manual pages
dnl
AC_SUBST(mkdir_p, "${MKDIR_P}")

AC_CONFIG_COMMANDS([checks],
		   [chmod +x test/check_cue.sh; chmod +x test/check_nrg.sh
		   ])

dnl
dnl Output configuration files
dnl
## AC_CONFIG_FILES([  po/Makefile.in\
AC_CONFIG_FILES([
       Makefile \
       example/Makefile \
       include/Makefile \
       include/cdio/Makefile \
       include/cdio++/Makefile \
       include/cdio/version.h \
       doc/doxygen/Doxyfile \
       doc/Makefile \
       lib/Makefile \
       lib/driver/Makefile \
       lib/iso9660/Makefile \
       lib/udf/Makefile \
       libcdio.pc \
       libiso9660.pc \
       libudf.pc \
       package/libcdio.spec \
       src/Makefile \
       test/check_common_fn \
       test/data/Makefile \
       test/driver/Makefile \
       test/driver/bincue.c \
       test/driver/cdrdao.c \
       test/driver/nrg.c \
       test/driver/track.c \
       test/testgetdevices.c \
       test/testisocd_joliet.c \
       test/testisorr.c \
       test/testisocd2.c \
       test/testpregap.c \
       test/testudf.c \
       test/Makefile \
       ])

if test "x$enable_cxx" != "xno" ; then
   AC_CONFIG_FILES([
      example/C++/Makefile \
      example/C++/OO/Makefile \
      lib/cdio++/Makefile \
      libcdio++.pc \
      libiso9660++.pc \
   ])
fi

# AC_CONFIG_FILES([po/Makefile])
AC_CONFIG_FILES([test/check_cue.sh], [chmod +x test/check_cue.sh])
AC_CONFIG_FILES([test/check_iso.sh], [chmod +x test/check_iso.sh])
AC_CONFIG_FILES([test/check_nrg.sh], [chmod +x test/check_nrg.sh])
AC_CONFIG_FILES([test/check_udf.sh], [chmod +x test/check_udf.sh])
AC_CONFIG_FILES([test/check_iso_read.sh],
		[chmod +x test/check_iso_read.sh])
AC_OUTPUT

AC_MSG_NOTICE([
Using CD-ROM drivers : $cd_drivers
Building cd-info     : $(test "x$enable_cd_info" = "xyes" && echo yes || echo no)
Building cd-read     : $(test "x$enable_cd_read" = "xyes" && echo yes || echo no)
Building cdda-player : $(test "x$enable_cdda_player" = "xyes" && echo yes || echo no)
Building iso-info    : $(test "x$enable_iso_info" = "xyes" && echo yes || echo no)
Building iso-read    : $(test "x$enable_iso_read" = "xyes" && echo yes || echo no)
Building C++ programs: $(test "x$enable_cxx" != "xno" && echo yes || echo no)])
