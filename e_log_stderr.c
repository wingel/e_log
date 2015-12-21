#include "e_log.h"

#include <stdio.h>
#include <stdlib.h>

enum e_log_level e_log_level;

void e_vlog(enum e_log_level level, const char *tag, const char *format, va_list ap)
{
    if (e_log_enabled(level, tag))
    {
	vfprintf(stderr, format, ap);
	putc('\n', stderr);
    }

    if (level == E_LOG_FATAL)
        e_abort();
}

int e_log_enabled(enum e_log_level level, const char *tag)
{
    return level >= e_log_level;
}
