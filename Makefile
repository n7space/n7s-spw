CC := gcc

# only syntax check
CFLAGS  := -std=c99 -Wall -Wextra -Wpedantic -fsyntax-only

INCLUDES := \
	-I src \
	-I src/microchip_spw \
	-I stubs

SRCS := $(wildcard src/*.c src/microchip_spw/*.c)

TEST_CC := arm-none-eabi-gcc
TEST_SIZE := arm-none-eabi-size

TEST_CPU_FLAGS := \
	-mcpu=cortex-m7 \
	-mfpu=fpv5-d16 \
	-mfloat-abi=hard \
	-mthumb

TEST_CFLAGS := \
	$(TEST_CPU_FLAGS) \
	-std=gnu99 \
	-Wall -Wextra \
	-ffunction-sections \
	-fdata-sections \
	-g -O0 \
	-DN7S_TARGET_SAMRH71F20

TEST_INCLUDES := \
	-I test \
	-I arm-bsp/src \
	-I src \
	-I src/microchip_spw \
	-I stubs

TEST_LDFLAGS := \
	$(TEST_CPU_FLAGS) \
	-T test/samrh71_gcc.ld \
	-Wl,--gc-sections \
	-Wl,-Map,build/test/test.map \
	-nostartfiles \
	-specs=nosys.specs \
	-lc -lm

TEST_SRCS := \
	$(wildcard test/*.c) \
	$(wildcard src/*.c) \
	$(filter-out src/microchip_spw/plib_spw.c, $(wildcard src/microchip_spw/*.c)) \
	arm-bsp/src/Matrix/Matrix.c \
	arm-bsp/src/Nvic/Nvic.c \
	arm-bsp/src/Wdt/Wdt.c

TEST_OBJS := $(patsubst %.c, build/test/%.o, $(TEST_SRCS))

TEST_ELF := build/test/test.elf

.PHONY: test test-clean

all:
	@for src in $(SRCS); do \
		$(CC) $(CFLAGS) $(INCLUDES) $$src || exit 1; \
	done

test: $(TEST_ELF)
	@$(TEST_SIZE) $(TEST_ELF)

$(TEST_ELF): $(TEST_OBJS)
	$(TEST_CC) $(TEST_LDFLAGS) -o $@ $^

build/test/%.o: %.c
	@mkdir -p $(dir $@)
	$(TEST_CC) $(TEST_CFLAGS) $(TEST_INCLUDES) -c $< -o $@

test-clean:
	rm -rf build/test
