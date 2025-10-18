#pragma once
#ifndef CONSOLE_HPP
#define CONSOLE_HPP
#define BLACK_F 0x0
#define BLUE_F 0x1
#define GREEN_F 0x2
#define CYAN_F 0x3
#define RED_F 0x4
#define PINK_F 0x5
#define ORANGE_F 0x6
#define LIGHT_GRAY_F 0x7
#define GRAY_F 0x8
#define LIGHT_BLUE_F 0x9
#define LIGHT_GREEN_F 0xA
#define WHITE_F 0xF
#define YELLOW_F 0xE
#include <cstddef>
class console{
	public:
		void out(const char* txt, unsigned char fg, unsigned char bg);
		void put_char(char c, unsigned char fg, unsigned char bg);
		void clear();
		int strcmp(const char* txt_f, const char* txt_s);
		int strncmp(const char* txt_f, const char* txt_s, size_t n);
		size_t str_len(const char* txt);
		const int vga_w = 80;
		const int vga_h = 25;
		unsigned char* vga = (unsigned char*)0xB8000;
		int col = 0;
		int row = 0;
		unsigned char defcol = WHITE_F;
		bool no_redact_prev = true;
		struct mapcolent {
			const char* inp;
			unsigned char col;
		};
		static mapcolent mapcol[];
		static const int mapcolsize;
		static const int CMD_COUNT;
		struct command {
			const char* name;
			void (*func)(console*, const char* args);
		};
		
		static void cmd_clear(console* self, const char* args);
		static void cmd_help(console* self, const char* args);
		static void cmd_col(console* self, const char* args);
		static void cmd_echo(console* self, const char* args);
		static void cmd_sysinfo(console* self, const char* args);	
		static void cmd_reboot(console* self, const char* args);
		static command commands[];
		void execute_cmd(const char* cmdline);
};
#endif
