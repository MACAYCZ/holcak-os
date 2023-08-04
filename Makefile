AS:=nasm
CC:=gcc
LD:=ld

.PHONY: start clean image stage1 stage2 kernel drivers library
MAKEFLAGS+=--no-print-directory

image: stage1 stage2 kernel build/build.img
build/build.img: tools/image.py image.json
	@mkdir -p ${@D}
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=build/stage1/build.bin of=$@ conv=notrunc
	./tools/image.py -c $@ $(abspath image.json)

stage1: build/stage1/build.bin
build/stage1/build.bin: ${shell find source/stage1/ -type f \( -name '*.asm' -o -name '*.inc' \)}
	@mkdir -p ${@D}
	${AS} -I source/stage1/ -fbin -o $@ $(filter %.asm,$^)

stage2: build/stage2/build.bin
build/stage2/build.bin: $(subst source/stage2/,build/stage2/,$(addsuffix .o,$(shell find source/stage2/ -type f \( -name '*.c' -o -name '*.asm' \))))
	@mkdir -p ${@D}
	${LD} -T scripts/stage2/linker.ld -Map build/stage2/build.map -nostdlib -melf_i386 -o $@ $(filter %.o,$^)

ifeq (1,$(shell if [ -d build/stage2/ ]; then echo 1; fi))
    -include $(shell find build/stage2/ -type f -name '*.d')
endif

build/stage2/%.c.o: source/stage2/%.c
	@mkdir -p ${@D}
	${CC} -Wall -Wextra -pedantic -ffreestanding -MMD -fno-pie -fno-pic -nostdlib -nostdinc -c -m32 -o $@ $<

build/stage2/%.asm.o: source/stage2/%.asm
	@mkdir -p ${@D}
	${AS} -I source/stage2/ -felf32 -MD $(addsuffix .d,$(basename $@)) -o $@ $<

kernel: drivers library build/kernel/build.bin
build/kernel/build.bin: $(subst source/kernel/,build/kernel/,$(addsuffix .o,$(shell find source/kernel/ -type f \( -name '*.c' -o -name '*.asm' \))))
	@mkdir -p ${@D}	
	${LD} -T scripts/kernel/linker.ld -Map build/kernel/build.map -nostdlib -melf_i386 -o $@ $(filter %.o,$^) $(shell find build/drivers/ build/library/ -type f -name '*.o')

ifeq (1,$(shell if [ -d build/kernel/ ]; then echo 1; fi))
    -include $(shell find build/kernel/ -type f -name '*.d')
endif

build/kernel/%.c.o: source/kernel/%.c
	@mkdir -p ${@D}
	${CC} -I source/drivers/ -I source/library/ -Wall -Wextra -pedantic -ffreestanding -MMD -fno-pie -fno-pic -nostdlib -nostdinc -c -m32 -o $@ $<

build/kernel/%.asm.o: source/kernel/%.asm
	@mkdir -p ${@D}
	${AS} -I source/kernel/ -felf32 -MD $(addsuffix .d,$(basename $@)) -o $@ $<

drivers: $(subst source/drivers/,build/drivers/,$(addsuffix .o,$(shell find source/drivers/ -type f \( -name '*.c' -o -name '*.asm' \))))
ifeq (1,$(shell if [ -d build/drivers/ ]; then echo 1; fi))
    -include $(shell find build/drivers/ -type f -name '*.d')
endif

build/drivers/%.c.o: source/drivers/%.c
	@mkdir -p ${@D}
	${CC} -I source/library/ -Wall -Wextra -pedantic -ffreestanding -MMD -fno-pie -fno-pic -nostdlib -nostdinc -c -m32 -o $@ $<

build/drivers/%.asm.o: source/drivers/%.asm
	@mkdir -p ${@D}
	${AS} -I source/drivers/ -felf32 -MD $(addsuffix .d,$(basename $@)) -o $@ $<

source/drivers/isrg.c source/drivers/isrg.h: tools/isrg.py
	./tools/isrg.py source/drivers/

library: $(subst source/library/,build/library/,$(addsuffix .o,$(shell find source/library/ -type f \( -name '*.c' -o -name '*.asm' \))))
ifeq (1,$(shell if [ -d build/library/ ]; then echo 1; fi))
    -include $(shell find build/library/ -type f -name '*.d')
endif

build/library/%.c.o: source/library/%.c
	@mkdir -p ${@D}
	${CC} -Wall -Wextra -pedantic -ffreestanding -MMD -fno-pie -fno-pic -nostdlib -nostdinc -c -m32 -o $@ $<

build/library/%.asm.o: source/library/%.asm
	@mkdir -p ${@D}
	${AS} -I source/library/ -felf32 -MD $(addsuffix .d,$(basename $@)) -o $@ $<

start: image
#	qemu-system-i386 -m 256M -drive format=raw,file=build/build.img -boot c
	qemu-system-i386 -fda build/build.img

clean:
	rm -rf build/
