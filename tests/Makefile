.PHONY: build start clean
# TODO: Add holcak-os!

build:
	(cd holcak-bl && make build TARGET=i686-elf)
	dd if=/dev/zero of=build.img bs=512 count=65536
	./holcak-bl/script/install.py build.img holcak-bl/build/
	echo 'start=2048, type=83' | sfdisk build.img

start: build
#	qemu-system-i386 -M q35 -m 256M -drive format=raw,file=build.img,media=disk -boot c
	qemu-system-i386 -M pc -m 256M -drive format=raw,file=build.img,media=disk -boot c
#	qemu-system-i386 -M pc -m 256M -drive format=raw,if=floppy,file=build.img

debug: build
	qemu-system-i386 -M pc -m 256M -drive format=raw,file=build.img,media=disk -boot c -s -S &
	gdb -ex "target remote localhost:1234" -ex "symbol-file holcak-bl/build/stage2/build.elf"

clean:
	(cd holcak-bl && make clean TARGET=i686-elf)
	rm -f build.img
