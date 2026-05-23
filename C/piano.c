#include <stdint.h>
#include "speaker.h"
#include "port.h"

uint8_t get_note(void){
	return inb(0x60);
}

uint8_t test_status(void){
	if(inb(0x64) & 2){
		return 1;
	}
	else{
		return 0;
	}
}

void play_piano(void){
	do{
		
		if(get_note() == 0x20){
			Do();
		  }
		if(get_note() == 0x13){
			Re();
		  }
	}while(test_status());
}
