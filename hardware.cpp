#include "hardware.hpp"
#include <cstdint>
void return_cpu_vendor(char* vendor){
	uint32_t eax, ebx, ecx, edx;
	eax = 0;
	asm volatile("cpuid" : "=b"(ebx), "=d"(edx), "=c"(ecx) : "a"(eax));
	*(uint32_t*)&vendor[0] = ebx;
	*(uint32_t*)&vendor[4] = edx;
	*(uint32_t*)&vendor[8] = ecx;
	vendor[12] = '\0';
}
