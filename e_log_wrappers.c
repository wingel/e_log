#include "e_log.h"

void e_log(enum e_log_level level, const char *tag, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    e_vlog(level, tag, fmt, ap);
    va_end(ap);
}

#if !defined(E_LOG_USE_VARIADIC_MACROS) && !defined(__GNUC__)
void e_debug(const char *tag, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    e_vlog(E_LOG_DEBUG, tag, fmt, ap);
    va_end(ap);
}

void e_info(const char *tag, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    e_vlog(E_LOG_INFO, tag, fmt, ap);
    va_end(ap);
}

void e_warning(const char *tag, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    e_vlog(E_LOG_WARNING, tag, fmt, ap);
    va_end(ap);
}

void e_error(const char *tag, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    e_vlog(E_LOG_ERROR, tag, fmt, ap);
    va_end(ap);
}

void e_fatal(const char *tag, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    e_vlog(E_LOG_FATAL, tag, fmt, ap);
    va_end(ap);
}

#endif /* E_LOG_USE_VARIADIC_MACROS */
