AC_INIT(ocamlyices2,0.1.0)

AC_PROG_CC


AC_CHECK_LIB(stdc++,main,[LIBS="$LIBS -lstdc++"],[
  AC_MSG_WARN([libstdc++ required with Yices with GMP statically linked])
])

AC_CHECK_HEADER(gmp.h,[
  AC_DEFINE([HAVE_GMP_H], [1],[Define if gmp.h available])
],[
  AC_MSG_WARN([No gmp.h available. Disable get_integer_value_as_string and get_rational_value_as_string.])
])

AC_CHECK_FUNC(fopencookie,
  AC_DEFINE([HAVE_FOPENCOOKIE],[1],[Define if fopencookie available]),
  AC_MSG_WARN(fopencookie not found))

AC_CHECK_FUNC(funopen, [
  AC_DEFINE([HAVE_FUNOPEN], [1],[Define if funopen available])
  AC_MSG_WARN(Your system support funopen. Consider helping us to get more efficient pp_* functions!)
])

AC_CHECK_PROG([OCAMLC], [ocamlc], [ocamlc])
AS_IF([test "x$OCAMLC" = x], AC_MSG_ERROR(Cannot find ocamlc.))

AC_MSG_CHECKING([ocamlc version])
OCAMLVERSION=`$OCAMLC -version`
AC_MSG_RESULT([$OCAMLVERSION])

AS_VERSION_COMPARE($OCAMLVERSION, 3.12,
  AC_MSG_ERROR([Please update to a newer version of ocaml.]))

AC_MSG_CHECKING([ocamlc binary annotations])
AS_IF([ocamlc -bin-annot > /dev/null], [
  AC_MSG_RESULT([yes])
  ANNOTFLAG="-bin-annot -annot"
],[
  AC_MSG_RESULT([no])
  ANNOTFLAG="-annot"
])

AC_CHECK_PROG([OCAMLDOC], [ocamldoc], [ocamldoc])
AS_IF([test "x$OCAMLDOC" = x], AC_MSG_WARN([Cannot find ocamldoc.]))

AC_CHECK_PROG([OCAMLFIND], [ocamlfind], [ocamlfind])
AS_IF([test "x$OCAMLFIND" = x], AC_MSG_ERROR([Cannot find ocamlfind.]))

AC_CHECK_PROG([OCAMLMKLIB], [ocamlmklib], [ocamlmklib])
AS_IF([test "x$OCAMLMKLIB" = x], AC_MSG_ERROR([Cannot find ocamlmklib.]))


OCAMLBEST=byte
AC_CHECK_PROG([OCAMLOPT], [ocamlopt], [ocamlopt])
AS_IF([test "x$OCAMLOPT" = x], [
 AC_MSG_WARN([Cannot find ocamlopt; bytecode compilation only.])
], [
  AC_MSG_CHECKING([ocamlopt version])
  OCAMLOPTVERSION=`$OCAMLOPT -version`
  AC_MSG_RESULT([$OCAMLOPTVERSION])

  AS_IF([test "x$OCAMLVERSION" != "x$OCAMLOPTVERSION"],[
    AC_MSG_WARN([Ocamlopt version differs from ocamlc version. Discard ocamlopt.])
    OCAMLOPT=no
  ],[
    OCAMLBEST=opt
  ])
])

AC_SUBST([ANNOTFLAG])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_FILES([Makefile.config META])

AC_OUTPUT
