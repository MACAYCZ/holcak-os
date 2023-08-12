AS:=nasm
CC:=i686-elf-gcc
LD:=i686-elf-ld
OBJCOPY:=i686-elf-objcopy

.PHONY: start clean
MAKEFLAGS+=--no-print-directory
CSTANDARD:=c17

DRIVERS:=source/drivers/isrg.c $(subst source/drivers/,build/drivers/,$(addsuffix .o,$(shell find source/drivers/ -type f \( -name '*.c' -o -name '*.asm' \))))
LIBRARY:=$(subst source/library/,build/library/,$(addsuffix .o,$(shell find source/library/ -type f \( -name '*.c' -o -name '*.asm' \))))

build/build.img: tools/image.py image.json build/stage1/build.bin build/stage2/build.bin build/stage3/build.bin build/kernel/build.bin
	@mkdir -p ${@D}
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=build/stage1/build.bin of=$@ conv=notrunc
	./tools/image.py -c $@ $(abspath image.json)

build/stage1/build.bin: ${shell find source/stage1/ -type f \( -name '*.asm' -o -name '*.inc' \)}
	@mkdir -p ${@D}
	${AS} -I source/stage1/ -fbin -o $@ $(filter %.asm,$^)

build/stage2/build.bin: ${shell find source/stage2/ -type f \( -name '*.asm' -o -name '*.inc' \)}
	@mkdir -p ${@D}
	${AS} -I source/stage1/ -I source/stage2/ -fbin -o $@ $(filter %.asm,$^)

build/stage3/build.bin: ${DRIVERS} ${LIBRARY} $(subst source/stage3/,build/stage3/,$(addsuffix .o,$(shell find source/stage3/ -type f \( -name '*.c' -o -name '*.asm' \))))
	@mkdir -p ${@D}
	${LD} -T scripts/stage3/linker.ld --gc-sections -Map build/stage3/build.map -nostdlib -melf_i386 -o build/stage3/build.elf $(filter %.o,$^)
	${OBJCOPY} -O binary build/stage3/build.elf $@

ifeq (1,$(shell if [ -d build/stage3/ ]; then echo 1; fi))
    -include $(shell find build/stage3/ -type f -name '*.d')
endif

build/stage3/%.c.o: source/stage3/%.c
	@mkdir -p ${@D}
	${CC} -I source/drivers/ -I source/library/ -nostartfiles -Wall -Wextra -pedantic -ffreestanding -MMD -fno-pie -fno-pic -nostdlib -nostdinc -c -m32 -o $@ $<

build/stage3/%.asm.o: source/stage3/%.asm
	@mkdir -p ${@D}
	${AS} -I source/stage3/ -std=${CSTANDARD} -felf32 -MD $(addsuffix .d,$(basename $@)) -o $@ $<

build/kernel/build.bin: ${DRIVERS} ${LIBRARY} $(subst source/kernel/,build/kernel/,$(addsuffix .o,$(shell find source/kernel/ -type f \( -name '*.c' -o -name '*.asm' \))))
	@mkdir -p ${@D}	
	${LD} -T scripts/kernel/linker.ld --gc-sections -Map build/kernel/build.map -nostdlib -melf_i386 -o build/kernel/build.elf $(filter %.o,$^)
	${OBJCOPY} -O binary build/kernel/build.elf $@

ifeq (1,$(shell if [ -d build/kernel/ ]; then echo 1; fi))
    -include $(shell find build/kernel/ -type f -name '*.d')
endif

build/kernel/%.c.o: source/kernel/%.c
	@mkdir -p ${@D}
	${CC} -I source/drivers/ -I source/library/ -std=${CSTANDARD} -Wall -Wextra -pedantic -ffreestanding -MMD -fno-pie -fno-pic -nostdlib -nostdinc -c -m32 -o $@ $<

build/kernel/%.asm.o: source/kernel/%.asm
	@mkdir -p ${@D}
	${AS} -I source/kernel/ -felf32 -MD $(addsuffix .d,$(basename $@)) -o $@ $<

ifeq (1,$(shell if [ -d build/drivers/ ]; then echo 1; fi))
    -include $(shell find build/drivers/ -type f -name '*.d')
endif

build/drivers/%.c.o: source/drivers/%.c
	@mkdir -p ${@D}
	${CC} -I source/drivers/ -I source/library/ -std=${CSTANDARD} -Wall -Wextra -pedantic -ffreestanding -MMD -fno-pie -fno-pic -nostdlib -nostdinc -c -m32 -o $@ $<

build/drivers/%.asm.o: source/drivers/%.asm
	@mkdir -p ${@D}
	${AS} -I source/drivers/ -felf32 -MD $(addsuffix .d,$(basename $@)) -o $@ $<

source/drivers/isrg.c source/drivers/isrg.h: tools/isrg.py
	./tools/isrg.py source/drivers/

ifeq (1,$(shell if [ -d build/library/ ]; then echo 1; fi))
    -include $(shell find build/library/ -type f -name '*.d')
endif

build/library/%.c.o: source/library/%.c
	@mkdir -p ${@D}
	${CC} -I source/library/ -std=${CSTANDARD} -Wall -Wextra -pedantic -ffreestanding -MMD -fno-pie -fno-pic -nostdlib -nostdinc -c -m32 -o $@ $<

build/library/%.asm.o: source/library/%.asm
	@mkdir -p ${@D}
	${AS} -I source/library/ -felf32 -MD $(addsuffix .d,$(basename $@)) -o $@ $<

start: build/build.img
#	qemu-system-i386 -m 256M -drive format=raw,file=build/build.img -boot c
	qemu-system-i386 -m 256M -drive format=raw,if=floppy,file=build/build.img

clean:
	rm -rf build/
