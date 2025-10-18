#pragma once
#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP
#include <cstdint>
extern uint8_t keyboard_read_scan_code();
extern char keyboard_translate(uint8_t scan_code);
extern void keyboard_init();

#endif
