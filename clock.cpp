#include "clock.hpp"
static inline void outb(uint16_t port, uint8_t val) {
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
static inline uint8_t cmos_read(uint8_t reg){
	outb(0x70, reg);
	return inb(0x71);
}
static inline uint8_t bcd_to_bin(uint8_t val){
    return (val & 0x0F) + ((val >> 4) * 10);
}

Time return_time(){
	Time t;
	t.second = bcd_to_bin(cmos_read(0x00));
	t.minute = bcd_to_bin(cmos_read(0x02));
	t.hour = bcd_to_bin(cmos_read(0x04 & 0x7F)) + 3;
	t.day = bcd_to_bin(cmos_read(0x07));
	t.month = bcd_to_bin(cmos_read(0x08));
	t.year = bcd_to_bin(cmos_read(0x09)) + 2000;
	return t;
}
