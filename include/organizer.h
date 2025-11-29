/*
==========================================================================================================
 Project:    File Organizer Tool
 File:       organizer.h
 Author:     Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
 Created:    2025-11-29
 Updated:    2025-11-29
 License:    MIT License (see LICENSE file for details)
==========================================================================================================

 Description:
    Public interface for the file organization engine. Defines the configuration
    structure and the main organizer entry point.

 Usage:
    - Configure an OrganizerConfig instance.
    - Call organizer_run(&config) from main().

 Notes:
    - This module is intentionally independent of CLI parsing.
    - It can be reused in other front-ends (e.g., GUI or another CLI wrapper).

==========================================================================================================
*/

#ifndef ORGANIZER_H
#define ORGANIZER_H

#include <stdbool.h>

/**
 * Configuration for the file organizer.
 */
typedef struct {
    /** Target directory to organize. */
    const char *target_dir;

    /** If true, only print planned operations without modifying the file system. */
    bool dry_run;

    /** If true, print verbose diagnostic output. */
    bool verbose;
} OrganizerConfig;

/**
 * Run the file organizer.
 *
 * @param config  Pointer to configuration structure.
 * @return        0 on success, non-zero on failure.
 */
int organizer_run(const OrganizerConfig *config);

#endif /* ORGANIZER_H */
