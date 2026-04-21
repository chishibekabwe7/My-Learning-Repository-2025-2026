# /*
#  * Filename: Makefile
#  * Purpose: Compiles all mini-os source placeholder files with gcc using -Wall and -g.
#  * Author: [Author Name]
#  * Date: 2026-04-21
#  */

CC = gcc
CFLAGS = -Wall -g
SRC_DIR = src

SOURCES = \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/process.c \
	$(SRC_DIR)/scheduler.c \
	$(SRC_DIR)/memory.c \
	$(SRC_DIR)/ipc.c \
	$(SRC_DIR)/deadlock.c \
	$(SRC_DIR)/logger.c

OBJECTS = $(SOURCES:.c=.o)

all: $(OBJECTS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)

.PHONY: all clean
