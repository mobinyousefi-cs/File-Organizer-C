# =========================================================================================================
#  Project:    File Organizer Tool
#  File:       Makefile
#  Author:     Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
#  Created:    2025-11-29
#  Updated:    2025-11-29
#  License:    MIT License (see LICENSE file for details)
# =========================================================================================================
#
#  Description:
#      Build configuration for the File Organizer Tool in C.
#
#  Usage:
#      make            # Build the project
#      make clean      # Remove build artifacts
#
#  Notes:
#      - Adjust compiler flags or directories as needed.
#      - You can integrate this into a CI pipeline on GitHub.
#
# =========================================================================================================

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -Iinclude
LDFLAGS =

SRC_DIR = src
OBJ_DIR = build
BIN_DIR = bin
TARGET = $(BIN_DIR)/file_organizer

SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/organizer.c \
       $(SRC_DIR)/logger.c

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

.PHONY: all clean dirs

all: dirs $(TARGET)

dirs:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
