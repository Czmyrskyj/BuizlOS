#include "console.hpp"
#include "buizl_logo.h"
#include <cstdint>
static inline void outb(uint16_t port, uint8_t val) {
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

console::mapcolent console::mapcol[]  = {
			{"red", RED_F},
			{"blue", BLUE_F},
			{"black", BLACK_F},
			{"white", WHITE_F},
			{"green", GREEN_F},
			{"yellow", YELLOW_F},
			{"cyan", CYAN_F}
		};
uint32_t char_map(uint32_t c){
    switch(c){
       case 0x2588: return 0xDB; 
        case 0x2593: return 0xB2; 
        case 0x2592: return 0xB1; 
        case 0x2591: return 0xB0;  
        
        default:
            if (c >= 0 && c <= 127) return c;
            return '?';
    }
}
void console::cmd_clear(console* self, const char* args){
	self->clear();
};
void console::cmd_echo(console* self, const char* args){
	if (!args){ self->out("Echo couldn't find an arguments.\n", WHITE_F, RED_F); return; }
	self->out(args, WHITE_F, BLACK_F);
	self->out("\n", WHITE_F, BLACK_F);
}

void console::cmd_sysinfo(console* self, const char* args){
	self->clear();
	self->out((const char*)buizl_logo_txt, self->defcol, BLACK_F);
}
void console::cmd_help(console* self, const char*) {
    self->out("BuizlOS shell syntax:\n", WHITE_F, GRAY_F);
    self->out("help: prints all available commands.\n", WHITE_F, BLACK_F);
    self->out("clear: clears the screen.\n", WHITE_F, BLACK_F);    
    self->out("echo: prints the given arguments.\n", WHITE_F, BLACK_F);
    self->out("reboot: reboots the system.\n", WHITE_F, BLACK_F);
    self->out("col: changes text to the color specified by the following argument.\n", WHITE_F, BLACK_F);
    self->out("sysinfo: prints an info about your personal computer, such as CPU, GPU, RAM\n", WHITE_F, BLACK_F);
}

void console::cmd_col(console* self, const char* args){
if (!args || !*args) return;

    for (int i = 0; i < self->mapcolsize; ++i) {
        if (self->strncmp(args, self->mapcol[i].inp, 16) == 0) {
            self->defcol = self->mapcol[i].col;
            return;
        }
    }

    self->out("Unknown color.\n", RED_F, 0);	
}
console::command console::commands[] = {
	{"clear", &console::cmd_clear},
	{"echo", &console::cmd_echo},
	{"help", &console::cmd_help},
	{"col", &console::cmd_col},
	{"sysinfo", &console::cmd_sysinfo},
	{"reboot", &console::cmd_reboot}
};
const int console::mapcolsize = sizeof(console::mapcol)/sizeof(console::mapcol[0]);
const int console::CMD_COUNT = sizeof(console::commands)/sizeof(console::commands[0]);
size_t console::str_len(const char* txt){
	int pos = 0;
	while (txt[pos] != '\0'){
		pos++;
	}
	return pos;
}
void console::out(const char* txt, unsigned char fg, unsigned char bg){
	unsigned char clr = (bg << 4) | (fg & 0x0F);
	for (int char_pos = 0; txt[char_pos] != '\0'; char_pos++){
		if (txt[char_pos] == '\n'){
			row++;
			col = 0;
			continue;
		}
		vga[(row * vga_w + col) * 2] = txt[char_pos];
		vga[(row * vga_w + col) * 2 + 1] = clr;
		col++;
	}
}
int console::strcmp(const char* a, const char* b){
	while (*a && (*a == *b)){
		a++;
		b++;
	}
	return *(const unsigned char*)a - *(const unsigned char*)b;
}
int console::strncmp(const char* txt_f, const char* txt_s, size_t n){
	for (int i = 0; i < n; i++){
		if (txt_f[i] != txt_s[i] || txt_f[i] != '\0' || txt_f[i] != '\0')
			return (unsigned char)txt_f[i] - (unsigned char)txt_s[i];
		}
	return 0;
}
void console::put_char(char c, unsigned char fg, unsigned char bg){
	if (c == '\n'){
		row++;
		col = 0;
		return;
	}
	if (c == '\b'){
		if (col > 0){
			col--;
			vga[(row * vga_w + col) * 2] = ' ';
			vga[(row * vga_w + col) * 2 + 1] = bg;
		} else if (!no_redact_prev && row > 0) {
			row--;
			col = 0;
			for (int x = vga_w - 1; x >= 0; x--){
				if (vga[(row * vga_w + x) * 2] != ' '){
					col = x + 1;
					break;
				}
			}
		}
		return;
	}
	vga[(row * vga_w + col) * 2] = c;
	vga[(row * vga_w + col) * 2 + 1] = (bg << 4) | (fg & 0x0F);
	col++;
	if (col >= vga_w){
		col = 0;
		row++;
	}
}
void console::clear(){
	for (int y = 0; y < vga_h; y++){
		for (int x = 0; x < vga_w; x++){
			vga[(y * vga_w + x) * 2] = ' ';
			vga[(y * vga_w + x) * 2 + 1] = 0x07;
		}
	}
	col = 0;
	row = 0;
}
void console::cmd_reboot(console* self, const char* args){
	while (inb(0x64) & 0x02) {}
	outb(0x64, 0xFE);
}
void console::execute_cmd(const char* cmdline) {
 
    const char* space = nullptr;
    for (int i = 0; cmdline[i]; i++) {
        if (cmdline[i] == ' ') {
            space = cmdline + i;
            break;
        }
    }

    const char* cmd = cmdline;
    const char* args = nullptr;
    if (space) {
        args = space + 1;
        size_t cmd_len = space - cmdline;
        char temp[64];
        for (size_t i = 0; i < cmd_len; i++) temp[i] = cmd[i];
        temp[cmd_len] = '\0';
        cmd = temp;
    }

    for (int i = 0; i < CMD_COUNT; i++) {
        if (strcmp(cmd, commands[i].name) == 0) {
            commands[i].func(this, args); // ✅ pass console instance + arguments
            return;
        }
    }

    out("Unknown command\n", WHITE_F, RED_F);
}

