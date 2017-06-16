AC_INIT(yices2,0.0.3)
AC_CONFIG_AUX_DIR([autoconf])
m4_include([ext/yices/autoconf/m4/csl_check_libs.m4]) # for CSL_STATIC_CHECK_GMP, CSL_CHECK_STATIC_LIBPOLY, etc...
AC_CANONICAL_HOST

: ${CFLAGS="-O2 -Wall -Wextra -Wconversion"}
AC_PROG_CC

AC_CHECK_HEADER(gmp.h, [],
                [AC_MSG_ERROR(GMP headers required)])

AC_CHECK_FUNC(fopencookie, [
  AC_DEFINE([HAVE_FOPENCOOKIE],[1],[Define if fopencookie available])
  HAVE_FOPENCOOKIE=yes
])

AC_CHECK_FUNC(funopen, [
  AC_DEFINE([HAVE_FUNOPEN], [1],[Define if funopen available])
  HAVE_FUNOPEN=yes
])

AS_IF([test "x$HAVE_FUNOPEN$HAVE_FOPENCOOKIE" = x], [
  AC_MSG_WARN(Your system support funopen or fopencookie: print functions not supported!)
])

AC_CHECK_PROG([OCAMLFIND], [ocamlfind], [ocamlfind])
AS_IF([test "x$OCAMLFIND" = x], AC_MSG_ERROR(Cannot find ocamlfind.))

AC_MSG_CHECKING([for ocamlfind ocamlc])
AS_IF([$OCAMLFIND ocamlc 1> /dev/null 2>&1], [
  AC_MSG_RESULT(yes)
], [
  AC_MSG_RESULT(no)
  AC_MSG_ERROR('ocamlfind' not configured correctly (cannot run 'ocamlc'))
])

AC_MSG_CHECKING([for ocamlfind ocamlopt])
AS_IF([$OCAMLFIND ocamlopt 1> /dev/null 2>&1], [
  AC_MSG_RESULT(yes)
  HAVE_OCAMLOPT=yes
], [
  AC_MSG_RESULT(no)
  AC_MSG_WARN("'ocamlopt' not installed or not configured in findlib.conf")
])

AC_MSG_CHECKING([for ocamlfind ocamldoc])
AS_IF([$OCAMLFIND ocamldoc 1> /dev/null 2>&1], [
  AC_MSG_RESULT(yes)
  HAVE_OCAMLDOC=yes
], [
  AC_MSG_RESULT(no)
  AC_MSG_WARN("'ocamldoc' not installed or not configured in findlib.conf")
])

AC_MSG_CHECKING([OCaml version])
OCAMLVERSION=`$OCAMLFIND ocamlc -version 2>/dev/null`
AC_MSG_RESULT([$OCAMLVERSION])
AS_VERSION_COMPARE($OCAMLVERSION, 3.12,
  AC_MSG_ERROR([Please update to a newer version of ocaml.]))

AC_MSG_CHECKING([for OCaml library zarith])
AS_IF([$OCAMLFIND query zarith 1> /dev/null 2>&1], [
  AC_MSG_RESULT(yes)
], [
  AC_MSG_RESULT(no)
  AC_MSG_ERROR([Cannot find zarith with 'ocamlfind'. Please install zarith.])
])

# OUnit is only needed for testing.
AC_MSG_CHECKING([for OCaml library ounit])
AS_IF([$OCAMLFIND query oUnit 1> /dev/null 2>&1], [
  AC_MSG_RESULT(yes)
  HAVE_OUNIT=yes
], [
  AC_MSG_RESULT(no)
])

AC_MSG_CHECKING([for OCaml binary annotations (.cmt, .cmti)])
AS_IF([$OCAMLFIND ocamlc -bin-annot 1> /dev/null 2>&1], [
  AC_MSG_RESULT([yes])
  BIN_ANNOT='-bin-annot'
  ANNOTFLAG="-bin-annot -annot"
],[
  AC_MSG_RESULT([no])
  ANNOTFLAG="-annot"
])

AC_ARG_WITH([static-gmp],
   [AS_HELP_STRING([--with-static-gmp=<path>],[Full path to a static GMP library
      (e.g., libgmp.a). This static GMP will be used for zarith and for building
      yices with gmp statically linked to libgmp.a. Without this option, -lgmp will be used.])],
   [if test "x$withval" == x; then
      AC_MSG_WARN([--with-static-gmp was used but no path was given. Using system libgmp.a or shared lib if not found.])
    else
      static_libgmp=$withval
    fi
   ],
   [])

#
# Options for forcing the use of shared gmp.
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
AC_SUBST(FORCE_SHARED_GMP)

if test $FORCE_SHARED_GMP = yes; then
  AC_MSG_NOTICE([use gmp shared library (--with-shared-gmp)])
else
  AC_MSG_NOTICE([trying to use static libgmp.a (to disable, use --with-shared-gmp)])
  CSL_CHECK_STATIC_GMP([$static_libgmp],[])
fi

AC_ARG_WITH([gmp-embedded],[AS_HELP_STRING([--without-gmp-embedded],
      [By default, the static library libyices.a
      created will be partially linked (ld -r) so that the GMP library is not
      needed afterwards (i.e., only -lyices is needed). If you want to disable
      the partial linking (and thus -lgmp and -lyices will be needed), you
      can use this flag. Enable this flag if your system does not like the
      `ld -r` option.])],
      [GMP_EMBEDDED=no],
      [GMP_EMBEDDED=yes])
AC_SUBST(GMP_EMBEDDED)

if test "x$STATIC_GMP" = x; then
  AC_CHECK_LIB(gmp, __gmpz_init, [],
             [AC_MSG_ERROR(GMP required, see https://gmplib.org)])
fi

# -fPIC is not necessary for mingw and cygwin because on windows, every
# object is position-indenpendant code. For other hosts, use -fPIC.
# The flag -DNOYICES_DLL disables the 'dll' mode on windows and allows to
# create a static library.
# -DCYGWIN and -DMINGW tell to the yices.h header to pick the right fonction
# calls (for getc()) that are specific to windows.
case $host_os in
  mingw*)
    CPPFLAGS+=" -DNOYICES_DLL -DMINGW"
    ;;
  cygwin*)
    CPPFLAGS+=" -DNOYICES_DLL -DCYGWIN"
    ;;
  darwin*)
    CPPFLAGS+=" -DMACOSX"
    ;;
  sunos*)
    CPPFLAGS+=" -DSOLARIS"
    ;;
  linux*)
    CPPFLAGS+=" -DLINUX -U_FORTIFY_SOURCE"
    ;;
  freebsd*)
    CPPFLAGS+=" -DFREEBSD"
    ;;
  unix*)
    CPPFLAGS+=" -DLINUX"
    ;;
  *)
    AC_MSG_ERROR([Don't know how to compile on $host_os])
    ;;
esac

case $host_os in
  mingw* | cygwin*)
    SO=dll
  ;;
  linux* | *bsd* | unix* | sunos* | darwin*)
    SO=so
  ;;
esac
AC_SUBST(SO)

AC_SUBST([ANNOTFLAG])
AC_SUBST([BIN_ANNOT])
AC_SUBST([HAVE_OCAMLOPT])
AC_SUBST([HAVE_OCAMLDOC])
AC_SUBST([HAVE_OUNIT])
AC_SUBST(CPPFLAGS)
AC_SUBST(CFLAGS)
AC_SUBST(LDFLAGS)
AC_SUBST(STATIC_GMP)
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_FILES([Makefile.config META opam])

AC_OUTPUT

AC_MSG_NOTICE([summary:
  Host:                             $host
  Static gmp library:               $STATIC_GMP
  Use shared gmp instead of static: $FORCE_SHARED_GMP
  Embed gmp inside libyices.a:      $GMP_EMBEDDED])