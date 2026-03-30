CC      := gcc

# only syntax check
CFLAGS  := -std=c99 -Wall -Wextra -Wpedantic -fsyntax-only

INCLUDES := \
	-I src \
	-I src/microchip_spw \
	-I stubs

SRCS := $(wildcard src/*.c src/microchip_spw/*.c)

.PHONY: all

all:
	@for src in $(SRCS); do \
		$(CC) $(CFLAGS) $(INCLUDES) $$src || exit 1; \
	done

