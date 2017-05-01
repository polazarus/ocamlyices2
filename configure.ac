AC_INIT(yices2,0.1.0)

: ${CFLAGS="-O2 -Wall -Wextra -Wconversion"}
AC_PROG_CC

AC_CHECK_LIB(gmp, __gmpz_init, [],
             [AC_MSG_ERROR(GMP required, see https://gmplib.org)])

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
  AC_MSG_ERROR("'ocamlfind' not configured correctly (cannot run 'ocamlc')")
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

AC_SUBST([ANNOTFLAG])
AC_SUBST([BIN_ANNOT])
AC_SUBST([HAVE_OCAMLOPT])
AC_SUBST([HAVE_OCAMLDOC])
AC_SUBST([HAVE_OUNIT])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_FILES([Makefile.config META opam])

AC_OUTPUT
