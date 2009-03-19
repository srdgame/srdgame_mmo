#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([srdgame], [0.1], [srdgame@gmail.com])
AC_CONFIG_SRCDIR([src/shared/util/final.h])
AC_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
AC_CHECK_LIB([mysqlclient], [mysql_init])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([arpa/inet.h netdb.h netinet/in.h string.h sys/ioctl.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_STRUCT_TM
AC_C_VOLATILE

# Checks for library functions.
AC_PROG_GCC_TRADITIONAL
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_TYPE_SIGNAL
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([gethostbyname inet_ntoa memset socket])

AC_PROG_RANLIB
AC_PROG_LIBTOOL

AC_CONFIG_FILES([Makefile
                 plan/Makefile
                 src/Makefile
                 src/login/Makefile
                 src/realm/Makefile
		 		 src/sample/Makefile
                 src/shared/Makefile
                 src/shared/config/Makefile
                 src/shared/crypto/Makefile
                 src/shared/crypto/md5/Makefile
                 src/shared/crypto/sha/Makefile
                 src/shared/database/Makefile
				 src/shared/game/Makefile
                 src/shared/log/Makefile
                 src/shared/network/Makefile
		 		 src/shared/packet/Makefile
				 src/shared/sample/Makefile
		 		 src/shared/script/Makefile
                 src/shared/thread/Makefile
                 src/shared/util/Makefile
                 src/world/Makefile])
AC_OUTPUT
