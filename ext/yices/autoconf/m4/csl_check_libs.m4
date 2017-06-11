
dnl
dnl CSL_COLLECT_LIBRARY_PATHS
dnl -------------------------
dnl Sets variable search_libs to a default list of directories in which to search for libraries.
dnl The list include /usr/local/lib /usr/lib /lib + directories found by 
dnl ld --verbose + LD_LIBRARY_PATH + DYLD_LIBRARY_PATH + LDFLAGS if non-empty.
dnl
AC_DEFUN([CSL_COLLECT_LIBRARY_PATHS],
[
#
# Some (non-GNU) versions of ld do not recognize --verbose
#
# Also, some versions of GNU ld have things like SEARCH_DIR(=/usr/local/lib32)
# and the '=' causes us trouble.
#
# man ld says
#   If searchdir begins with "=", then the "=" will be replaced by the
#   sysroot prefix, a path specified when the linker is configured.
#
# We ignore this here. If the search dir starts with =, we remove =
#
# NOTE: this sed command uses \? which, is a GNU extension.
#
if ld --verbose > /dev/null 2>&1; then
   base_libs=`ld --verbose | grep SEARCH_DIR | sed -e 's,SEARCH_DIR(\"\?=\?,,g' -e 's,\"\?);,,g' | sed -e 's,SEARCH_DIR(\",,g' -e 's,\");,,g' `
else
   base_libs=""
fi
#
# Add LD_LIBRARY_PATH or DYLD_LIBRARY_PATH if they are defined
#
if test "x$LD_LIBRARY_PATH" != x; then
   aux_libs=`echo $LD_LIBRARY_PATH | sed -e 's,:, ,g'`
else
   if test "x$DYLD_LIBRARY_PATH" != x; then
      aux_libs=`echo $DYLD_LIBRARY_PATH | sed -e 's,:, ,g'`
   else
      aux_libs=""
   fi
fi
search_libs="$aux_libs $base_libs /usr/local/lib /usr/lib /lib"
#
# Add other libraries from $LDFLAGS
#
prev_l_opt=false
for ld_arg in $LDFLAGS; do
  if test "$prev_l_opt" = "true"; then
     search_libs=" $ld_arg $search_libs"
  else
     if test "$ld_arg" = "-L"; then
        prev_l_opt=true
     else
        libprefix=`echo $ld_arg | cut -c -2`
        body=`echo $ld_arg | cut -c 3-`
        if test "$libprefix" = "-L"; then
          search_libs=" $body $search_libs"
        else
          libprefix=`echo $ld_arg | cut -c -15`
          body=`echo $ld_arg | cut -c 16-`
          if test "$libprefix" = "--library-path"; then
            search_libs=" $body $search_libs"
          fi
        fi
     fi
  fi
done
])


dnl
dnl CSL_CHECK_LIB_PATH(path,option)
dnl ----------------------------------
dnl path should be the full path to a static library.
dnl this checks whether the given file exists. Prints an
dnl error and exit otherwise.
dnl
dnl If the test succeeds, set variable testlib to path.
dnl
AC_DEFUN([CSL_CHECK_LIB_PATH],
[
testlib=""
AC_MSG_CHECKING([for $1])
if test -f $1; then
  AC_MSG_RESULT(found)
  testlib=$1
else
  AC_MSG_RESULT(no)
  AC_MSG_ERROR([*** $1 was not found: Check option $2 ***])
fi
])


dnl
dnl CSL_CHECK_HEADER(header,includedirlist,option)
dnl ----------------------------------------
dnl check whether a header file is present in one of the includedir
dnl list or, if not found in this list, in the system include directories
dnl (including CPPFLAGS). If the header is found in one of includedirlist,
dnl the variable $testincludedir is set to the found includedir.
dnl If the header has been found in the system include directories,
dnl $testincludedir is let empty.
dnl
dnl The argument 'option' must be the '--with-...-include-dir from which
dnl includedir was given and is used to print the 'checking' message.
dnl
AC_DEFUN([CSL_CHECK_HEADER],
[
testincludedir=""
for incldir in $2; do
  AC_MSG_CHECKING([for gmp.h in $incldir])
  if test -d "$incldir" && test -f "$incldir/$1" ; then
    AC_MSG_RESULT(found)
    testincludedir="$incldir"
    break
  else
    AC_MSG_RESULT(no)
  fi
done
if test "x$testincludedir" = x; then
  AC_CHECK_HEADERS([gmp.h],
    [AC_MSG_NOTICE([using $1 found in system include directories])],
    [AC_MSG_ERROR([*** Header $1 not found. Check option $option or set CPPFLAGS ***])])
fi
])



dnl
dnl CSL_CHECK_GMP(libgmp,includedir)
dnl --------------------------------
dnl attempt to compile a test program that calls a gmp function
dnl set run_ok to "yes" if the program executes and exits with 0
dnl set run_ok to "no" otherwise
dnl
dnl libgmp must be the full-path name to the library to test
dnl includedir must be the directory where the corresponding 'gmp.h'
dnl can be found
dnl
AC_DEFUN([CSL_CHECK_GMP],
[
save_cppflags=$CPPFLAGS
save_libs=$LIBS
#
# Add -I option to CPPFLAGS (it must be first)
# Add $1 to LIBS
#
if test "x$2" != x ; then
   CPPFLAGS="-I$2 $CPPFLAGS"
fi
LIBS="$1 -lm"
AC_MSG_CHECKING([whether $1 is usable])
#
# Test program: check whether the GMP version is recent enough
#
AC_RUN_IFELSE(
[AC_LANG_PROGRAM([[
#include <gmp.h>

mpz_t tst;
]],
[[
mpz_init(tst);
mpz_clear(tst);
if ((__GNU_MP_VERSION < 4) ||
    ((__GNU_MP_VERSION == 4) && (__GNU_MP_VERSION_MINOR < 1))) {
    return 1;
  }
]])

],run_ok=yes,run_ok=no,run_ok=no)
AC_MSG_RESULT([$run_ok])
#
# restore CPPFLAGS and LIBS
#
CPPFLAGS=$save_cppflags
LIBS=$save_libs
])



dnl
dnl CSL_CHECK_STATIC_GMP(libgmp,includedir)
dnl ------------------------------------
dnl If 'libgmp' is empty, try to find the full path to a usable libgmp.a.
dnl Otherwise, check that the given 'libgmp' is usable.
dnl If 'includedir' is empty, try to find a gmp.h header at the same place as
dnl libgmp and in ../include; if it exists, use this directory as -Iincludedir.
dnl If 'includedir' is not empty, then the checks are compiled with -Iincludedir.
dnl if everything works, then set:
dnl  - STATIC_GMP is the fullpath for libgmp.a,
dnl  - STATIC_GMP_INCLUDE_DIR is includedir
dnl
AC_DEFUN([CSL_CHECK_STATIC_GMP],
[
if test "x$1" != x; then
  includedirlist="$(dirname $1)/ $(dirname $1)/../include"
fi
CSL_CHECK_HEADER([gmp.h],[$2 $(echo $includedirlist)],--with-static-gmp-include-dir)
AC_MSG_NOTICE([Trying to find a usable libgmp.a])
#
# if $1 is not given, search for it in library paths
#
if test -z "$1"; then
   CSL_COLLECT_LIBRARY_PATHS
   #
   # now $search_libs contains all the directories to search
   #
   AC_MSG_NOTICE([Searching in $search_libs])
   for ldir in $search_libs; do
       AC_MSG_CHECKING([for libgmp.a in $ldir])
       if test -f $ldir/libgmp.a; then
          AC_MSG_RESULT(found)
          CSL_CHECK_GMP($ldir/libgmp.a,$testincludedir)
          if test $run_ok = yes; then
            STATIC_GMP=$ldir/libgmp.a
            STATIC_GMP_INCLUDE_DIR=$testincludedir
            break
          fi
       else
         AC_MSG_RESULT(no)
       fi
   done
#
#
else
  #
  # User gave option --with-static-gmp=xxx
  # Check whether the specified xxx actually works
  #
  CSL_CHECK_LIB_PATH($1,--with-static-gmp)
  CSL_CHECK_GMP($testlib,$testincludedir)
  if test $run_ok = yes; then
     STATIC_GMP=$testlib
     STATIC_GMP_INCLUDE_DIR=$testincludedir
  else
     AC_MSG_ERROR([*** $1 does not appear to be usable: check option --with-static-gmp ***])
  fi
fi
if test "x$STATIC_GMP" = x; then
   AC_MSG_WARN([*** No usable libgmp.a library was found, defaults to -lgmp (dynamic linking) ***])
fi
])


#
# Check that the given static library has relocation records. If no relocation
# records are found, we can safely deduce that libgmp.a is not
# position-independent code (PIC).
# On windows, all code is always PIC.
# 'libgmp' is the libgmp.a file.
# Set STATIC_GMP_HAS_PIC to 'yes' if it has PIC, 'no' if not.
#
# CHECK_LIB_HAS_RELOC(variable,libgmp.a)
AC_DEFUN([CHECK_LIB_HAS_RELOC],
[
AC_MSG_CHECKING([whether $2 has relocation records (necessary for PIC)])
AS_IF([$(OBJDUMP) -r $2 | grep "RELOCATION RECORDS" -q 2>&5 >&5],
  [$1=yes
  AC_MSG_RESULT([yes])],
  [$1=no
  AC_MSG_RESULT([no])])
AC_SUBST($1)
])

#
# Check that the given static library has PIC (position-independent code).
# As this function relies on libtool, you should run LT_OUTPUT to force
# the creation of ./libtool before the end of configure.
# If libgmp.a does not have PIC, then it will not be possible to
# create a shared library that embeds libgmp.a; the only thing we
# can do with non-PIC is create a static library.
#
# Set variable to 'yes' if given libgmp is PIC, 'no' if not.
#
# CSL_CHECK_STATIC_GMP_HAS_PIC(variable,libgmp,includedir)
AC_DEFUN([CSL_CHECK_STATIC_GMP_HAS_PIC],
[
save_cppflags=$CPPFLAGS
save_libs=$LIBS
#
# Add -I option to CPPFLAGS (it must be first)
# Add $2 to LIBS
#
if test "x$3" != x ; then
   CPPFLAGS="-I$3 $CPPFLAGS"
fi
LIBS="$2 -lm"

AC_MSG_CHECKING([whether $2 is PIC])
AC_LANG(C)
AC_LANG_CONFTEST([AC_LANG_PROGRAM([[
#include <gmp.h>
mpz_t tst;
mp_limb_t* t;
mp_size_t s;
]],[[
// The following should trigger an error with 'ld' with a message:
// ld: illegal text-relocation to '___gmp_binvert_limb_table'
// when trying to link for a shared library (because PIC is not present)
mpn_divexact_1(t,t,s,*t)
]])])
if $SHELL libtool --mode=compile $CC -shared -c conftest.c -o conftest.lo 2>&5 >&5 \
  && $SHELL libtool --mode=link $CC -rpath /usr/lib -o lib.la conftest.lo $2 2>&5 >&5;
then
  $1=yes
  AC_MSG_RESULT(yes)
else
  $1=no
  AC_MSG_RESULT(no)
fi
CPPFLAGS=$save_cppflags
LIBS=$save_libs
$SHELL libtool --mode=clean $RM -f conftest.* lib.la 2>&5 >&5
])

dnl
dnl CSL_CHECK_LIBPOLY(libpoly,includedir,libs)
dnl ------------------------------------------
dnl attempt to compile and run a test program that calls libpoly
dnl set run_ok to "yes" if the program executes and exits with 0.
dnl set run_ok to "no" otherwise.
dnl
dnl libpoly must be the full-path name to the library to test.
dnl includedir must be a directory with 'poly/polyn.h' can be found.
dnl libs may contain GMP libraries to use
dnl
AC_DEFUN([CSL_CHECK_LIBPOLY],
[
save_cppflags=$CPPFLAGS
save_libs=$LIBS
#
# Add the -I option to CPPFLAGS
# Add $1 then $3 to LIBS
#
if test "x$2" != x ; then
   CPPFLAGS="-I$2 $CPPFLAGS"
fi
LIBS="$1"
if test "x$3" != x ; then
   LIBS="$LIBS $3"
fi
AC_MSG_CHECKING([whether $1 is usable])
#
# Test program
#
AC_RUN_IFELSE(
[AC_LANG_PROGRAM([[
#include <poly/variable_db.h>
#include <poly/variable_order.h>
#include <poly/polynomial_context.h>
#include <poly/polynomial.h>
]],
[[
lp_variable_db_t* var_db = lp_variable_db_new();
lp_variable_order_t* var_order = lp_variable_order_new();
lp_polynomial_context_t* ctx = lp_polynomial_context_new(lp_Z, var_db, var_order);
lp_polynomial_t* p = lp_polynomial_new(ctx);
lp_polynomial_delete(p);
lp_polynomial_context_detach(ctx);
lp_variable_order_detach(var_order);
lp_variable_db_detach(var_db);
]])

],run_ok=yes,run_ok=no,run_ok=no)
AC_MSG_RESULT([$run_ok])
#
# restore CPPFLAGS and LIBS
#
CPPFLAGS=$save_cppflags
LIBS=$save_libs
])


dnl
dnl CSL_CHECK_STATIC_LIBPOLY(libpoly,includedir)
dnl --------------------------------------------
dnl Try to find a usable libpoly.a.
dnl If $1 is not empty, it's assumed to be the full path to libpoly.a
dnl If $2 is not empty, it's assumed to be the path to the
dnl corresponding include directory (i.e., the relevant headers are
dnl assumed to be in $2/poly/...
dnl If $3 is not empty, it's the GMP library to use
dnl
dnl If the test succeeds. then this macro sets STATIC_LIBPOLY to
dnl the full path for libpoly.a and STATIC_LIBPOLY_INCLUDE_DIR to $2.
dnl
AC_DEFUN([CSL_CHECK_STATIC_LIBPOLY],
[
if test "x$1" != x; then
  includedirlist="$(dirname $1)/ $(dirname $1)/../include"
fi
CSL_CHECK_HEADER([poly/polynomial.h],[$2 $(echo $includedirlist)],--with-static-libpoly-include-dir)
AC_MSG_NOTICE([Searching for a usable libpoly.a])
#
# If $1 is empty search for it in the 
#
if test -z "$1" ; then
   CSL_COLLECT_LIBRARY_PATHS
   #
   # $search_libs contains the directories to search
   #
   AC_MSG_NOTICE([Searching in $search_libs])
   for ldir in $search_libs ; do
      AC_MSG_CHECKING([for libpoly.a in $ldir])
      if test -f $ldir/libpoly.a ; then
         AC_MSG_RESULT(found)
         testlib=$ldir/libpoly.a
         CSL_CHECK_LIBPOLY($testlib,$testincludedir,$3)
         if test $run_ok = yes ; then
            STATIC_LIBPOLY=$testlib
            STATIC_LIBPOLY_INCLUDE_DIR=$testincludedir
            break
         fi
      else
         AC_MSG_RESULT(no)
      fi
   done
#
else
  # got --with-static-libpoly=xxxx
  CSL_CHECK_LIB_PATH($1,--with-static-libpoly)
  CSL_CHECK_LIBPOLY($testlib,$testincludedir,$3)
  if test $run_ok = yes; then
     STATIC_LIBPOLY=$testlib
     STATIC_LIBPOLY_INCLUDE_DIR=$testincludedir
  else
     AC_MSG_ERROR([*** $1 does not appear to be usable: check option --with-static-libpoly ***])
  fi
fi

if test "x$STATIC_LIBPOLY" = x; then
   AC_MSG_WARN([*** No usable libpoly.a library was found ***])
fi
])

