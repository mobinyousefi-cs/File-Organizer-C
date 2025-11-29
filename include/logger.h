/*
==========================================================================================================
 Project:    File Organizer Tool
 File:       logger.h
 Author:     Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
 Created:    2025-11-29
 Updated:    2025-11-29
 License:    MIT License (see LICENSE file for details)
==========================================================================================================

 Description:
    Lightweight logging interface for the File Organizer Tool. Supports different
    severity levels and centralizes formatted logging output.

 Usage:
    - Call logger_set_level() once during initialization.
    - Use logger_log(level, fmt, ...) for all log messages.

 Notes:
    - The implementation writes ERROR/WARN to stderr, INFO/DEBUG to stdout.
    - The current implementation is simple but can be extended (e.g., file logging).

==========================================================================================================
*/

#ifndef LOGGER_H
#define LOGGER_H

#include <stdarg.h>

/**
 * Log severity levels.
 */
typedef enum {
    LOG_LEVEL_ERROR = 0,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG
} LogLevel;

/**
 * Set the global log level. Messages with a level greater than this
 * will be suppressed.
 */
void logger_set_level(LogLevel level);

/**
 * Log a formatted message at the given severity level.
 */
void logger_log(LogLevel level, const char *fmt, ...);

#endif /* LOGGER_H */
