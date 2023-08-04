SOURCES:=$(shell find source/driver/ -type f \( -name '*.c' -o -name '*.asm' \))
OBJECTS:=$(subst source/driver/,build/driver/,$(addsuffix .o,${SOURCES}))

.PHONY: driver
driver: ${OBJECTS}

ifeq (1,$(shell if [ -d build/driver/ ]; then echo 1; fi))
    -include $(shell find build/driver/ -type f -name '*.d')
endif

build/driver/%.c.o: source/driver/%.c
	@mkdir -p ${@D}
	${CC} -Wall -Wextra -pedantic -ffreestanding -MMD -fno-pie -fno-pic -nostdlib -nostdinc -c -m32 -o $@ $<

build/driver/%.asm.o: source/driver/%.asm
	@mkdir -p ${@D}
	${AS} -I source/driver/ -felf32 -MD $(addsuffix .d,$(basename $@)) -o $@ $<