/*
==========================================================================================================
 Project:    File Organizer Tool
 File:       main.c
 Author:     Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
 Created:    2025-11-29
 Updated:    2025-11-29
 License:    MIT License (see LICENSE file for details)
==========================================================================================================

 Description:
    Command-line entry point for the File Organizer Tool. Handles parsing
    of CLI arguments, configuration setup, and invoking the organizer core.

 Usage:
    ./file_organizer [options] [DIRECTORY]

    Options:
      -d, --dir DIR     Target directory (default: current directory)
      -n, --dry-run     Show planned moves without changing the file system
      -v, --verbose     Enable verbose logging
      -h, --help        Show help message

 Notes:
    - A positional DIRECTORY argument overrides the -d/--dir option.
    - Non-regular files (directories, symlinks, devices) are skipped.

==========================================================================================================
*/

#include <stdio.h>
#include <string.h>

#include "organizer.h"
#include "logger.h"

static void print_usage(const char *progname);

int main(int argc, char **argv)
{
    OrganizerConfig config;
    config.target_dir = ".";  /* default: current directory */
    config.dry_run = false;
    config.verbose = false;

    for (int i = 1; i < argc; ++i) {
        const char *arg = argv[i];

        if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        } else if (strcmp(arg, "-n") == 0 || strcmp(arg, "--dry-run") == 0) {
            config.dry_run = true;
        } else if (strcmp(arg, "-v") == 0 || strcmp(arg, "--verbose") == 0) {
            config.verbose = true;
        } else if (strcmp(arg, "-d") == 0 || strcmp(arg, "--dir") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Error: missing argument for %s\n", arg);
                print_usage(argv[0]);
                return 1;
            }
            config.target_dir = argv[++i];
        } else if (arg[0] == '-') {
            fprintf(stderr, "Error: unknown option '%s'\n", arg);
            print_usage(argv[0]);
            return 1;
        } else {
            /* Positional directory argument overrides previous -d/--dir. */
            config.target_dir = arg;
        }
    }

    if (config.verbose) {
        logger_set_level(LOG_LEVEL_DEBUG);
    } else {
        logger_set_level(LOG_LEVEL_INFO);
    }

    int rc = organizer_run(&config);
    if (rc != 0) {
        logger_log(LOG_LEVEL_ERROR, "File organization failed with code %d\n", rc);
    }

    return rc;
}

static void print_usage(const char *progname)
{
    fprintf(stderr,
            "Usage: %s [options] [DIRECTORY]\n"
            "\n"
            "Options:\n"
            "  -d, --dir DIR     Target directory (default: current directory)\n"
            "  -n, --dry-run     Show planned moves without changing the file system\n"
            "  -v, --verbose     Enable verbose logging\n"
            "  -h, --help        Show this help message\n",
            progname);
}
