#pragma once 
#ifndef CLOCK_HPP
#define CLOCK_HPP
#include <cstdint>
struct Time{
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint8_t month;
	uint16_t year;
};
extern Time return_time();
#endif
