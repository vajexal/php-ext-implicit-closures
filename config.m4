dnl config.m4 for extension implicit_closures

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary.

dnl If your extension references something external, use 'with':

dnl PHP_ARG_WITH([implicit_closures],
dnl   [for implicit_closures support],
dnl   [AS_HELP_STRING([--with-implicit_closures],
dnl     [Include implicit_closures support])])

dnl Otherwise use 'enable':

PHP_ARG_ENABLE([implicit_closures],
  [whether to enable implicit_closures support],
  [AS_HELP_STRING([--enable-implicit_closures],
    [Enable implicit_closures support])],
  [no])

if test "$PHP_IMPLICIT_CLOSURES" != "no"; then
  dnl Write more examples of tests here...

  dnl Remove this code block if the library does not support pkg-config.
  dnl PKG_CHECK_MODULES([LIBFOO], [foo])
  dnl PHP_EVAL_INCLINE($LIBFOO_CFLAGS)
  dnl PHP_EVAL_LIBLINE($LIBFOO_LIBS, IMPLICIT_CLOSURES_SHARED_LIBADD)

  dnl If you need to check for a particular library version using PKG_CHECK_MODULES,
  dnl you can use comparison operators. For example:
  dnl PKG_CHECK_MODULES([LIBFOO], [foo >= 1.2.3])
  dnl PKG_CHECK_MODULES([LIBFOO], [foo < 3.4])
  dnl PKG_CHECK_MODULES([LIBFOO], [foo = 1.2.3])

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-implicit_closures -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/implicit_closures.h"  # you most likely want to change this
  dnl if test -r $PHP_IMPLICIT_CLOSURES/$SEARCH_FOR; then # path given as parameter
  dnl   IMPLICIT_CLOSURES_DIR=$PHP_IMPLICIT_CLOSURES
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for implicit_closures files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       IMPLICIT_CLOSURES_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$IMPLICIT_CLOSURES_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the implicit_closures distribution])
  dnl fi

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-implicit_closures -> add include path
  dnl PHP_ADD_INCLUDE($IMPLICIT_CLOSURES_DIR/include)

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-implicit_closures -> check for lib and symbol presence
  dnl LIBNAME=IMPLICIT_CLOSURES # you may want to change this
  dnl LIBSYMBOL=IMPLICIT_CLOSURES # you most likely want to change this

  dnl If you need to check for a particular library function (e.g. a conditional
  dnl or version-dependent feature) and you are using pkg-config:
  dnl PHP_CHECK_LIBRARY($LIBNAME, $LIBSYMBOL,
  dnl [
  dnl   AC_DEFINE(HAVE_IMPLICIT_CLOSURES_FEATURE, 1, [ ])
  dnl ],[
  dnl   AC_MSG_ERROR([FEATURE not supported by your implicit_closures library.])
  dnl ], [
  dnl   $LIBFOO_LIBS
  dnl ])

  dnl If you need to check for a particular library function (e.g. a conditional
  dnl or version-dependent feature) and you are not using pkg-config:
  dnl PHP_CHECK_LIBRARY($LIBNAME, $LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $IMPLICIT_CLOSURES_DIR/$PHP_LIBDIR, IMPLICIT_CLOSURES_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_IMPLICIT_CLOSURES_FEATURE, 1, [ ])
  dnl ],[
  dnl   AC_MSG_ERROR([FEATURE not supported by your implicit_closures library.])
  dnl ],[
  dnl   -L$IMPLICIT_CLOSURES_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(IMPLICIT_CLOSURES_SHARED_LIBADD)

  dnl In case of no dependencies
  AC_DEFINE(HAVE_IMPLICIT_CLOSURES, 1, [ Have implicit_closures support ])

  PHP_NEW_EXTENSION(implicit_closures, implicit_closures.c, $ext_shared)
fi
