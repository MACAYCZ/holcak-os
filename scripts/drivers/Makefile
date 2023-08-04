SOURCES:=$(shell find source/drivers/ -type f \( -name '*.c' -o -name '*.asm' \))
OBJECTS:=$(subst source/drivers/,build/drivers/,$(addsuffix .o,${SOURCES}))

.PHONY: always
always: ${OBJECTS}

ifeq (1,$(shell if [ -d build/drivers/ ]; then echo 1; fi))
    -include $(shell find build/drivers/ -type f -name '*.d')
endif

build/drivers/%.c.o: source/drivers/%.c
	@mkdir -p ${@D}
	${CC} -I source/library/ -Wall -Wextra -pedantic -ffreestanding -MMD -fno-pie -fno-pic -nostdlib -nostdinc -c -m32 -o $@ $<

build/drivers/%.asm.o: source/drivers/%.asm
	@mkdir -p ${@D}
	${AS} -I source/drivers/ -felf32 -MD $(addsuffix .d,$(basename $@)) -o $@ $<
