/*
 * display_functions.c
 *
 * Created: 09.03.2019 16:25:39
 *  Author: ole
 */ 
#include <avr/io.h>
#include <stdbool.h>
#include "display_functions.h"

static bool sc_bit = SHIFTDISPLAY;
static bool shift = SHIFTDIRECTION;
static bool autoshift = ENABLE_SHIFT;
static bool cursor_blinking = false;
static bool cursor_enabled = false;
static bool power_on = false;
static uint8_t data_to_write = 0;

void Display_Enable(void){

	ENABLEPINPORT |= (1<<ENABLEPIN);
	
	#if ENABLE_WAIT == 1
	for(uint8_t x = 255; x>0;){
		x--;
	}
	#endif
		
	ENABLEPINPORT &= ~(1<<ENABLEPIN);

}
//implementation needs to be finished
void Display_Read_Enable(void){
	#if Wholeportused == 1
	
		DATADDR = 0x00;
	
	#else
	
	
	#endif


}
//implementation needs to be finished
void Display_Read_Disable(void){
	#if Wholeportused == 1
		
		DATADDR = 0xFF;
	
	#else
	
	
	#endif


}
//implementation needs to be finished
void Display_Write_Data(uint8_t data, uint8_t rs, uint8_t rw){

	Display_Write_RS_RW(rs,rw);
	#if Wholeportused == 1
		
		DATAPORT = data;
	
	#else
	
		//to be implemented
		
	
	#endif
	Display_Enable();

}
void Display_Write_RS_RW(uint8_t rs, uint8_t rw){

	if(rs == 1){
		RSPINPORT |= 1<<RSPIN;
	} else {
	if(rs == 0){
		RSPINPORT &= ~(1<<RSPIN);
		}
	}

	if(rw == 1){
		RWPINPORT <= 1<<RWPIN;
		} else {
		if(rw == 0){
			RWPINPORT &= ~(1<<RWPIN);
		}
	}
}
//implementation needs to be finished
uint8_t Display_Read_CGRAM(void){
	
	Display_Read_Enable();
	Display_Write_RS_RW(1,1);
	Display_Enable();
	
	uint8_t returndata = 0;
	#if Wholeportused == 1
		
		returndata = DATAPIN;
	
	#else
		
		//to be implemented
	
	#endif
	Display_Read_Disable();
	
	return returndata;
}
//implementation needs to be finished
struct busy Display_Read_BUSY(void){
	
	struct busy returnvalue;
	returnvalue.is_busy = 0;
	returnvalue.adresscounter = 0;
	
	Display_Read_Enable();
	Display_Write_RS_RW(0,1);
	Display_Enable();
	#if Wholeportused == 1
	
	returnvalue.is_busy = (DATAPIN>>7);
	returnvalue.adresscounter = DATAPIN&0b01111111;
	
	#else
	
	returnvalue.is_busy() = (DATAPIN7PIN>>DATAPIN7);
	
	#endif
	
	Display_Read_Disable();

	return returnvalue;
}
void Display_Set_CGRAM_Pos(uint8_t position){
	
	data_to_write = 0b01000000 | (position & 0b00111111);
	Display_Write_Data(data_to_write,0,0);

}
void Display_Set_DDRAM_Pos(uint8_t position){
	
	data_to_write = 0b10000000 | (position & 0b01111111);
	Display_Write_Data(data_to_write,0,0);
	
}
void Display_Change_Fonttable(uint8_t fonttable){
	
	data_to_write = 0b00110000 | LINECOUNT << 3 | (fonttable & 0b11);
	Display_Write_Data(data_to_write,0,0);

}
void Display_shift_Display_Or_Cursor(bool Shift_Display){
	
	
}
void Display_Create_Char(uint8_t CGRAM_Position, uint8_t character[8]){
	
	
}

void Display_Write_Char(char data){
	
	
	
}

void Display_Goto_Position(uint8_t Line, uint8_t Row){
	
	
}
void Display_Autoincrement(bool increment){
	
	
	
}
void Display_Clear(){
	
	Display_Write_Data(1,0,0);
	while(Display_Read_BUSY().is_busy){
		//wait until the display isn't busy anymore
		
	}
	
	
}
void Display_Write_String(char *to_write){
	
	
	
}
void Display_Power(bool on){
	
	data_to_write = 0b00001000 | (on<<2) (cursor_enabled<<1) | (cursor_blinking<<1);
	Display_Write_Data(data_to_write,0,0);
	
}
void Display_Setup(void){
	
	
}
void Display_Return_Home(void){
	
	Display_Write_Data(0b10,0,0);
	
}
void Display_Enable_Cursor(bool on){

	data_to_write = 0b00001000 | (power_on<<2)  (on<<1) | cursor_blinking;
	cursor_enabled = on;
	Display_Write_Data(data_to_write,0,0);	
	
}

void Display_Enable_Cursor_Blinking(bool on){
	
	data_to_write = 0b00001000 | (power_on<<2) | (cursor_enabled<<1) | on;
	cursor_blinking = on;
	Display_Write_Data(data_to_write,0,0);
	
}