.PHONY: stage1
stage1: build/stage1/build.bin

build/stage1/build.bin: ${shell find source/stage1/ -type f \( -name '*.asm' -o -name '*.inc' \)}
	@mkdir -p ${@D}
	${AS} -I source/stage1/ -fbin -o $@ $(filter %.asm,$^)
