#include "keyboard.hpp"
static inline uint8_t inb(uint16_t port){
	uint8_t ret;
	asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}
static inline void outb(uint16_t port, uint8_t val){
	asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}
static const char scan_code_map[128] = {
    0,    27, '1', '2', '3', '4', '5', '6', '7', '8',  '9', '0', '-', '=', '\b', 
    '\t','q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o',  'p', '[', ']', '\n', 0,   
    'a',  's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',  '\'', '`', 0,    '\\', 'z',
    'x',  'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,   '*', 0,    ' ', 0,   0,    
    0,    0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2',  '3', '0', '.', 0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
};
uint8_t keyboard_read_scan_code(){
	while (!(inb(0x64) & 1)) {}
	return inb(0x60);
}

char keyboard_translate(uint8_t code){
	if (code & 0x80) return 0;
	return scan_code_map[code];
}

