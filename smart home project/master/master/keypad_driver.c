
/*
 * keypad_driver.c
 *
 * Created: 12/12/2020 11:58:16 م
 *  Author: ascom
 */ 
#include "keypad_driver.h"
void keypad_vInit(void)
{
	DIO_vsetPINDir(KEYPAD_PORT,KEYPAD_FIRST_PIN,1);
	DIO_vsetPINDir(KEYPAD_PORT,KEYPAD_SECOND_PIN,1);
	DIO_vsetPINDir(KEYPAD_PORT,KEYPAD_THIRD_PIN,1);
	DIO_vsetPINDir(KEYPAD_PORT,KEYPAD_FOURTH_PIN,1);
	DIO_vsetPINDir(KEYPAD_PORT,KEYPAD_FIFTH_PIN,0);
	DIO_vsetPINDir(KEYPAD_PORT,KEYPAD_SIXTH_PIN,0);
	DIO_vsetPINDir(KEYPAD_PORT,KEYPAD_SEVENTH_PIN,0);
	DIO_vsetPINDir(KEYPAD_PORT,KEYPAD_EIGHTH_PIN,0);
	DIO_vconnectpullup(KEYPAD_PORT,KEYPAD_FIFTH_PIN,1);
	DIO_vconnectpullup(KEYPAD_PORT,KEYPAD_SIXTH_PIN,1);
	DIO_vconnectpullup(KEYPAD_PORT,KEYPAD_SEVENTH_PIN,1);
	DIO_vconnectpullup(KEYPAD_PORT,KEYPAD_EIGHTH_PIN,1);
}
uint8 keypad_u8check_press(void)
{
	uint8 arr[4][4]={{'7','8','9','/'},{'4','5','6','*'},{'1','2','3','-'},{'A','0','=','+'}}; //keypad switchs
	uint8 row; 
	uint8 coloumn; 
	uint8 key_pressed_indicator; 
	
	uint8 returnval=NOT_PRESSED; 
	for(row=0;row<4;row++)
	{
		DIO_write(KEYPAD_PORT,KEYPAD_FIRST_PIN,1);
		DIO_write(KEYPAD_PORT,KEYPAD_SECOND_PIN,1);
		DIO_write(KEYPAD_PORT,KEYPAD_THIRD_PIN,1);
		DIO_write(KEYPAD_PORT,KEYPAD_FOURTH_PIN,1);
		
		_delay_ms(20);
		DIO_write(KEYPAD_PORT,row,0);
		
		for(coloumn=0;coloumn<4;coloumn++)
		{
			key_pressed_indicator=DIO_u8read(KEYPAD_PORT,(coloumn+4)); 
			if(key_pressed_indicator==0)
			{
				returnval=arr[row][coloumn]; 
				break; 
			}
		}
	}
	return returnval ; 
}