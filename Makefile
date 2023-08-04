AS:=nasm
CC:=gcc
LD:=ld

.PHONY: start clean stage1 stage2 kernel
MAKEFLAGS+=--no-print-directory

build/build.img: tools/image.py image.json stage1 stage2 kernel
	@mkdir -p ${@D}
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=build/stage1/build.bin of=$@ conv=notrunc
	./tools/image.py -c $@ $(abspath image.json)

stage1:; @${MAKE} -f scripts/stage1/Makefile AS=${AS} CC=${CC} LD=${LD}
stage2:; @${MAKE} -f scripts/stage2/Makefile AS=${AS} CC=${CC} LD=${LD}
kernel:; @${MAKE} -f scripts/kernel/Makefile AS=${AS} CC=${CC} LD=${LD}

source/driver/isrg.c source/driver/isrg.h: tools/isrg.py
	./tools/isrg.py source/driver/

start: build/build.img
	qemu-system-i386 -m 256M -drive format=raw,file=build/build.img -boot c
#	qemu-system-i386 -fda build/build.img

clean:
	rm -rf build/
