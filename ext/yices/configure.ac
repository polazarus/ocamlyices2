dnl========================================================================
dnl The Yices SMT Solver. Copyright 2015 SRI International.
dnl
dnl This program may only be used subject to the noncommercial end user
dnl license agreement which is downloadable along with this program.
dnl========================================================================

dnl
dnl Configure for Yices
dnl
dnl This builds a platform-dependent configuration
dnl file stored in configs/Makefile.$ARCH
dnl

#
# Version numbers: <version>.<minor>.<patch-level>
# - this determines the name of the distribution tarfile
# - the number is also used in the shared library
#   for linux:  the soname is libyices.so.<major>.<minor>
#   for darwin: the compatibility version is <major>.<minor>.0
#               the version is set to <major>.<minor>.<patch-level>
#
# Conventions we should follow:
# <major> increases for major new releases.
# <minor> increases when we make changes that loose
#         backward compatibility.
# <patch-level> increases for bug fixes or other improvements
#         that maintain backward compatibility.
#
# Example: if P is linked against libyices.so version 2.0.0
# - P should still work (without recompliling) with libyices 2.0.1
# - P should not work anymore with libyices 2.1.0 or 3.0.0
#

AC_PREREQ(2.60)
AC_INIT([Yices],[2.5.2])

MAJOR=$(echo $PACKAGE_VERSION | cut -d "." -f1)
MINOR=$(echo $PACKAGE_VERSION | cut -d "." -f2)
PATCH_LEVEL=$(echo $PACKAGE_VERSION | cut -d "." -f3)
AC_SUBST(MAJOR)
AC_SUBST(MINOR)
AC_SUBST(PATCH_LEVEL)

AC_ARG_VAR(LTFLAGS,AS_HELP_STRING([allows to pass options to libtool, e.g.:
  --dry-run, -n, --finish... ]))

AC_CONFIG_MACRO_DIRS([autoconf/m4])
AC_CONFIG_AUX_DIR([autoconf])
m4_include([autoconf/m4/progtest.m4]) # for checking that libgmp-*.dll is present
m4_include([autoconf/m4/csl_check_libs.m4]) # for CSL_STATIC_CHECK_GMP, CSL_CHECK_STATIC_LIBPOLY, etc...

# Initialize libtool. Libtool make the creation of static and shared
# libraries easier. On Mac OS X, it will automatically create a .dylib,
# on linux a .so, on Windows a .dll...
# This script also checks for CC, LD, AR, RANLIB, STRIP and others.
# Note that LT_INIT will call AC_PROG_RANLIB.
# First, we save user-given CFLAGS because LT_INIT modify it. Note that
# LT_INIT will take care of AC_PROG_CC.
saved_cflags=$CFLAGS
LT_INIT([win32-dll shared static])
CFLAGS=$saved_cflags

AC_SUBST(enable_static)
AC_SUBST(enable_shared)

dnl
dnl Tools for building Yices
dnl ------------------------
dnl
AS_IF([! make -v --version | grep -i "gnu make" > /dev/null],
  [AC_MSG_WARN([*** GNU make is needed for building Yices.])])
AC_PROG_SED
AC_PROG_LN_S
AC_PROG_MKDIR_P
AC_PROG_INSTALL
AC_PROG_EGREP
AC_CHECK_TOOL(LD,ld,ld)
AC_CHECK_TOOL(AR,ar,ar)
AC_CHECK_TOOL(STRIP,strip,strip)

#
# gperf generates source files. Better
# to keep AC_CHECK_PROG rather than AC_CHECK_TOOL
#
AC_CHECK_PROG(GPERF,gperf,gperf)
#
# CHECK GPERF
# -----------
# the src/Makefile uses command-line options that appeared in version 3.0
# of gperf
#
HAS_GPERF=no
if test "x$GPERF" = x; then
  AC_MSG_WARN([*** gperf not found. It is only used for re-generating hash-tables
  when frontend lexers source files are modified (only useful for yices developers).
  If you have no idea what this is, you can ignore this warning.])
else
  AC_MSG_CHECKING([gperf version])
  gperfversion=`$GPERF --version 2>&1 | awk '/^GNU/ { print $3 }'`
  case $gperfversion in
  0.* | 1.* | 2.* )
     AC_MSG_RESULT([${gperfversion}])
     AC_MSG_WARN([*** found $GPERF version $gperfversion but version 3.0 or higher is required ***])
     ;;
   3.*)
     AC_MSG_RESULT([${gperfversion}])
     HAS_GPERF=yes
     ;;
  *)
     AC_MSG_RESULT([${gperfversion}])
     AC_MSG_WARN([*** Unexpected $GPERF output. Cannot determine version. Continuing anyway ***])
     ;;
  esac
fi
AC_SUBST(HAS_GPERF)

if test $enable_static = yes; then
  STATIC_CPPFLAGS+=" -DYICES_STATIC"
fi
AC_SUBST(STATIC_CPPFLAGS)

dnl
dnl Check whether the C compiler accepts the flag -fno-stack-protector
dnl -------------------------------------------------------------------
dnl
dnl Several Linux distributions (also Darwin) come with GCC that has
dnl -fstack-protector enabled by default. This may cause problems at
dnl link time on the user machine (if the user doesn't have the right
dnl runtime libraries). The symptom is:
dnl    undefined reference to '__stack_chk_fail'
dnl To avoid this, we try to give flag -fno-stack-protector when
dnl compiling Yices in release mode. However, this flag is not supported
dnl by older versions of GCC (e.g., gcc-4.0.x don't seem to have it)
dnl
dnl The following test attempts to determine whether the C compiler
dnl supports -fno-stack-protector. If it does, we set variable
dnl NO_STACK_PROTECTOR to -fno-stack-protector.  Otherwise, we leave
dnl NO_STACK_PROTECTOR empty
dnl
NO_STACK_PROTECTOR=""
AC_MSG_CHECKING([whether $CC accepts option -fno-stack-protector])
if $CC -fno-stack-protector -c -xc - -o /dev/null < /dev/null > /dev/null 2>&1 ; then
   NO_STACK_PROTECTOR="-fno-stack-protector"
   AC_MSG_RESULT(yes)
else
   AC_MSG_RESULT(no)
fi;
AC_SUBST(NO_STACK_PROTECTOR)



dnl
dnl GMP Libraries
dnl -------------
dnl
dnl We need as many as three different versions of GMP:
dnl
dnl To build static executables and libraries, we link with the
dnl default libgmp.a (usually /usr/local/lib/libgmp.a).
dnl
dnl To construct the yices library (libyices.so) with GMP included,
dnl we need a version of libgmp.a that's position independent code.
dnl On systems where -fPIC is not the default, we must build
dnl a specific version of GMP from the source, using option CFLAG=-fPIC.
dnl
dnl To build dynamic executables and libraries without GMP included,
dnl we need a a dynamic version of the GMP library (e.g., /usr/local/lib/libgmp.so).
dnl
dnl On mingw and cygwin, dynamic and static GMP libraries do not coexist well.
dnl They must be installed in different locations and use different gmp.h files.
dnl
dnl To deal with this, the build Makefiles use the following variables:
dnl   STATIC_GMP = full path to a static version of gmp (e.g., /usr/local/lib/libgmp.a)
dnl   STATIC_GMP_INCLUDE_DIR = directory where a 'gmp.h' compatible with STATIC_GMP is located
dnl                            (or empty if 'gmp.h' is in a standard place)
dnl
dnl   PIC_GMP = full path to a libgmp.a that's PIC code
dnl   PIC_GMP_INCLUDE_DIR = directory where a 'gmp.h' header file compatible with PIC_GMP
dnl                         is located
dnl
dnl By default, the configure script searches for a usable STATIC_GMP in default
dnl locations and sets PIC_GMP to the same thing as STATIC_GMP.
dnl
dnl If the configure script does not find STATIC_GMP or if the defaults are
dnl incorrect, the right libraries can be given as options to ./configure:
dnl
dnl --with-static-gmp=full-path-to-libgmp.a for static linking
dnl --with-static-gmp-include-dir=corresponding directory for gmp.h
dnl
dnl --with-pic-gmp=full-path-to-relocatable-libgmp.a
dnl --with-pic-gmp-include-dir=directory where the corresponding gmp.h resides
dnl
static_libgmp=""
AC_ARG_WITH([static-gmp],
   [AS_HELP_STRING([--with-static-gmp=<path>],[Full path to a static GMP library (e.g., libgmp.a)])],
   [if test "x$withval" == x; then
      AC_MSG_WARN([--with-static-gmp was used but no path was given. Using defaults])
    else
      static_libgmp=$withval
    fi
   ],
   [])

static_includegmp=""
AC_ARG_WITH([static-gmp-include-dir],
   [AS_HELP_STRING([--with-static-gmp-include-dir=<directory>],
            [Directory of include file "gmp.h" compatible with static GMP library])],
   [if test "x$withval" == x; then
      AC_MSG_WARN([--with-static-gmp-include-dir was used but no directory was given. Using defaults])
    else
      static_includegmp=$withval
    fi
   ],
   [])


pic_libgmp=""
AC_ARG_WITH([pic-gmp],
   [AS_HELP_STRING([--with-pic-gmp=<path>],[Full path to a relocatable GMP library (e.g., libgmp.a)])],
   [if test "x$withval" == x; then
      AC_MSG_WARN([--with-pic-gmp was used but no path was given. Using defaults])
    else
      pic_libgmp=$withval
    fi
   ],
   [])

pic_includegmp=""
AC_ARG_WITH([pic-gmp-include-dir],
   [AS_HELP_STRING([--with-pic-gmp-include-dir=<directory>],
            [Directory of include file "gmp.h" compatible with relocatable GMP library])],
   [if test "x$withval" == x; then
      AC_MSG_WARN([--with-pic-gmp-include-dir was used but no directory was given. Using defaults])
    else
      pic_includegmp=$withval
    fi
   ],
   [])


dnl
dnl Options for mcsat and the libpoly library
dnl -----------------------------------------
dnl --enable-mcsat: add support for MCSAT 
dnl
dnl If mcsat is emabled, we provide options to specify
dnl the static libraries, using the same conventions as for GMP.
dnl
use_mcsat="no"
AC_ARG_ENABLE([mcsat],
   [AS_HELP_STRING([--enable-mcsat],[Enable support for MCSAT. This requires the libpoly library.])],
   [if test "$enableval" = yes ; then 
      use_mcsat="yes"
      AC_MSG_NOTICE([Enabling support for MCSAT])
      MCSAT_CPPFLAGS+= -DHAVE_MCSAT
      AC_SUBST(MCSAT_CPPFLAGS)
    fi],
   [])


static_lpoly=""
AC_ARG_WITH([static-libpoly],
   [AS_HELP_STRING([--with-static-libpoly=<path>],[Full path to libpoly.a])],
   [if test $use_mcsat == "no" ; then
      AC_MSG_WARN([Ignoring option --with-static-libpoly since MCSAT support is disabled])
    else 
      if test "x$withval" == x; then
        AC_MSG_WARN([--with-static-poly was used but no path was given. Using defaults])
      else
        static_lpoly=$withval
      fi
    fi
   ],
   [])

static_includelpoly=""
AC_ARG_WITH([static-libpoly-include-dir],
   [AS_HELP_STRING([--with-static-libpoly-include-dir=<directory>],
            [Path to include files compatible with libpoly.a (e.g., /usr/local/include)])],
   [if test $use_mcsat == "no" ; then
      AC_MSG_WARN([Ignoring option --with-static-libpoly-include-dir since MCSAT support is disabled])
    else 
      if test "x$withval" == x; then
         AC_MSG_WARN([--with-static-libpoly-include-dir was used but no directory was given. Using defaults])
      else
        static_includelpoly=$withval
      fi
    fi
   ],
   [])

pic_lpoly=""
AC_ARG_WITH([pic-libpoly],
   [AS_HELP_STRING([--with-pic-libpoly=<path>],[Full path to a relocatable libpoly.a])],
   [if test $use_mcsat == "no" ; then
      AC_MSG_WARN([Ignoring option --with-pic-libpoly since MCSAT support is disabled])
    else 
      if test "x$withval" == x; then
         AC_MSG_WARN([--with-pic-libpoly was used but no path was given. Using defaults])
      else
         pic_lpoly=$withval
      fi
    fi
   ],
   [])

pic_includelpoly=""
AC_ARG_WITH([pic-libpoly-include-dir],
   [AS_HELP_STRING([--with-pic-libpoly-include-dir=<directory>],
            [Path to include files compatible with the relocatable libpoly.a])],
   [if test $use_mcsat == "no" ; then
      AC_MSG_WARN([Ignoring option --with-pic-libpoly-include-dir since MCSAT support is disabled])
    else
      if test "x$withval" == x; then
         AC_MSG_WARN([--with-pic-libpoly-include-dir was used but no directory was given. Using defaults])
      else
         pic_includelpoly=$withval
      fi
    fi
   ],
   [])

#
# Options for forcing the use of shared libraries.
#
# By default, configure will search for a static version of each library. If
# the user wants to disable this behaviour, he can use these.
#
AC_ARG_WITH([shared-gmp],[AS_HELP_STRING([--with-shared-gmp],
      [By default, a static version of the GMP library will be searched.
        This option forces the use of the shared version.
        This applies for both shared and static libraries.])],
      [FORCE_SHARED_GMP=yes],
      [FORCE_SHARED_GMP=no])

#
# CHECK LIBRARIES
# ---------------
#
AC_LANG([C])

#
# The PIC (position-independent code) property for libgmp.a is required if we
# want to link it statically to the shared yices library. PIC is not required
# to produce the static yices library.
# All code is PIC by default on mingw/cygwin/etc. So we can skip the test for
# those $host_os.
# NOTE: We must search for the static versions first since other tests may
# modify $LIBS and $CPPFLAGS.
STATIC_GMP=""
STATIC_GMP_INCLUDE_DIR=""
PIC_GMP=""
PIC_GMP_INCLUDE_DIR=""
case $host_os,$FORCE_SHARED_GMP in
  *,yes)
    AC_MSG_NOTICE([force using an installed gmp shared library with --with-shared-gmp])
    # Do not check GMP static libraries. The library will be checked later.
    # Nevertheless, we must assure that gmp.h can be found right now.
    AC_CHECK_HEADERS([gmp.h],[],[AC_MSG_ERROR([gmp.h is needed. Try setting CPPFLAGS.])])
  ;;
  openbsd* | freebsd* | linux* | sunos* | darwin*)
    CSL_CHECK_STATIC_GMP($static_libgmp,$static_includegmp)
    # Now check that the
    # The function CSL_CHECK_STATIC_GMP_HAS_PIC needs ./libtool;
    # usually, it would be created at the end of ./configure. LT_OUTPUT forces
    # the creation of ./libtool for using it inside ./configure.
    LT_OUTPUT
    CSL_CHECK_STATIC_GMP_HAS_PIC(STATIC_GMP_HAS_PIC,$STATIC_GMP,$STATIC_GMP_INCLUDE_DIR)

    # If STATIC_GMP is PIC, set PIC_GMP to STATIC_GMP as a default value that
    # can be overwritten by --with-pic-gmp
    if test "$STATIC_GMP_HAS_PIC" = yes; then
      PIC_GMP=$STATIC_GMP
      PIC_GMP_INCLUDE_DIR=$STATIC_GMP_INCLUDE_DIR
    fi

    # If shared library is enabled and --with-pic-gmp is given, test that
    # it is in fact PIC.
    if test $enable_shared = yes -a "x$pic_libgmp" != x; then
      CSL_CHECK_HEADER([gmp.h],[$pic_includegmp "$(dirname $pic_libgmp)" "$(dirname $pic_libgmp)/../include"],--with-pic-gmp-include-dir)
      CSL_CHECK_LIB_PATH($pic_libgmp,--with-pic-gmp)
      CSL_CHECK_GMP($testlib,$testincludedir)
      if test $run_ok != yes; then
        AC_MSG_ERROR([*** $testlib does not appear to be usable: check option --with-pic-gmp ***])
      fi
      CSL_CHECK_STATIC_GMP_HAS_PIC(PIC_GMP_HAS_PIC,$testlib,$testincludedir)
      if test "$PIC_GMP_HAS_PIC" != yes; then
        AC_MSG_ERROR([
*** $testlib does not appear to be PIC
*** Most package manager install non-PIC libgmp.a (apt, homebrew...).
*** You can simply remove --with-pic-gmp to use shared gmp or download and build gmp manually:
*** To build libgmp.a with PIC: ./configure --with-pic --host=$host --prefix=\$PWD/installdir
*** Then, give me the option: --with-pic-gmp=/path_to_installdir/lib/libgmp.a])
      fi
      PIC_GMP=$testlib
      PIC_GMP_INCLUDE_DIR=$testincludedir
    fi
    ;;
  mingw* | cygwin*)
    CSL_CHECK_STATIC_GMP($static_libgmp,$static_includegmp)
    PIC_GMP=$STATIC_GMP
    PIC_GMP_INCLUDE_DIR=$STATIC_GMP_INCLUDE_DIR
    STATIC_GMP_HAS_PIC=yes
    PIC_GMP_HAS_PIC=yes
    ;;
esac
AC_SUBST(STATIC_GMP)
AC_SUBST(STATIC_GMP_INCLUDE_DIR)
AC_SUBST(STATIC_GMP_HAS_PIC)
AC_SUBST(PIC_GMP)
AC_SUBST(PIC_GMP_INCLUDE_DIR)

#
# MCSAT SUPPORT
# -------------
ENABLE_MCSAT="$use_mcsat"
AC_SUBST(ENABLE_MCSAT)

#
# libpoly.a
#
STATIC_LIBPOLY=""
STATIC_LIBPOLY_INCLUDE_DIR=""
AC_SUBST(STATIC_LIBPOLY)
AC_SUBST(STATIC_LIBPOLY_INCLUDE_DIR)
if test $use_mcsat = yes ; then
  if test "x$STATIC_GMP" = x ; then
    auxlibs="-lgmp"
  else
    auxlibs="$STATIC_GMP"
  fi
  CSL_CHECK_STATIC_LIBPOLY($static_lpoly,$static_includelpoly,$auxlibs)
fi

#
# Position-independent static version of LIBPOLY
#
PIC_LIBPOLY="$STATIC_LIBPOLY"
PIB_LIBPOLY_INCLUDE_DIR="$STATIC_LIBPOLY_INCLUDE_DIR"
AC_SUBST(PIC_LIBPOLY)
AC_SUBST(PIC_LIBPOLY_INCLUDE_DIR)
if test $use_mcsat = yes ; then
  if test "x$pic_lpoly" != x ; then
     CSL_CHECK_HEADER([poly/polynomial.h],[$pic_includelpoly "$(dirname $pic_lpoly)" "$(dirname $pic_lpoly)/../include"],--with-pic-libpoly-include-dir)
     CSL_CHECK_LIB_PATH($pic_lpoly,--with-pic-lpoly)
     if test "x$PIC_GMP" != x ; then
        auxlibs="$PIC_GMP"
     else
       if test "x$STATIC_GMP" != x ; then 
         auxlibs="$STATIC_GMP"
       else
         auxlibs="-lgmp"
       fi
     fi
     CSL_CHECK_STATIC_LIBPOLY($pic_lpoly,$pic_includelpoly,$auxlibs)
     if test $run_ok = yes; then
       PIC_LIBPOLY=$testlib
       PIC_LIBPOLY_INCLUDE_DIR=$testincludedir
     else
       AC_MSG_ERROR([*** $testlib does not appear to be usable: check option --with-pic-libpoly ***])
     fi
  fi
fi

#
# Default libpoly
#
if test $use_mcsat = yes ; then
   AC_CHECK_LIB([poly],[lp_polynomial_new], [], [AC_MSG_ERROR([*** libpoly library not found. Try to set LDFLAGS ***])])
fi

#
# Fix MKDIR_P to an absolute path if it's set to './install-sh -c -d'
# because the Makefiles import its definition via ./configs/make.include.
# This may be unreliable (it won't handle spaces in directory names).
#
# case "$MKDIR_P" in
#   ./install-sh*)
#      curdir=`pwd`
#      new_mkdir_p="$curdir/$MKDIR_P"
#     AC_MSG_WARN([*** MKDIR_P is relative: converting $MKDIR_P to $new_mkdir_p ***])
#     MKDIR_P=$new_mkdir_p
#     ;;
# esac
#

#
#
# Compilation flags (LIB_CPPFLAGS, LIB_LDFLAGS...)

#
# OS-dependent compilation flags + which dynamic libraries to build
#    libyices_dynamic = dynamic library for make lib
#    static_libyices_dynamic = dynamic library for make static-lib
#
# -fPIC: PIC is the default on Darwin/Cygwin/Mingw (and causes
#  compilation warning on the latter two if present). This flag
#  is automatically added by libtool using --enable-shared and
#  --enable-static.
#
# STATIC contains any
#
case $host_os in
  cygwin*)
    CPPFLAGS+=" -DCYGWIN"

    # The flag -static-libstd is necessary because of standard
    # symbols like getc_unlocked that must be statically linked.
    CFLAGS+=" -Wl,--stack,8388608 -static-libgcc"
    ;;
  mingw*)
    CPPFLAGS+=" -DMINGW -D__USE_MINGW_ANSI_STDIO"
    CFLAGS+=" -Wl,--stack,8388608 -static-libgcc"

    # Format string issues on Windows/mingw
    CFLAGS+=" -Wno-format"
    ;;
  darwin*)
    CPPFLAGS+=" -DMACOSX"
    CFLAGS+=" -fvisibility=hidden"
    ;;
  sunos*)
    CPPFLAGS+=" -DSOLARIS"
    CFLAGS+=" -fvisibility=hidden"
    ;;
  linux*)
    CPPFLAGS+=" -DLINUX -U_FORTIFY_SOURCE"
    CFLAGS+=" -fvisibility=hidden"
    ;;
  freebsd*)
    CPPFLAGS+=" -DFREEBSD"
    CFLAGS+=" -fvisibility=hidden"
    ;;
  unix*)
    CPPFLAGS+=" -DLINUX"
    CFLAGS+=" -fvisibility=hidden"
    ;;
  *)
    AC_MSG_ERROR([Don't know how to compile on $host_os])
  ;;
esac
AC_SUBST(libyices_dynamic)
AC_SUBST(static_libyices_dynamic)

#
# Warning levels: dropped the flag -Winiline in all build modes.
# Can't keep up with gcc/clang.
#
CFLAGS+=" -Wall -Wredundant-decls"


#
# Check build mode (stored in MODE)
#
AC_ARG_WITH([mode],
   [AS_HELP_STRING([--with-mode=MODE],[The mode used during compilation/distribution.
      It can be one of release, debug, devel, profile, gcov, valgrind, purify,
      quantify or gperftools. (default: release)])],
   [
    case $withval in
      release|debug|devel|profile|gcov|valgrind|purify|quantify|gperftools)
        AC_MSG_NOTICE([Build mode selected: $withval])
        YICES_MODE=$withval
        ;;
      "")
        AC_MSG_WARN([--with-mode was given without any mode. Defaulting to 'release'.])
        YICES_MODE=release
        ;;
      *)
        AC_MSG_ERROR([Invalid build mode: $withval])
        ;;
    esac
    AC_SUBST([YICES_MODE])
   ],
   [YICES_MODE=release
    AC_MSG_NOTICE([Build mode selected by default: $YICES_MODE])]
)


#
# Compilation flags dependent on MODE
#
# Option -fomit-frame-pointer confuses the Mac OS profiling tools
# (don't use it if MODE=profile).
#
# Option -fno-stack-protector is useful on Ubuntu (and probably other
# Linux distributions). On these distributions, gcc has
# -fstack-protector enabled by default. This can cause link-time
# errors on the user's systems:
#    undefined reference to '__stack_chk_fail'.
# However this option is not supported by older versions of GCC (before gcc-4.1?)
#
# Update: 02/16/2012: we check whether -fno-stack-protector is supported
# in the configure script. The script sets variable $NO_STACK_PROTECTOR
# properly.
#
# Related issue reported by Richard Corden (January 18, 2016)
# Ubuntu sets -D_FORTIFY_SOURCE=2 by default (to detect buffer overflows
# and other security issues). This causes link-time issues again:
#   undefined reference to __longjmp_chk.
# The fix is to add -U_FORTIFY_SOURCE when we compile on linux
#
# To link with google gperftools:
#  on Linux we add the flag -Wl,--no-as-needed otherwise the profiler
#  library may not be linked with the binaries. (Because some
#  Linux distributions set -Wl,--as-needed by default).
#
#
case $YICES_MODE in
  release)
    CFLAGS+=" -O3 -fomit-frame-pointer $NO_STACK_PROTECTOR"
    CPPFLAGS+=" -DNDEBUG"
    ;;
  devel)
    CFLAGS+=" -O3 -g -fomit-frame-pointer $NO_STACK_PROTECTOR"
    ;;
  profile)
    CFLAGS+=" -O3 -pg"
    CPPFLAGS+=" -DNDEBUG"
    ;;
  gcov)
    CFLAGS+=" -fprofile-arcs -ftest-coverage"
    CPPFLAGS+=" -DNDEBUG"
    ;;
  valgrind|quantify|purify)
    CFLAGS+=" -O3 -g"
    CPPFLAGS+=" -DNDEBUG"
    ;;
  gperftools)
    CFLAGS+=" -O3 -g"
    CPPFLAGS+=" -DNDEBUG"
    case $host_os in
      linux) LIBS+= -Wl,--no-as-needed -lprofiler ;;
      *) LIBS+=" -lprofiler" ;;
    esac
    ;;
  # debug mode
  *)
    CFLAGS+=" -g"
  ;;
esac

#
# Link command for purify/quantify
#
case $host_os in
  sunos)
    case $YICES_MODE in
      purify) LNK=purify $CC ;;
      quantify) LNK=quantify $CC ;;
      *) LNK=$CC ;;
    esac
    ;;
  *)
    LNK=$CC
    ;;
esac
AC_SUBST([LNK])

#
# More CPPFLAGS for compiling static objects
#
if test "x$STATIC_GMP_INCLUDE_DIR" != x; then
  CPPFLAGS+=" -I$STATIC_GMP_INCLUDE_DIR"
fi
if test "x$STATIC_LIBPOLY_INCLUDE_DIR" != x; then
  CPPFLAGS+=" -I$STATIC_LIBPOLY_INCLUDE_DIR"
fi

dnl AC_CHECK_HEADERS([poly/poly.h], [], [AC_MSG_ERROR([*** libpoly headers not found. Try to set CPPFLAGS ***])])

#
# Set the right extension to binary files (nothing on Unix, .exe on Windows)
#
case $host_os in
  mingw*) EXEEXT=".exe";;
esac
AC_SUBST(EXEEXT)

HAS_STATIC_GMP=$(test x"${STATIC_GMP}" = "x" && echo no || echo yes)
HAS_PIC_GMP=$(test x"${PIC_GMP}" = "x" && echo no || echo yes)
AC_SUBST(HAS_STATIC_GMP)
AC_SUBST(HAS_PIC_GMP)

#
# Try to find -lgmp if no static libgmp.a has been given or found.
#
# - for the static library, if no libgmp.a has been given (can be PIC or
#   non-PIC, the only difference is that PIC may be slower), we must default
#   to using -lgmp. In this case, check for its presence.
# - for the shared library, if no PIC has been given, default to -lgmp.
#
# Save $LIBS because AC_CHECK_LIB will alter it and we want different LIBS
# for static and shared mode.
previous_libs=$LIBS
AC_CHECK_LIB(gmp, __gmpz_cmp, [HAS_SHARED_GMP=yes],[HAS_SHARED_GMP=no])
AC_SUBST(HAS_SHARED_GMP)
LIBS=$previous_libs

STATIC_YICES_USES_SHARED_GMP=no
if test $enable_static = yes -a "$HAS_STATIC_GMP" != yes; then
  if test "$HAS_SHARED_GMP" != yes; then
    AC_MSG_ERROR([*** You have --enable-static but no static or shared libgmp found.])
    AC_MSG_ERROR([*** Try giving --with-static-gmp= or setting LDFLAGS.])
  else
    STATIC_LIBS+=" -lgmp"
    STATIC_YICES_USES_SHARED_GMP=yes
  fi
fi
AC_SUBST(STATIC_LIBS)
AC_SUBST(STATIC_YICES_USES_SHARED_GMP)

SHARED_YICES_USES_SHARED_GMP=no
if test $enable_shared = yes -a "$HAS_PIC_GMP" != yes; then
  if test "$HAS_SHARED_GMP" != yes; then
    AC_MSG_ERROR([*** You have --enable-shared but no static (with PIC enabled) or shared libgmp found.])
    AC_MSG_ERROR([*** Try giving --with-pic-gmp= or setting LDFLAGS.])
  else
    SHARED_LIBS+=" -lgmp"
    SHARED_YICES_USES_SHARED_GMP=yes
  fi
fi
AC_SUBST(SHARED_LIBS)
AC_SUBST(SHARED_YICES_USES_SHARED_GMP)

#
# Just a warning message when static and shared libs are built simultanously
# on windows. The static library will be named libyices_static.a instead of
# libyices.a.
#
case $host_os--static_$enable_static--shared_$enable_shared in
  mingw*--static_yes--shared_yes |\
  cygwin*--static_yes--shared_yes)
    AC_ARG_WITH([static-name],[AS_HELP_STRING([--with-static-name=name],
      [(Windows only) when building simultanously shared and static libraries,
      allows you to give a different name for the static version of libyices.a.])],
      [if test "x$withval" == x; then
        libyices_a=libyices_static.a
        AC_MSG_WARN([--with-static-name was used but no name was given. Using default $libyices_a])
      else
        libyices_a=$withval
      fi],
      [libyices_a=libyices_static.a])
    AC_MSG_WARN([*** On mingw/cygwin builds, if static and shared libs are built simultanously,])
    AC_MSG_WARN([*** the static library will be named $libyices_a to avoid clash with the dll.])
    ;;
  *)
    libyices_a=libyices.a
    ;;
esac
AC_SUBST(libyices_a)

#
# On Windows, we must set -DNOYICES_DLL and -D__GMP_LIBGMP_DLL depending on
# the cases.
#
case $host_os--static_$enable_static--staticgmp_$HAS_STATIC_GMP in
  mingw*--static_yes--staticgmp_yes |\
  cygwin*--static_yes--staticgmp_yes)
    STATIC_CPPFLAGS+=" -DNOYICES_DLL -D__GMP_LIBGMP_DLL=0"
  ;;
  mingw*--static_yes--staticgmp_no |\
  cygwin*--static_yes--staticgmp_no)
    STATIC_CPPFLAGS+=" -DNOYICES_DLL -D__GMP_LIBGMP_DLL=1"
  ;;
esac
case $host_os--shared_$enable_shared--picgmp_$HAS_PIC_GMP in
  mingw*--shared_yes--picgmp_yes |\
  cygwin*--shared_yes--picgmp_yes)
    SHARED_CPPFLAGS+=" -D__GMP_LIBGMP_DLL=0"
  ;;
  mingw*--shared_yes--picgmp_no |\
  cygwin*--shared_yes--picgmp_no)
    SHARED_CPPFLAGS+=" -D__GMP_LIBGMP_DLL=1"
  ;;
esac
AC_SUBST(STATIC_CPPFLAGS)
AC_SUBST(SHARED_CPPFLAGS)

#
# On windows, if shared or static library is asked but no static GMP has been
# given, we must check that the libgmp.dll is present somewhere in the PATH.
#
case $host_os in mingw* | cygwin*)
  if test "$STATIC_YICES_USES_SHARED_GMP" = yes -o "$SHARED_YICES_USES_SHARED_GMP" = yes; then
    AC_MSG_CHECKING([for libgmp-10.dll])
    AM_PATH_PROG_WITH_TEST([HAS_SHARED_GMP_DLL], [libgmp],
      [{ basedir=`echo "$ac_dir" | sed -e 's,/bin$,,'`; test -r "$basedir"/lib/libgmp-10.dll; }])
    if test "x$HAS_SHARED_GMP_DLL" = x; then
      AC_MSG_RESULT([not found in PATH])
      AC_MSG_ERROR([Required library DLL 'libgmp-10.dll' not found; try to set the PATH. Also, no static version of 'libgmp.a' has been given/found.])
    else
      AC_MSG_RESULT([found in PATH])
    fi
    AC_MSG_ERROR([*** please make sure that libgmp-10.dll is in your PATH or give a libgmp.a with --with-static-gmp ***])
  fi ;;
esac

# Note that AC_SUBST can be anywhere but must be after the
# last affectation of the variable.
AC_SUBST(CFLAGS)
AC_SUBST(CPPFLAGS)
AC_SUBST(LDFLAGS)
AC_SUBST(STATIC_GMP)
AC_SUBST(STATIC_GMP_HAS_PIC)
AC_SUBST(STATIC_GMP_INCLUDE_DIR)
AC_SUBST(STATIC_GMP_LIB_DIR)
AC_SUBST(HAS_STATIC_GMP)
AC_SUBST(STATIC_LIBPOLY)
AC_SUBST(STATIC_LIBPOLY_INCLUDE_DIR)


# Store config data in make.include and output (= replace the AC_SUBST)
AC_CONFIG_FILES([make.include])
AC_OUTPUT

summary="EXEEXT:                     $EXEEXT
SED:                        $SED
LN_S:                       $LN_S
MKDIR_P:                    $MKDIR_P
CC:                         $CC
LD:                         $LD
AR:                         $AR
RANLIB:                     $RANLIB
STRIP:                      $STRIP
GPERF:                      $GPERF
NO_STACK_PROTECTOR:         $NO_STACK_PROTECTOR
STATIC_GMP:                 $STATIC_GMP
STATIC_GMP_INCLUDE_DIR:     $STATIC_GMP_INCLUDE_DIR
PIC_GMP:                    $PIC_GMP
PIC_GMP_INCLUDE_DIR:        $PIC_GMP_INCLUDE_DIR
ENABLE_MCSAT:               $ENABLE_MCSAT
STATIC_LIBPOLY:             $STATIC_LIBPOLY
STATIC_LIBPOLY_INCLUDE_DIR: $STATIC_LIBPOLY_INCLUDE_DIR
PIC_LIBPOLY:                $PIC_LIBPOLY
PIC_LIBPOLY_INCLUDE_DIR:    $PIC_LIBPOLY_INCLUDE_DIR

Version:                    $PACKAGE_STRING
Host type:                  $host
Install prefix:             $prefix
Build mode:                 $YICES_MODE

For both static and shared library:
  CPPFLAGS:                 $CPPFLAGS
  CFLAGS:                   $CFLAGS
  LDFLAGS:                  $LDFLAGS

For static library          $libyices_a:
  Enable:                   $enable_static
  STATIC_CPPFLAGS:          $STATIC_CPPFLAGS
  STATIC_LIBS:              $STATIC_LIBS
  Libgmp.a found:           $HAS_STATIC_GMP
  Libgmp.a path:            $STATIC_GMP
  Libgmp.a is pic:          $STATIC_GMP_HAS_PIC (non-PIC is faster for the static library)
  Use shared gmp instead of libgmp.a:  $STATIC_YICES_USES_SHARED_GMP

For shared library:
  Enable:                   $enable_shared
  SHARED_CPPFLAGS:          $SHARED_CPPFLAGS
  SHARED_LIBS:              $SHARED_LIBS
  Libgmp.a with PIC found:  $HAS_PIC_GMP
  Libgmp.a path:            $PIC_GMP
  Use shared gmp instead of libgmp.a: $SHARED_YICES_USES_SHARED_GMP"

AC_MSG_NOTICE([Summary of the configuration:
$summary])
echo "$summary" > summary