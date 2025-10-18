#include "console.hpp"
#include "keyboard.hpp"
extern "C" void kernel_main(void){
        console std;
	
        std.out("Welcome to BuizlOS.                                                                 \n", WHITE_F, CYAN_F);
	std.out("Author: Dvoriak.                                                                \n", BLACK_F, WHITE_F);
	char input_buffer[128];
	int input_buffer_len = 0;
	while (true){
		uint8_t keyboard_code = keyboard_read_scan_code();
		char cur_act_key_char = keyboard_translate(keyboard_code);
		if (cur_act_key_char){
			char cur_act_key_char_temp[2];
			cur_act_key_char_temp[0] = cur_act_key_char;
			cur_act_key_char_temp[1] = '\0';
			std.put_char(*cur_act_key_char_temp, std.defcol, BLACK_F);
		}
		if (cur_act_key_char && cur_act_key_char != '\n' && cur_act_key_char != '\b'){
			if (input_buffer_len < sizeof(input_buffer) - 1){
				input_buffer[input_buffer_len++] = cur_act_key_char;
			}
		}
		if (cur_act_key_char == '\n'){
		 input_buffer[input_buffer_len] = '\0';  
            	std.execute_cmd(input_buffer);
		    input_buffer_len = 0;    
		  }
	}
}
