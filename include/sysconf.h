/* include/sysconf.h.  Generated from sysconf.h.in by configure.  */
/* include/sysconf.h.in.  Generated from configure.in by autoheader.  */

/* "Default umask Permissions" */
#define DEFUMASK 077

/* "No prefix needed for dlopen" */
#define DL_PREFIX ""

/* "Has sys/types.h" */
#define HAS_SYS_TYPES_H 1

/* "" */
#define HAVE_BACKTRACE 1

/* Define to 1 if you have the `fork' function. */
#define HAVE_FORK 1

/* Define to 1 if you have the `gethostbyname' function. */
#define HAVE_GETHOSTBYNAME 1

/* Define to 1 if you have the `gethostbyname_r' function. */
#define HAVE_GETHOSTBYNAME_R 1

/* Define to 1 if you have the `gettimeofday' function. */
#define HAVE_GETTIMEOFDAY 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `setgrent' function. */
#define HAVE_SETGRENT 1

/* Define to 1 if you have the `snprintf' function. */
#define HAVE_SNPRINTF 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strcasecmp' function. */
#define HAVE_STRCASECMP 1

/* Define to 1 if you have the `strdup' function. */
#define HAVE_STRDUP 1

/* Define to 1 if you have the `strerror' function. */
#define HAVE_STRERROR 1

/* Define to 1 if you have the `stricmp' function. */
/* #undef HAVE_STRICMP */

/* "" */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strlcat' function. */
/* #undef HAVE_STRLCAT */

/* Define to 1 if you have the `strlcpy' function. */
/* #undef HAVE_STRLCPY */

/* Define to 1 if you have the `strsignal' function. */
#define HAVE_STRSIGNAL 1

/* Define to 1 if you have the `strspn' function. */
#define HAVE_STRSPN 1

/* Define to 1 if you have the `sys_errlist' function. */
#define HAVE_SYS_ERRLIST 1

/* "" */
#define HAVE_SYS_SELECT_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the `umask' function. */
#define HAVE_UMASK 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* va_list as array */
/* #undef HAVE_VA_LIST_AS_ARRAY */

/* "Module dir" */
#define MODULE_PATH "/home/ziber/xanadu/modules/"

/* "Has mysql/mysql.h" */
/* #undef MYSQL_HEADER_PREFIX */

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME ""

/* Define to the full name and version of this package. */
#define PACKAGE_STRING ""

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME ""

/* Define to the version of this package. */
#define PACKAGE_VERSION ""

/* "Run group" */
/* #undef RUNGROUP */

/* "Binary Dir" */
#define SERVICES_BIN "/home/ziber/xanadu/services"

/* "services bin dir" */
#define SERVICES_DIR "/home/ziber/xanadu"

/* The size of a `int', as computed by sizeof. */
#define SIZEOF_INT 4

/* The size of a `long', as computed by sizeof. */
#define SIZEOF_LONG 4

/* The size of a `short', as computed by sizeof. */
#define SIZEOF_SHORT 2

/* "modules not available" */
/* #undef STATIC_LINKING */

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* "Modules available" */
#define USE_MODULES 1

/* "Use Mysql" */
/* #undef USE_MYSQL */

/* "Use RDB" */
/* #undef USE_RDB */

/* Define to `short' if <sys/types.h> does not define. */
/* #undef int16_t */

/* Define to `long' if <sys/types.h> does not define. */
/* #undef int32_t */

/* Define to `unsigned short' if <sys/types.h> does not define. */
/* #undef u_int16_t */

/* Define to `unsigned long' if <sys/types.h> does not define. */
/* #undef u_int32_t */

/* Static config, copy from here to below before running autoheader! */

#ifndef BIG_ENDIAN

/* Big Endian system */
/* #undef BIG_ENDIAN */

#endif

#ifndef LITTLE_ENDIAN

/* Little Endian system */
#define LITTLE_ENDIAN 1

#endif

/**
 * NOTE: BIG_ENDIAN and LITTLE_ENDIAN defines should not be left in
 * by autoheader as they may be defined or may not be, as such we check
 * if there defined before messing with them!
 **/

#ifdef HAS_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef __STRICT_ANSI__
#include <stdarg.h>
#include <stdio.h>

/* We KNOW these are not ansi, we are defining them here to suppress the warning
 *  * s messages on a "make strict" compile */
#ifndef snprintf
int snprintf(char *str, size_t size, const char *format, ...);
#endif
#ifndef vprintf
int vprintf(const char *format, va_list ap);
#endif
#ifndef vfprintf
int vfprintf(FILE *stream, const char *format, va_list ap);
#endif
#ifndef vsprintf
int vsprintf(char *str, const char *format, va_list ap);
#endif
#ifndef vsnprintf
int vsnprintf(char *str, size_t size, const char *format, va_list ap);
#endif

FILE *popen(const char *command, const char *type);
int pclose(FILE *stream);
FILE *fdopen(int fildes, const char *mode);

int ftruncate(int fd, off_t length);

#ifdef HAVE_STRSIGNAL
char *strsignal(int sig);
#endif

#endif


typedef int16_t int16;
typedef u_int16_t uint16;
typedef int32_t int32;
typedef u_int32_t uint32;

