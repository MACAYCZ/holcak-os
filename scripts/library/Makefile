SOURCES:=$(shell find source/library/ -type f \( -name '*.c' -o -name '*.asm' \))
OBJECTS:=$(subst source/library/,build/library/,$(addsuffix .o,${SOURCES}))

.PHONY: always
always: ${OBJECTS}

ifeq (1,$(shell if [ -d build/library/ ]; then echo 1; fi))
    -include $(shell find build/library/ -type f -name '*.d')
endif

build/library/%.c.o: source/library/%.c
	@mkdir -p ${@D}
	${CC} -Wall -Wextra -pedantic -ffreestanding -MMD -fno-pie -fno-pic -nostdlib -nostdinc -c -m32 -o $@ $<

build/library/%.asm.o: source/library/%.asm
	@mkdir -p ${@D}
	${AS} -I source/library/ -felf32 -MD $(addsuffix .d,$(basename $@)) -o $@ $<
