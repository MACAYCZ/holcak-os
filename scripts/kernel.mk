SOURCES:=$(shell find source/kernel/ -type f \( -name '*.c' -o -name '*.asm' \))
OBJECTS:=$(subst source/kernel/,build/kernel/,$(addsuffix .o,${SOURCES}))

.PHONY: kernel
kernel: build/kernel/build.bin

ifeq (1,$(shell if [ -d build/kernel/ ]; then echo 1; fi))
    -include $(shell find build/kernel/ -type f -name '*.d')
endif

build/kernel/build.bin: driver library ${OBJECTS}
	@mkdir -p ${@D}
	${LD} -T scripts/kernel.ld -Map build/kernel/build.map -nostdlib -melf_i386 -o $@ ${OBJECTS}

build/kernel/%.c.o: source/kernel/%.c
	@mkdir -p ${@D}
	${CC} -I source/driver/ -I source/library/ -Wall -Wextra -pedantic -ffreestanding -MMD -fno-pie -fno-pic -nostdlib -nostdinc -c -m32 -o $@ $<

build/kernel/%.asm.o: source/kernel/%.asm
	@mkdir -p ${@D}
	${AS} -I source/kernel/ -felf32 -MD $(addsuffix .d,$(basename $@)) -o $@ $<
