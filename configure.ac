## autoconf configure.ac && autoreconf -if

################################################################################
# This file is free and unencumbered software released into the public domain.
#
# Anyone is free to copy, modify, publish, use, compile, sell, or distribute 
# this file, for any purpose, and by any means.
#
# In jurisdictions that recognize copyright laws, the author or authors of this 
# file dedicate any and all copyright interest in the file to the public
# domain. We make this dedication for the benefit of the public at large and to 
# the detriment of our heirs and successors. We intend this dedication to be an 
# overt act of relinquishment in perpetuity of all present and future rights to 
# this file under copyright law.
#
# THE FILE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
# AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN 
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
# WITH THIS FILE OR THE USE OR OTHER DEALINGS IN THIS FILE.
#
# For more information, please refer to http://unlicense.org/
################################################################################

AC_PREREQ([2.69])
AC_INIT(DESCRIPTION)
AC_PROG_CC_C99

#AC_CHECK_TYPE(
  #[__int128],
  #[],
  #[AC_MSG_ERROR([No compiler support for __int128])]
#)

OBEXTRA="instclldb"

LLDB_HOME="/usr/local"

AC_ARG_WITH(lldb-home,
	AC_HELP_STRING([--with-lldb-home], [Path to LLDB installation.]),
	[LLDB_HOME=$withval])

LLDBR_LIBS="-L${LLDB_HOME}/lib -llldb"

UNAME=`uname`
MAKE=make

if test x$UNAME = xFreeBSD ; then
MAKE=gmake
fi

AC_SUBST(MAKE)

AC_SUBST(LLDB_HOME)
AC_SUBST(LLDBR_LIBS)
AC_SUBST(OBEXTRA)

AC_OUTPUT(src/Makevars)
