/*
 * display_functions.c
 *
 * Created: 09.03.2019 16:25:39
 *  Author: ole
 */ 
#include <avr/io.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "display_functions.h"

static bool sc_bit = SHIFTDISPLAY;
static bool shift = SHIFTDIRECTION;
static bool autoshift = ENABLE_SHIFT;
static uint8_t cursor_blinking = CURSOR_BLINKING;
static bool cursor_enabled = CURSOR_ENABLED;
static bool power_on = false;
static uint8_t data_to_write = 0;

uint8_t dsp_line[4] ={
	//This order is known by Trial and Error
	0x80, //Line 1
	0xC0, //Line 4
	0x94, //Line 2
	0xD4, //Line 3
};

/**
 * @brief Strobe the enable pin
 * 
 */
void Display_Enable(void){

	ENABLEPINPORT |= (1<<ENABLEPIN);
	
	#if ENABLE_WAIT == 1
	for(uint8_t x = 255; x>0;){
		x--;
	}
	#endif
		
	ENABLEPINPORT &= ~(1<<ENABLEPIN);

}

/**
 * @brief Put data port into read-mode
 * 
 */
void Display_Read_Enable(void){
	#if Wholeportused == 1
	
		DATADDR = 0x00;
	
	#else
		//TODO implementation needs to be finished
	#endif


}

/**
 * @brief Put data port into write-mode
 * 
 */
void Display_Read_Disable(void){
	#if Wholeportused == 1
		
		DATADDR = 0xFF;
	
	#else
		//TODO implementation needs to be finished
	#endif


}

/**
 * @brief Write data to the display
 * 
 * @param data data to be written
 * @param rs state of RS pin
 * @param rw state of RW pin
 */
void Display_Write_Data(uint8_t data, uint8_t rs, uint8_t rw){

	Display_Write_RS_RW(rs,rw);
	#if Wholeportused == 1
		
		DATAPORT = data;
	
	#else
		//TODO implementation needs to be finished
	#endif
	Display_Enable();

}

/**
 * @brief Set the state of the RS and RW pins
 * 
 * @param rs state of RS pin
 * @param rw state of RW pin
 */
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

/**
 * @brief Read from the CGRAM
 * 
 * @return uint8_t read byte
 */
uint8_t Display_Read_CGRAM(void){
	
	Display_Read_Enable();
	Display_Write_RS_RW(1,1);
	Display_Enable();
	
	uint8_t returndata = 0;
	#if Wholeportused == 1
		
		returndata = DATAPIN;
	
	#else
		//TODO implementation needs to be finished
	#endif
	Display_Read_Disable();
	
	return returndata;
}


/**
 * @brief Check busy state of the display
 * 
 * @return struct busy busy-information of the display
 */
struct busy Display_Read_BUSY(void){
	//TODO implementation needs to be finished	
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

/**
 * @brief Set the CGRAM (character generator RAM) position of the display controller
 * 
 * @param position CGRAM position/address
 */
void Display_Set_CGRAM_Pos(uint8_t position){
	
	data_to_write = 0b01000000 | (position & 0b00111111);
	Display_Write_Data(data_to_write,0,0);

}

/**
 * @brief Set the DDRAM (display data RAM) position of the display controller
 * 
 * @param position DDRAM position/address
 */
void Display_Set_DDRAM_Pos(uint8_t position){
	
	data_to_write = 0b10000000 | (position & 0b01111111);
	Display_Write_Data(data_to_write,0,0);
	
}

/**
 * @brief Change the font table to the selected character set
 * 
 * @param fonttable ID of the font table/character set
 */
void Display_Change_Fonttable(uint8_t fonttable){
	
	data_to_write = 0b00110000 | LINECOUNT << 3 | (fonttable & 0b11);
	Display_Write_Data(data_to_write,0,0);

}

/**
 * @brief Select whether display of cursor get's shifted
 * 
 * @param Shift_Display display get's shifted if true
 */
void Display_shift_Display_Or_Cursor(bool Shift_Display){
	
	
}

/**
 * @brief Create and store a custom character
 * 
 * @param CGRAM_Position position of the character in CGRAM
 * @param character array containing all lines of the character
 */
void Display_Create_Char(uint8_t CGRAM_Position, uint8_t character[8]){
	
	Display_Write_Data(0x40+8*CGRAM_Position,0,0);

	//write each line
	for(uint8_t x = 0; x < 8; x++){
		
		Display_Write_Data(character[x],1,0);
		
	}
	
}

/**
 * @brief Write character to display
 * 
 * @param data character to be written
 */
void Display_Write_Char(char data){
	
	Display_Write_Data(data,1,0);
	
}

/**
 * @brief Go to cursor position
 * 
 * @param Line target line
 * @param Row target row
 */
void Display_Goto_Position(uint8_t Line, uint8_t Row){
	Display_Write_Data(dsp_line[Line]+Row, 0, 0);	
}

/**
 * @brief Set whether the cursor position auto-increments on character-writes
 * 
 * @param increment auto-increment if true
 */
void Display_Autoincrement(bool increment){
	
	
	
}

#include <avr/delay.h>

/**
 * @brief Clear entire display area
 * 
 */
void Display_Clear(){
	
	Display_Write_Data(1,0,0);
	while(Display_Read_BUSY().is_busy){
		//wait until the display isn't busy anymore
		
	}
	_delay_ms(20); // Hacky fix since is_busy DOES NOT WORK!!!
	//Display_Write_Data(0x02, 0, 0);  // Go home
	
	
}

/**
 * @brief Write string to display
 * 
 * @param to_write String to be written (pointer to char-array)
 */
void Display_Write_String(char *to_write){
	
	for(size_t x = 0; x < strlen(to_write); x++){
		
		Display_Write_Char(to_write[x]);
		
	}
	
}

/**
 * @brief Set the display power-state
 * 
 * @param on display turned on if true
 */
void Display_Power(bool on){
	
	data_to_write = 0b00001000 | (on<<2) | (cursor_enabled<<1) | (cursor_blinking<<1);
	Display_Write_Data(data_to_write,0,0);
	
}

/**
 * @brief Setup the display
 * 
 */
void Display_Setup(void){
	Display_Write_Data(0x39,0,0);	//European character set
	Display_Write_Data(0x08,0,0);	//Display off
	Display_Write_Data(0x06,0,0);	//Increment cursor by 1 and don't shift display
	Display_Write_Data(0x17,0,0);	//Character mode, internal power on
	Display_Write_Data(0x02,0,0);	//Return home
	Display_Write_Data(0x0C,0,0);	//Display on
	
}

/**
 * @brief Set cursor position to (0,0)
 * 
 */
void Display_Return_Home(void){
	
	Display_Write_Data(0b10,0,0);
	
}

/**
 * @brief Set cursor-display status
 * 
 * @param on Cursor visible if true
 */
void Display_Enable_Cursor(bool on){

	data_to_write = 0b00001000 | (power_on<<2) | (on<<1) | cursor_blinking;
	cursor_enabled = on;
	Display_Write_Data(data_to_write,0,0);	
	
}

/**
 * @brief Set cursor blinking status
 * 
 * @param on Cursor blinks if true
 */
void Display_Enable_Cursor_Blinking(bool on){
	
	data_to_write = 0b00001000 | (power_on<<2) | (cursor_enabled<<1) | on;
	cursor_blinking = on;
	Display_Write_Data(data_to_write,0,0);
	
}

/**
 * @brief Create and store a custom character
 * 
 * @param charpos position of character in CGRAM
 * @param line1 line 1 of character graphic
 * @param line2 line 2 of character graphic
 * @param line3 line 3 of character graphic
 * @param line4 line 4 of character graphic
 * @param line5 line 5 of character graphic
 * @param line6 line 6 of character graphic
 * @param line7 line 7 of character graphic
 * @param line8 line 8 of character graphic
 */
void Display_Define_Custom_Character(
	uint8_t charpos, uint8_t line1, uint8_t line2, uint8_t line3, uint8_t line4, uint8_t line5, uint8_t line6, uint8_t line7, uint8_t line8
) {
    /*
     * TODO: Evaluate this function and improve signature.
     *
     * */
//TODO difference to void Display_Create_Char(uint8_t CGRAM_Position, uint8_t character[8])?

    //where to write in cgram
    Display_Write_Data(0x40+8*charpos, 0, 0);

    //write each line
    Display_Write_Data(line1, 1, 0);
    Display_Write_Data(line2, 1, 0);
    Display_Write_Data(line3, 1, 0);
    Display_Write_Data(line4, 1, 0);
    Display_Write_Data(line5, 1, 0);
    Display_Write_Data(line6, 1, 0);
    Display_Write_Data(line7, 1, 0);
    Display_Write_Data(line8, 1, 0);

    //dsiplay return home
    Display_Write_Data(0x02, 0, 0);
}


/*
uint8_t dsp_command [7] = {

	0x39, //function set european chararacter set

	0x08, //display off

	0x06, //entry mode set increment cursor by 1 not shifting display

	0x17, //Character mode and internal power on

	0x00, //0x01, //clear display NEVER USE CLS IT WILL GET THE DISPLAY HOT SICNE EXECUTION TIME >2 ms

	0x02, //return home

	0x0C, //display on

};
*/



