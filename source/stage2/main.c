_Noreturn void main(void) {
	*((char*)0xB8000) = 'A';
	__asm__ volatile ("cli");
	while (1);
}
