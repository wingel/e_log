Embedded log API
================

This is a simple library for logging on embedded systems.

Main goals:

* Simple to use.

* Have a simple and stupid default implementation that prints log
  messages to standard output or standard error.

* Have a clear separation between the logging API and the
  implementation of it.  It should be possible to compile an object
  file using the logging API and switch to a different logging
  implementation by relinking to a different logging library.

* Be possible to disable debug logs at compile time to save space.

* Have a way of distinguishing logs from different modules.

* Be decently efficient and not use too much memory.

* If possibe it should be portable.  It would be nice to be able to
  use the same API on old non-C99 compilers or even better on
  non-standards compliant compilers such as Microchip's PICC.

* A liberal license so that it can be used in proprietary deeply
  embedded projects.

Usage
=====

Here's an example of how to use the logging API.  "my.test" is a
logging tag.  Tags are free form, but it's a good idea to come up with
some kind of regular structure for tags within your project.

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

e_fatal is supposed to abort the program, but it is up to the
implementation to decide exactly what to do.

Normally debug logs are compiled in, unless NDEBUG is defined.  It is
possible to forcibly enable debug logs for the current compilation
unit by defining E_LOG_ENABLE_DEBUG before including "e_log.h".  It is
also possible to forcibly disable debug logs with E_LOG_DISABLE_DEBUG.

For each tag and log level it's possible to check if it is enabled or
not.  This can be used to avoid costly calculations if the log message
won't be displayed anyway.  For example, some code that only
calculates some statistics if informational messages should be printed
could look like this:

    if (e_info_enabled("my.statistics"))
    {
        char s[1024];

        calculate_statistics(s, sizeof(s));
        log_info("my.statistics", "statistics: %s", s);
    }

Simple stderr implementation
============================

e_log_stderr.c contains a simple implementation of e_log which prints
all log messages with a log level higher than e_log_level to standard
error.  The defines in e_log.h and wrappers in e_log_wrappers.c map
the whole e_log api onto two functions.

The first function is uses e_log_level to decide if a log message
should be printed or not:

    int e_log_enabled(enum e_log_level level, const char *tag)
    {
        return level >= e_log_level;
    }

The second function is a vprintf-like function which actually prints
the log message:

    void e_vlog(enum e_log_level level, const char *tag,
                const char *format, va_list ap)
    {
        if (e_log_enabled(level, tag))
        {
            vfprintf(stderr, format, ap);
            putc('\n', stderr);
        }

        if (level == E_LOG_FATAL)
            e_abort();
    }

For fatal log messages, the function e_abort defined in e_log_abort.c
will be called.

More advanced implementations
=============================

A more advanced logger implementation can do much more.

The simple stderr variant ignores the log tag.  A smarter logging
implementation could use the tag to have different logging levels for
different modules.

A different implementation can choose to override only parts of the
stderr implementation.  For example it could override only the e_abort
function, instead of calling abort(3) it would reboot the whole
embedded system.

An implementation that logs to syslog could override only e_vlog and
e_log_enabled.  Or it could forward the logs to a different logging
library such as GLib's g_log or log4c.

Other logging libraries
=======================

In my opinion log4c (http://log4c.sourceforge.net/) is much too
complicated.

The Gnome glib g_log API
(https://developer.gnome.org/glib/stable/glib-Message-Logging.html) is
actually quite nice, but the API mixes the functions to actually log
something with the implementation.  They also have things such as
G_LOG_FATAL_MASK and G_LOG_LEVEL_USER_SHIFT which means that one has
to recompile ones code if the implementation changes.  It also relies
on preprocessor tricks which aren't that portable to older compilers.

zlog (https://hardysimpson.github.io/zlog/) also looks quite nice but
I feel that it is a bit too complicated to use since it requires the
user to get a category handle before being able to log anything.

All of these libraries are also LGPL which rules them out for
proprietary deeply embedded systems.


