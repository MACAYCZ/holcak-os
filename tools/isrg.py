#!/bin/python3
from sys import argv

ISR_FLAGS = "IDT_PRESENT | IDT_GATE_INT_32 | IDT_RING_0"

if len(argv) < 2:
	print(f"Usage: {argv[0]} <folder>")
	print("Error: Insufficient number of arguments were provided!")
	exit(1)

with open(f"{argv[1]}/isrg.h", "w") as f:
	f.write("#ifndef ISRG_H_\n")
	f.write("#define ISRG_H_\n")
	f.write("#include \"idt.h\"\n\n")
	for i in range(0xFF):
		f.write("__attribute__((cdecl))\n")
		f.write(f"extern void isr_{i}(void);\n\n")
	f.write("void isr_init(void);\n\n")
	f.write("#endif//ISRG_H_\n")

with open(f"{argv[1]}/isrg.c", "w") as f:
	f.write("#include \"isrg.h\"\n\n")
	f.write("void isr_init(void) {\n")
	for i in range(0xFF):
		f.write(f"\tidt_gate_init({i}, isr_{i}, {ISR_FLAGS});\n")
	f.write("}\n")
