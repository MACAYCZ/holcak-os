SOURCES:=$(shell find source/stage2/ -type f \( -name '*.c' -o -name '*.asm' \))
OBJECTS:=$(subst source/stage2/,build/stage2/,$(addsuffix .o,${SOURCES}))

build/stage2/build.bin: ${OBJECTS}
	@mkdir -p ${@D}
	${LD} -T scripts/stage2/linker.ld -Map build/stage2/build.map -nostdlib -melf_i386 -o $@ ${OBJECTS}

ifeq (1,$(shell if [ -d build/stage2/ ]; then echo 1; fi))
    -include $(shell find build/stage2/ -type f -name '*.d')
endif

build/stage2/%.c.o: source/stage2/%.c
	@mkdir -p ${@D}
	${CC} -Wall -Wextra -pedantic -ffreestanding -MMD -fno-pie -fno-pic -nostdlib -nostdinc -c -m32 -o $@ $<

build/stage2/%.asm.o: source/stage2/%.asm
	@mkdir -p ${@D}
	${AS} -I source/stage2/ -felf32 -MD $(addsuffix .d,$(basename $@)) -o $@ $<
