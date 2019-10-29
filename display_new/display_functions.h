/*
 * display_functions.h
 *
 * Created: 09.03.2019 16:27:16
 *  Author: ole
 */ 

#include <stdbool.h>
#include <avr/io.h>

#ifndef DISPLAY_FUNCTIONS_H_
#define DISPLAY_FUNCTIONS_H_


//DEFINES
#define CURSOR_BLINKING  false
#define CURSOR_ENABLED  false

//Config defines
//0 for 1 line displays
//1 for 2 line and 4 line Displays
#define LINECOUNT 1
//0b00 english japanese
//0b01 english russian
//0b10 western european I
//0b11 western european II
#define FONTTABLE 0b10
//autoincrement status
//0 is off
//1 is on
#define INCREMENT 1
//shift direction
//1 = shift right
//0 = shift left
#define SHIFTDIRECTION 1
//enable shift
//0 = disabled
//1 = enabled
#define ENABLE_SHIFT 0
//shift cursor or Display
//0 = shift Cursor
//1 = shift Display
#define SHIFTDISPLAY 1
//should wait after setting enablepin?
//1 yes
//0 no
#define ENABLE_WAIT 1


//PIN Defines

//RS Pin connceted to display
#define RSPIN PC0
#define RSPINPORT PORTC
//RW Pin connected to display
#define RWPIN PC1
#define RWPINPORT PORTC
//Enable Pin connected to display
#define ENABLEPIN PC2
#define ENABLEPINPORT PORTC

//is a whole port for display Communication used?
//decides if we are using Display_Write_Data()for a single port or for split/mirrored port
#define Wholeportused 1

#if Wholeportused == 1
//if you use a whole port for comunication with the display it is enough to define the dataport and use the Display_Write_Data() function
	#define DATAPORT PORTE
	#define DATADDR DDRE
	#define DATAPIN PINE
#else
	//if the dataport is mirrored or split onto different port it requires the definition of the following to
	//communicate with the display and the use of the dunction Display_Write_Data_Individual() instead of Display_Write_Data()
	#define DATAPIN0 PD0
	#define DATAPIN0PORT PORTD
	#define DATAPIN0DDR DDRD
	#define DATAPIN0PIN PIND
	#define DATAPIN1 PD1
	#define DATAPIN1PORT PORTD
	#define DATAPIN1DDR DDRD
	#define DATAPIN1PIN PIND
	#define DATAPIN2 PD2
	#define DATAPIN2PORT PORTD
	#define DATAPIN2DDR DDRD
	#define DATAPIN2PIN PIND
	#define DATAPIN3 PD3
	#define DATAPIN3PORT PORTD
	#define DATAPIN3DDR DDRD
	#define DATAPIN3PIN PIND
	#define DATAPIN4 PD4
	#define DATAPIN4PORT PORTD
	#define DATAPIN4DDR DDRD
	#define DATAPIN4PIN PIND
	#define DATAPIN5 PD5
	#define DATAPIN5PORT PORTD
	#define DATAPIN5DDR DDRD
	#define DATAPIN5PIN PIND
	#define DATAPIN6 PD6
	#define DATAPIN6PORT PORTD
	#define DATAPIN6DDR DDRD
	#define DATAPIN6PIN PIND
	#define DATAPIN7 PD7
	#define DATAPIN7PORT PORTD
	#define DATAPIN7DDR DDRD
	#define DATAPIN7PIN PIND
#endif // Wholeportused

//Struct for Busyflag and Address counter

struct busy{
	
		bool is_busy;
		uint8_t adresscounter;
};

//the "raw" function to interact with the display
//you should only use them if you know what you are doing
void Display_Write_Data(uint8_t data, uint8_t rs, uint8_t rw);
uint8_t Display_Read_CGRAM(void);
struct busy Display_Read_BUSY();
void Display_Set_CGRAM_Pos(uint8_t position);
void Display_Set_DDRAM_Pos(uint8_t position);
void Display_Write_RS_RW(uint8_t rs, uint8_t rw);
void Display_Enable(void);
void Display_Read_Enable(void);
void Display_Read_Disable(void);

//
//the "higher" logic functions to interact with the display
//

void Display_Write_Char(char data);
void Display_Write_String(char *to_write);
void Display_Create_Char(uint8_t CGRAM_Position, uint8_t character[8]);
void Display_Clear(void);

//
//Display Navigation Functions
//

//return to position 0,0 (the beginning of the display)
void Display_Return_Home(void);
void Display_Goto_Position(uint8_t Line, uint8_t Row);

//
//Display Help Functions
//
void Display_Write_Large_Number(uint8_t start_position, uint8_t number);
void Display_Large_Number_Helper(uint8_t start_position, uint8_t firstline_0, uint8_t firstline_1, uint8_t firstline_2, uint8_t secondline_0, uint8_t secondline_1, uint8_t secondline_2,
								 uint8_t thirdline_0, uint8_t thirdline_1, uint8_t thirdline_2, uint8_t fourthline_0, uint8_t fourthline_1, uint8_t fourthline_2);

//
//Display Config Functions
//

void Display_Autoincrement(bool increment);
void Display_Power(bool on);
void Display_Enable_Cursor(bool on);
void Display_Enable_Cursor_Blinking(bool on);
//setup the display with standard values declared in the define
void Display_Setup(void);
void Display_Change_Fonttable(uint8_t fonttable);
void Display_Shift_Display_Or_Cursor(bool Shift_Display);

#endif /* DISPLAY_FUNCTIONS_H_ */