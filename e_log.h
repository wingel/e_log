/* Copyright (C) 2011-2015 Christer Weinigel <christer@weinigel.se> */

#ifndef _E_LOG_H
#define _E_LOG_H

#include <stdarg.h>

/* If E_LOG_ENABLE_DEBUG is defined, always enable debug messages.
 * Otherwise, default to turning on E_LOG_ENABLE_DEBUG unless NDEBUG
 * or E_LOG_DISABLE_DEBUG are defined. */

#ifdef E_LOG_ENABLE_DEBUG
/* do nothing */
#elif !defined(E_LOG_DISABLE_DEBUG) && !defined(NDEBUG)
#define E_LOG_ENABLE_DEBUG
#endif

/* If we're using a C99 compiler we can use variadic macros instead of
 * having separate functions for e_debug/e_info/.... */
#if __STDC_VERSION__ >= 199901L
#error foo
#define E_LOG_USE_VARIADIC_MACROS
#endif

enum e_log_level
{
    E_LOG_DEBUG,
    E_LOG_INFO,
    E_LOG_WARNING,
    E_LOG_ERROR,
    E_LOG_FATAL,
    E_LOG_NONE
};

/* The following two functions must be implemented in a backend */
void e_vlog(enum e_log_level level, const char *tag, const char *format, va_list ap)
#ifdef __GNUC__
    __attribute__ ((__format__(__printf__, 3, 0)))
#endif
    ;

int e_log_enabled(enum e_log_level, const char *tag);

/* This function is implemented in e_log_wrapper.c */
void e_log(enum e_log_level level, const char *tag, const char *format, ...)
#ifdef __GNUC__
    __attribute__ ((__format__(__printf__, 3, 4)))
#endif
    ;

/* The simple stderr implementation of e_vlog uses this variable */
extern enum e_log_level e_log_level;

#ifdef E_LOG_USE_VARIADIC_MACROS
#define e_debug(tag, ...) e_log(E_LOG_DEBUG, tag, __VA_ARGS__)
#define e_info(tag, ...) e_log(E_LOG_INFO, tag, __VA_ARGS__)
#define e_warning(tag, ...) e_log(E_LOG_WARNING, tag, __VA_ARGS__)
#define e_error(tag, ...) e_log(E_LOG_ERROR, tag, __VA_ARGS__)
#define e_fatal(tag, ...) e_log(E_LOG_FATAL, tag, __VA_ARGS__)
#elif __GNUC__
#define e_debug(tag, format, args...) e_log(E_LOG_DEBUG, tag, format, args)
#define e_info(tag, format, args...) e_log(E_LOG_INFO, tag, format, args)
#define e_warning(tag, format, args...) e_log(E_LOG_WARNING, tag, format, args)
#define e_error(tag, format, args...) e_log(E_LOG_ERROR, tag, format, args)
#define e_fatal(tag, format, args...) e_log(E_LOG_FATAL, tag, format, args)
#else
/* These functions are implemented in e_log_wrapper.c */
void e_debug (const char *tag, const char *format, ...);
void e_info (const char *tag, const char *format, ...);
void e_warning (const char *tag, const char *format, ...);
void e_error (const char *tag, const char *format, ...);
void e_fatal (const char *tag, const char *format, ...);
#endif

/* If you are using an ancient compiler such as Microchips compiler
 * for PIC that does not support variadic macros, you can use an ugly
 * macro to allow compiling out e_debug calls.  Use this macro with an
 * extra parenthesis like this:
 *
 *     E_DEBUG( ("tag", "the number is %u", 17) );
 *
 * It makes the code uglier but this should be portable even to an
 * ancient dim-wit C compiler.
 */
#define E_DEBUG(params) e_debug params

#define e_vdebug (tag, format, ap) e_vlog(E_LOG_DEBUG , tag, format, ap)
#define e_vinfo (tag, format, ap) e_vlog(E_LOG_INFO , tag, format, ap)
#define e_vwarning (tag, format, ap) e_vlog(E_LOG_WARNING, tag, format, ap)
#define e_verror (tag, format, ap) e_vlog(E_LOG_ERROR , tag, format, ap)
#define e_vfatal (tag, format, ap) e_vlog(E_LOG_FATAL , tag, format, ap)

#define e_debug_enabled (tag) e_log_enabled(E_LOG_DEBUG, tag)
#define e_info_enabled (tag) e_log_enabled(E_LOG_INFO, tag)
#define e_warning_enabled (tag) e_log_enabled(E_LOG_WARNING, tag)
#define e_error_enabled (tag) e_log_enabled(E_LOG_ERROR, tag)
#define e_fatal_enabled (tag) e_log_enabled(E_LOG_FATAL, tag)

#ifndef E_LOG_ENABLE_DEBUG

#undef e_debug
#undef e_vdebug
#undef e_debug_enabled

#ifdef __GNUC__
/* gcc is smart enough to optimise out unreachable code, but will
 * still issue warnings for it.  So use a nonexistent function in a
 * while(0) block when debugging is not wanted. */

void e_debug_notdef(const char *tag, const char *format, ...)
    __attribute__ ((__format__(__printf__, 2, 3)))
    ;

void e_vdebug_notdef(const char *tag, const char *format, va_list ap)
    __attribute__ ((__format__(__printf__, 2, 0)))
    ;

#define e_debug while (0) e_debug_notdef
#define e_vdebug while (0) e_vdebug_notdef

#else  /* __GNUC__ */

#define e_debug(...)
#define e_vdebug(...)

#endif  /* !__GNUC__ */

#define e_debug_enabled(tag) (0 && (tag))

#ifdef E_LOG_OLD_DIMWIT_COMPILER
#undef E_DEBUG
#define E_DEBUG(params)
#endif

#endif  /* !E_LOG_ENABLE_DEBUG */

void e_abort(void);

#endif /* _E_LOG_H */
