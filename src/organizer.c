/*
==========================================================================================================
 Project:    File Organizer Tool
 File:       organizer.c
 Author:     Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
 Created:    2025-11-29
 Updated:    2025-11-29
 License:    MIT License (see LICENSE file for details)
==========================================================================================================

 Description:
    Core implementation of the file organization logic. Scans a target directory,
    categorizes regular files by extension, and moves them into category folders
    such as Images, Documents, Audio, Video, Archives, Source, Other, etc.

 Usage:
    OrganizerConfig config = { .target_dir = ".", .dry_run = false, .verbose = false };
    int rc = organizer_run(&config);

 Notes:
    - Only regular files are moved; directories and other special file types
      are skipped.
    - If a file with the same name already exists in the target category,
      a unique name with a numeric suffix is generated.

==========================================================================================================
*/

#include "organizer.h"
#include "logger.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#ifndef _WIN32
#include <unistd.h>
#else
#include <direct.h>  /* _mkdir */
#endif

/* Map file extensions to category directory names. */
typedef struct {
    const char *ext;      /* lowercase extension, without dot */
    const char *category; /* directory name */
} ExtensionCategory;

static const ExtensionCategory EXTENSION_TABLE[] = {
    {"jpg",  "Images"},
    {"jpeg", "Images"},
    {"png",  "Images"},
    {"gif",  "Images"},
    {"bmp",  "Images"},
    {"tif",  "Images"},
    {"tiff", "Images"},
    {"svg",  "Images"},

    {"txt",  "Documents"},
    {"md",   "Documents"},
    {"pdf",  "Documents"},
    {"doc",  "Documents"},
    {"docx", "Documents"},
    {"rtf",  "Documents"},

    {"xls",  "Spreadsheets"},
    {"xlsx", "Spreadsheets"},
    {"csv",  "Spreadsheets"},

    {"ppt",  "Presentations"},
    {"pptx", "Presentations"},

    {"mp3",  "Audio"},
    {"wav",  "Audio"},
    {"flac", "Audio"},
    {"aac",  "Audio"},
    {"ogg",  "Audio"},

    {"mp4",  "Video"},
    {"mkv",  "Video"},
    {"avi",  "Video"},
    {"mov",  "Video"},
    {"wmv",  "Video"},

    {"zip",  "Archives"},
    {"rar",  "Archives"},
    {"7z",   "Archives"},
    {"tar",  "Archives"},
    {"gz",   "Archives"},

    {"c",    "Source"},
    {"h",    "Source"},
    {"cpp",  "Source"},
    {"hpp",  "Source"},
    {"py",   "Source"},
    {"java", "Source"},
    {"js",   "Source"},
    {"ts",   "Source"},
    {"cs",   "Source"},
    {"go",   "Source"},
    {"rb",   "Source"},
    {"php",  "Source"},
};

static const char *DEFAULT_CATEGORY = "Other";

static void join_path(char *buffer, size_t bufsize,
                      const char *dir, const char *name)
{
    if (bufsize == 0) {
        return;
    }

    size_t len = strlen(dir);
    int needs_sep = 1;
    if (len > 0 && (dir[len - 1] == '/' || dir[len - 1] == '\\')) {
        needs_sep = 0;
    }

    if (needs_sep) {
        snprintf(buffer, bufsize, "%s/%s", dir, name);
    } else {
        snprintf(buffer, bufsize, "%s%s", dir, name);
    }
}

static void to_lowercase(char *s)
{
    for (; *s; ++s) {
        if (*s >= 'A' && *s <= 'Z') {
            *s = (char)(*s - 'A' + 'a');
        }
    }
}

static const char *get_extension(const char *name)
{
    const char *dot = strrchr(name, '.');
    if (!dot || dot == name) {
        return NULL;
    }
    return dot + 1;
}

static const char *category_for_extension(const char *ext)
{
    if (!ext) {
        return DEFAULT_CATEGORY;
    }

    char tmp[64];
    size_t len = strlen(ext);
    if (len == 0 || len >= sizeof(tmp)) {
        return DEFAULT_CATEGORY;
    }

    strcpy(tmp, ext);
    to_lowercase(tmp);

    size_t count = sizeof(EXTENSION_TABLE) / sizeof(EXTENSION_TABLE[0]);
    for (size_t i = 0; i < count; ++i) {
        if (strcmp(tmp, EXTENSION_TABLE[i].ext) == 0) {
            return EXTENSION_TABLE[i].category;
        }
    }

    return DEFAULT_CATEGORY;
}

static int ensure_directory_exists(const char *base_dir,
                                   const char *category,
                                   char *out_path,
                                   size_t out_size)
{
    struct stat st;

    join_path(out_path, out_size, base_dir, category);

    if (stat(out_path, &st) == 0) {
        if (S_ISDIR(st.st_mode)) {
            return 0;
        }

        logger_log(LOG_LEVEL_ERROR,
                   "Path exists but is not a directory: %s\n",
                   out_path);
        return -1;
    }

    /* Try to create the directory. */
#ifdef _WIN32
    if (_mkdir(out_path) != 0) {
        logger_log(LOG_LEVEL_ERROR,
                   "Failed to create directory '%s': %s\n",
                   out_path, strerror(errno));
        return -1;
    }
#else
    if (mkdir(out_path, 0755) != 0) {
        logger_log(LOG_LEVEL_ERROR,
                   "Failed to create directory '%s': %s\n",
                   out_path, strerror(errno));
        return -1;
    }
#endif

    logger_log(LOG_LEVEL_INFO, "Created directory: %s\n", out_path);
    return 0;
}

static int build_unique_destination(const char *category_dir,
                                    const char *filename,
                                    char *out_path,
                                    size_t out_size)
{
    struct stat st;

    /* First try plain name. */
    join_path(out_path, out_size, category_dir, filename);
    if (stat(out_path, &st) != 0) {
        return 0; /* free */
    }

    /* Split into base name and extension. */
    char name[PATH_MAX];
    char ext[PATH_MAX];

    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) {
        /* No extension. */
        strncpy(name, filename, sizeof(name));
        name[sizeof(name) - 1] = '\0';
        ext[0] = '\0';
    } else {
        size_t base_len = (size_t)(dot - filename);
        if (base_len >= sizeof(name)) {
            base_len = sizeof(name) - 1;
        }
        memcpy(name, filename, base_len);
        name[base_len] = '\0';

        strncpy(ext, dot, sizeof(ext));
        ext[sizeof(ext) - 1] = '\0';
    }

    for (int i = 1; i < 10000; ++i) {
        if (snprintf(out_path, out_size, "%s/%s_%d%s",
                     category_dir, name, i, ext) >= (int)out_size) {
            continue; /* truncated; try next */
        }

        if (stat(out_path, &st) != 0) {
            return 0; /* found free name */
        }
    }

    logger_log(LOG_LEVEL_ERROR,
               "Could not generate unique name for '%s' in '%s'\n",
               filename, category_dir);
    return -1;
}

int organizer_run(const OrganizerConfig *config)
{
    if (config == NULL || config->target_dir == NULL) {
        logger_log(LOG_LEVEL_ERROR, "Invalid configuration\n");
        return 1;
    }

    const char *base_dir = config->target_dir;
    struct stat st;

    if (stat(base_dir, &st) != 0) {
        logger_log(LOG_LEVEL_ERROR,
                   "Cannot access directory '%s': %s\n",
                   base_dir, strerror(errno));
        return 1;
    }

    if (!S_ISDIR(st.st_mode)) {
        logger_log(LOG_LEVEL_ERROR,
                   "Path is not a directory: '%s'\n",
                   base_dir);
        return 1;
    }

    DIR *dir = opendir(base_dir);
    if (!dir) {
        logger_log(LOG_LEVEL_ERROR,
                   "Failed to open directory '%s': %s\n",
                   base_dir, strerror(errno));
        return 1;
    }

    logger_log(LOG_LEVEL_INFO,
               "Organizing files in '%s'%s\n",
               base_dir,
               config->dry_run ? " (dry-run mode)" : "");

    struct dirent *entry;
    char src_path[PATH_MAX];
    char category_dir[PATH_MAX];
    char dst_path[PATH_MAX];
    int result = 0;

    while ((entry = readdir(dir)) != NULL) {
        const char *name = entry->d_name;

        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
            continue;
        }

        join_path(src_path, sizeof(src_path), base_dir, name);

        if (stat(src_path, &st) != 0) {
            logger_log(LOG_LEVEL_WARN,
                       "Skipping '%s' (cannot stat: %s)\n",
                       src_path, strerror(errno));
            continue;
        }

        if (!S_ISREG(st.st_mode)) {
            if (config->verbose) {
                logger_log(LOG_LEVEL_DEBUG,
                           "Skipping non-regular file: %s\n",
                           src_path);
            }
            continue; /* Only organize regular files */
        }

        const char *ext = get_extension(name);
        const char *category = category_for_extension(ext);

        if (ensure_directory_exists(base_dir, category,
                                    category_dir,
                                    sizeof(category_dir)) != 0) {
            result = 1;
            continue;
        }

        if (build_unique_destination(category_dir, name,
                                     dst_path, sizeof(dst_path)) != 0) {
            result = 1;
            continue;
        }

        if (config->dry_run) {
            logger_log(LOG_LEVEL_INFO,
                       "[DRY-RUN] Move '%s' -> '%s'\n",
                       src_path, dst_path);
            continue;
        }

        if (rename(src_path, dst_path) != 0) {
            logger_log(LOG_LEVEL_ERROR,
                       "Failed to move '%s' -> '%s': %s\n",
                       src_path, dst_path, strerror(errno));
            result = 1;
        } else {
            logger_log(LOG_LEVEL_INFO,
                       "Moved '%s' -> '%s'\n",
                       src_path, dst_path);
        }
    }

    closedir(dir);
    return result;
}
