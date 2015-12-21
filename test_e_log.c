#if 1
#define E_LOG_ENABLE_DEBUG
#else
#define E_LOG_DISABLE_DEBUG
#endif

#include "e_log.h"

int main()
{
    E_DEBUG  (("my.test", "debug message:    %s", "DEBUG"));
    e_debug   ("my.test", "debug message:    %s", "debug");
    e_info    ("my.test", "info message:     %s", "info");
    e_warning ("my.test", "warning message:  %s", "warning");
    e_error   ("my.test", "error message:    %s", "error");
    e_fatal   ("my.test", "fatal message:    %s", "fatal");

    return 0;
}
