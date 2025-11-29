/*
==========================================================================================================
 Project:    File Organizer Tool
 File:       logger.c
 Author:     Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
 Created:    2025-11-29
 Updated:    2025-11-29
 License:    MIT License (see LICENSE file for details)
==========================================================================================================

 Description:
    Implementation of a simple logging facility with severity levels
    and timestamped output for the File Organizer Tool.

 Usage:
    - Call logger_set_level(LOG_LEVEL_*) during initialization.
    - Use logger_log(LOG_LEVEL_INFO, "Message: %d\n", value);

 Notes:
    - ERROR/WARN messages go to stderr, INFO/DEBUG go to stdout.
    - Timestamps are generated using local time.

==========================================================================================================
*/

#include "logger.h"

#include <stdio.h>
#include <time.h>
#include <stdarg.h>

static LogLevel current_level = LOG_LEVEL_INFO;

void logger_set_level(LogLevel level)
{
    current_level = level;
}

static const char *level_to_string(LogLevel level)
{
    switch (level) {
    case LOG_LEVEL_ERROR: return "ERROR";
    case LOG_LEVEL_WARN:  return "WARN";
    case LOG_LEVEL_INFO:  return "INFO";
    case LOG_LEVEL_DEBUG: return "DEBUG";
    default:              return "?";
    }
}

void logger_log(LogLevel level, const char *fmt, ...)
{
    if (level > current_level) {
        return;
    }

    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);

    char timestamp[20];
    if (tm_now != NULL) {
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_now);
    } else {
        timestamp[0] = '\0';
    }

    FILE *out = (level == LOG_LEVEL_ERROR || level == LOG_LEVEL_WARN) ? stderr : stdout;

    fprintf(out, "[%s] %-5s: ", timestamp, level_to_string(level));

    va_list args;
    va_start(args, fmt);
    vfprintf(out, fmt, args);
    va_end(args);
}
