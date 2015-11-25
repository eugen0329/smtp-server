rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))
SHELL := /bin/bash
CC = gcc

CFLAGS = -std=c99 -g -Wall -pedantic-errors
LDFLAGS =
LDLIBS = -ldl

OBJ_DIR     = ./obj
HEADERS_DIR = ./src
SRC_DIR     = ./src
INCLUDE_DIRS  = -I ./ -I $(SRC_DIR)

SRC_NAMES     = $(call rwildcard, $(SRC_DIR), *.c)
HEADERS_NAMES = $(wildcard $(SRC_DIR)/*.h)
OBJ_NAMES     = $(SRC_NAMES:.c=.o)

OBJECTS = $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(OBJ_NAMES))
HEADERS = $(rwildcard $(HEADERS_DIR)/*.h)
SOURCES = $(SRC_NAMES)
EXECUTABLE = bin

.PHONY: duplicate_src_tree clean c rebuild r

all: duplicate_src_tree $(EXECUTABLE) $(SOURCES)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(LDLIBS) $(OBJECTS) -o "$@"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c "$<" $(INCLUDE_DIRS) -o "$@"

duplicate_src_tree:
	@for f in  "$(shell \find $(SRC_DIR) -not -path '*/\.*' -type d -not -wholename $(SRC_DIR))"; do \
		\mkdir -pv "$(OBJ_DIR)/$$(echo $$f | sed "s/[./]*\w*\///")"; \
	done

rebuild: clean all
r: rebuild

clean:
	\rm -fv $(EXECUTABLE) $(OBJECTS)
c: clean
