#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([net2grid], [1.0], [giorgos@giorgos-Inspiron-5547])
AC_CONFIG_SRCDIR([cJSON.h])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h float.h limits.h locale.h netinet/in.h stddef.h stdlib.h string.h sys/socket.h unistd.h time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_STRTOD
AC_CHECK_FUNCS([localeconv memset socket])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
