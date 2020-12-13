/*
 * master.c
 *
 * Created: 12/12/2020 11:26:27 م
 * Author : ascom
 */ 
#include "main_config.h"
#include "LCD.h"
#include "EEPROM.h"
#include "timer_driver.h"
#include "keypad_driver.h"
#include "SPI.h"
#include "LED.h"
#include "menu.h"
#include <avr/io.h>

volatile uint16 session_counter = 0;
uint8 timeout_flag = FALSE;

int main(void)
{
	uint8 temperature = 0;//average temperature of the room
	uint8 temp_ones = NOT_SELECTED;//enter right number of the temperature
	uint8 temp_tens = NOT_SELECTED;//enter left number of the temperature
	
	uint8 login_mode = NO_MODE; 
	uint8 block_mode_flag = FALSE;
	uint8 key_pressed = NOT_PRESSED;
	LED_vInit(ADMIN_LED_PORT,ADMIN_LED_PIN);//the led of admin
	LED_vInit(GUEST_LED_PORT,GUEST_LED_PIN);//the led of guest
	LED_vInit(BLOCK_LED_PORT,BLOCK_LED_PIN);//the led of block
	LCD_vInit();
	keypad_vInit();
	SPI_vInitMaster();
	LCD_vSend_string("Welcome to smart");
	LCD_movecursor(2,1);
	LCD_vSend_string("home system");
	_delay_ms(1000);
	LCD_clearscreen();
	if ( (EEPROM_ui8ReadByteFromAddress(ADMIN_PASS_STATUS_ADDRESS)!=PASS_SET) || (EEPROM_ui8ReadByteFromAddress(GUEST_PASS_STATUS_ADDRESS)!=PASS_SET) )
	{
		LCD_vSend_string("Login for");
		LCD_movecursor(2,1);
		LCD_vSend_string("first time");
		_delay_ms(1000);
		LCD_clearscreen();
		LCD_vSend_string("Set Admin pass");
		LCD_movecursor(2,1);
		LCD_vSend_string("Admin pass:");

		uint8 pass_counter=0;
		uint8 pass[PASS_SIZE]={NOT_STORED,NOT_STORED,NOT_STORED,NOT_STORED};
		while (pass_counter<PASS_SIZE)
		{
			key_pressed = NOT_PRESSED;
			while (key_pressed == NOT_PRESSED)
			{
				key_pressed = keypad_u8check_press();
			}

			pass[pass_counter]=key_pressed;
			LCD_vSend_char(key_pressed);
			_delay_ms(CHARACTER_PREVIEW_TIME);
			LCD_movecursor(2,12+pass_counter);
			LCD_vSend_char(PASSWORD_SYMBOL);
			_delay_ms(100);
			pass_counter++;
		}
		EEPROM_vWriteBlockToAddress(EEPROM_ADMIN_ADDRESS,pass,PASS_SIZE);
		EEPROM_vWriteByteToAddress(ADMIN_PASS_STATUS_ADDRESS,PASS_SET);
		LCD_clearscreen();
		LCD_vSend_string("Pass Saved");
		_delay_ms(500);
		LCD_clearscreen();

		pass_counter=0;
		LCD_vSend_string("Set Guest Pass");
		LCD_movecursor(2,1);
		LCD_vSend_string("Guest Pass:");
		while (pass_counter<PASS_SIZE)
		{
			key_pressed = NOT_PRESSED;
			while (key_pressed == NOT_PRESSED)
			{
				key_pressed = keypad_u8check_press();
			}

			pass[pass_counter]=key_pressed;
			LCD_vSend_char(key_pressed);
			_delay_ms(CHARACTER_PREVIEW_TIME);
			LCD_movecursor(2,12+pass_counter);
			LCD_vSend_char(PASSWORD_SYMBOL); 
			_delay_ms(100);
			pass_counter++;
		}
		EEPROM_vWriteBlockToAddress(EEPROM_GUEST_ADDRESS,pass,PASS_SIZE);
		EEPROM_vWriteByteToAddress(GUEST_PASS_STATUS_ADDRESS,PASS_SET);
		LCD_clearscreen();
		LCD_vSend_string("Pass Saved");
		_delay_ms(500);
		LCD_clearscreen();
		EEPROM_vWriteByteToAddress(LOGIN_BLOCKED_ADDRESS,FALSE);
	}
	else
	{
		block_mode_flag = EEPROM_ui8ReadByteFromAddress(LOGIN_BLOCKED_ADDRESS); 
	}
	while (1)
	{
		key_pressed = NOT_PRESSED;
		uint8 pass_tries_count=0;
		
		if ( timeout_flag==TRUE )
		{
			timer0_stop();
			session_counter = 0;
			timeout_flag=FALSE;
			login_mode=NO_MODE;
			key_pressed = NOT_PRESSED;
			LED_vTurnOff(GUEST_LED_PORT,GUEST_LED_PIN);
			LED_vTurnOff(ADMIN_LED_PORT,ADMIN_LED_PIN);
			LCD_clearscreen();
			LCD_vSend_string("Session Timeout");
			_delay_ms(1000);
		}
		while (login_mode==NO_MODE)
		{
			if(block_mode_flag==TRUE)
			{
				LCD_clearscreen();
				LCD_vSend_string("Login blocked");
				LCD_movecursor(2,1);
				LCD_vSend_string("wait 20 seconds");
				LED_vTurnOn(BLOCK_LED_PORT,BLOCK_LED_PIN);
				_delay_ms(BLOCK_MODE_TIME);
				pass_tries_count = 0; 
				block_mode_flag = FALSE;
				LED_vTurnOff(BLOCK_LED_PORT,BLOCK_LED_PIN);
				EEPROM_vWriteByteToAddress(LOGIN_BLOCKED_ADDRESS,FALSE);
			}
			LCD_clearscreen();
			LCD_vSend_string("Select mode :");
			LCD_movecursor(2,1);
			LCD_vSend_string("0:Admin 1:Guest");
			while(key_pressed==NOT_PRESSED)
			{
				key_pressed = keypad_u8check_press();
			}
			if ( key_pressed!=CHECK_ADMIN_MODE && key_pressed!=CHECK_GUEST_MODE )
			{
				LCD_clearscreen();
				LCD_vSend_string("Wrong input.");
				key_pressed = NOT_PRESSED;
				_delay_ms(1000);
				continue;
			}

			uint8 pass_counter=0;
			uint8 pass[PASS_SIZE]={NOT_STORED,NOT_STORED,NOT_STORED,NOT_STORED};
			uint8 stored_pass[PASS_SIZE]={NOT_STORED,NOT_STORED,NOT_STORED,NOT_STORED};
			
			switch(key_pressed)
			{
				case CHECK_ADMIN_MODE:
				while(login_mode!=ADMIN)
				{
					key_pressed = NOT_PRESSED;
					LCD_clearscreen();
					LCD_vSend_string("Admin mode");
					LCD_movecursor(2,1);
					LCD_vSend_string("Enter Pass:");
					_delay_ms(200);
					pass_counter=0;
					while(pass_counter<PASS_SIZE)
					{
						while (key_pressed == NOT_PRESSED)
						{
							key_pressed = keypad_u8check_press();
						}
						pass[pass_counter]=key_pressed;
						LCD_vSend_char(key_pressed);
						_delay_ms(CHARACTER_PREVIEW_TIME);
						LCD_movecursor(2,12+pass_counter);
						LCD_vSend_char(PASSWORD_SYMBOL);
						_delay_ms(100);
						pass_counter++;
						key_pressed = NOT_PRESSED;
					}
					EEPROM_vReadBlockFromAddress(EEPROM_ADMIN_ADDRESS,stored_pass,PASS_SIZE);
					if ((ui8ComparePass(pass,stored_pass,PASS_SIZE)) == TRUE)
					{
						login_mode = ADMIN;
						pass_tries_count=0;
						LCD_clearscreen();
						LCD_vSend_string("Right pass");
						LCD_movecursor(2,1);
						LCD_vSend_string("Admin mode");
						_delay_ms(500);
						LED_vTurnOn(ADMIN_LED_PORT,ADMIN_LED_PIN);
						timer0_initializeCTC();
						LCD_clearscreen();
					}
					else
					{
						pass_tries_count++;
						login_mode = NO_MODE;
						LCD_clearscreen();
						LCD_vSend_string("Wrong Pass");
						LCD_movecursor(2,1);
						LCD_vSend_string("Tries left:");
						LCD_vSend_char(TRIES_ALLOWED-pass_tries_count+ASCII_ZERO);
						_delay_ms(1000);
						LCD_clearscreen();
						if (pass_tries_count>=TRIES_ALLOWED)
						{
							EEPROM_vWriteByteToAddress(LOGIN_BLOCKED_ADDRESS,TRUE);
							block_mode_flag = TRUE;
							break;
						}
					}
				}
				break;
				case  CHECK_GUEST_MODE:
				while(login_mode != GUEST)
				{
					key_pressed = NOT_PRESSED;
					LCD_clearscreen();
					LCD_vSend_string("Guest mode");
					LCD_movecursor(2,1);
					LCD_vSend_string("Enter pass:");
					_delay_ms(200);
					pass_counter=0;
					while(pass_counter<PASS_SIZE)
					{
						while (key_pressed == NOT_PRESSED)
						{
							key_pressed = keypad_u8check_press();
						}
						pass[pass_counter]=key_pressed;
						LCD_vSend_char(key_pressed);
						_delay_ms(CHARACTER_PREVIEW_TIME);
						LCD_movecursor(2,12+pass_counter);
						LCD_vSend_char(PASSWORD_SYMBOL); 
						_delay_ms(100);
						pass_counter++;
						key_pressed = NOT_PRESSED;
					}
					EEPROM_vReadBlockFromAddress(EEPROM_GUEST_ADDRESS,stored_pass,PASS_SIZE); 

					if (ui8ComparePass(pass,stored_pass,PASS_SIZE)==TRUE)
					{
						login_mode = GUEST;
						pass_tries_count=0;
						LCD_clearscreen();
						LCD_vSend_string("Right pass");
						LCD_movecursor(2,1);
						LCD_vSend_string("Guest mode");
						_delay_ms(500);
						LED_vTurnOn(GUEST_LED_PORT,GUEST_LED_PIN);
						timer0_initializeCTC();
						LCD_clearscreen();
					}
					else
					{
						pass_tries_count++;
						login_mode = NO_MODE;
						LCD_clearscreen();
						LCD_vSend_string("Wrong pass");
						LCD_movecursor(2,1);
						LCD_vSend_string("Tries left:");
						LCD_vSend_char(TRIES_ALLOWED-pass_tries_count+ASCII_ZERO);
						_delay_ms(1000);
						LCD_clearscreen();
						if (pass_tries_count>=TRIES_ALLOWED)
						{
							EEPROM_vWriteByteToAddress(LOGIN_BLOCKED_ADDRESS,TRUE);
							block_mode_flag = TRUE;
							break;
						}
					}
				}
				break;
			}//end of the switch
			
		}
		
		uint8 show_menu = MAIN_MENU;
		
		
		while(timeout_flag!=TRUE)
		{
			key_pressed = NOT_PRESSED;
			switch (show_menu)
			{
				case MAIN_MENU:
				do
				{
					LCD_clearscreen();
					LCD_vSend_string("1:Room1 2:Room2");
					LCD_movecursor(2,1);
					if(login_mode==ADMIN)
					{
						LCD_vSend_string("3:Room3 4:More ");
					}
					else if(login_mode==GUEST)
					{
						LCD_vSend_string("3:Room3 4:Room4");
					}
					
					
					key_pressed = u8GetKeyPressed(login_mode);
					_delay_ms(100);
					
					if (key_pressed == SELECT_ROOM1)
					{
						show_menu = ROOM1_MENU;
					}
					else if (key_pressed == SELECT_ROOM2)
					{
						show_menu = ROOM2_MENU;
					}
					else if (key_pressed == SELECT_ROOM3)
					{
						show_menu = ROOM3_MENU;
					}
					else if (key_pressed == SELECT_ROOM4 && login_mode == GUEST)
					{
						show_menu = ROOM4_MENU;
					}
					else if (key_pressed == ADMIN_MORE_OPTION && login_mode == ADMIN)
					{
						show_menu = MORE_MENU;
					}
					else if(key_pressed != NOT_PRESSED)
					{
						LCD_clearscreen();
						LCD_vSend_string("Wrong input");
						_delay_ms(500);
					}
				} while ( ((key_pressed < '1') || (key_pressed > '4') ) && (timeout_flag == FALSE) );
				
				break;
				
				case MORE_MENU:
				do
				{
					LCD_clearscreen();
					LCD_vSend_string("1:Room4    2:TV   ");
					LCD_movecursor(2,1);
					LCD_vSend_string("3:Air Cond.4:RET");
					key_pressed = u8GetKeyPressed(login_mode);
					_delay_ms(100);
					
					if (key_pressed == SELECT_ROOM4_ADMIN)
					{
						show_menu = ROOM4_MENU;
					}
					else if (key_pressed == SELECT_TV)
					{
						show_menu = TV_MENU;
					}
					else if (key_pressed == SELECT_AIR_CONDITIONING)
					{
						show_menu = AIRCONDITIONING_MENU;
					}
					else if (key_pressed == ADMIN_RET_OPTION)
					{
						show_menu = MAIN_MENU;
					}
					else if(key_pressed != NOT_PRESSED)
					{
						LCD_clearscreen();
						LCD_vSend_string("Wrong input");
						_delay_ms(500);
					}
				} while (( (key_pressed < '1') || (key_pressed > '4') ) && (timeout_flag == FALSE));
				
				break;
				
				case AIRCONDITIONING_MENU:
				do
				{
					LCD_clearscreen();
					LCD_vSend_string("1:Set temperature ");
					LCD_movecursor(2,1);
					LCD_vSend_string("2:Control  0:RET");
					key_pressed = u8GetKeyPressed(login_mode);
					_delay_ms(100);
					
					if (key_pressed == SELECT_SET_TEMPERATURE)
					{
						show_menu = TEMPERATURE_MENU;
					}
					else if (key_pressed == SELECT_AIR_COND_CTRL)
					{
						show_menu = AIRCOND_CTRL_MENU;
					}
					else if (key_pressed == SELECT_AIR_COND_RET)
					{
						show_menu = MORE_MENU;
					}
					else if(key_pressed != NOT_PRESSED)
					{
						LCD_clearscreen();
						LCD_vSend_string("Wrong input");
						_delay_ms(500);
					}
				} while (( (key_pressed < '0') || (key_pressed > '2') ) && (timeout_flag == FALSE));
				break;
				
				case ROOM1_MENU:
				vMenuOption(ROOM1_MENU,login_mode);
				show_menu = MAIN_MENU;
				break;
				
				case ROOM2_MENU:
				vMenuOption(ROOM2_MENU,login_mode);
				show_menu = MAIN_MENU;
				break;
				
				case ROOM3_MENU:
				vMenuOption(ROOM3_MENU,login_mode);
				show_menu = MAIN_MENU;
				break;
				
				case ROOM4_MENU:
				vMenuOption(ROOM4_MENU,login_mode);
				if (login_mode == GUEST)
				{
					show_menu = MAIN_MENU;
				}
				else
				{
					show_menu = MORE_MENU;
				}
				break;
				
				case TV_MENU:
				vMenuOption(TV_MENU,login_mode);
				show_menu = MORE_MENU;
				break;
				
				case AIRCOND_CTRL_MENU:
				vMenuOption(AIRCOND_CTRL_MENU,login_mode);
				show_menu = AIRCONDITIONING_MENU;
				break;
				
				case TEMPERATURE_MENU:
				temperature = 0;
				while (temperature==0 && timeout_flag == FALSE)
				{
					key_pressed = NOT_PRESSED;
					LCD_clearscreen();
					LCD_vSend_string("Set temp.:__ ");
					LCD_vSend_char(DEGREES_SYMBOL); 
					LCD_vSend_char('C'); 
					LCD_movecursor(1,11);
					_delay_ms(200);
					key_pressed = u8GetKeyPressed(login_mode);
					_delay_ms(250);

					if (timeout_flag == TRUE) 
					{
						break;
					}
					if (key_pressed <'0' || key_pressed >'9')
					{
						LCD_clearscreen();
						LCD_vSend_string("Wrong input");
						_delay_ms(500);
						continue;
					}
					else
					{
						LCD_vSend_char(key_pressed);
						temp_tens = key_pressed-ASCII_ZERO;
						key_pressed = NOT_PRESSED;
					}
					key_pressed = u8GetKeyPressed(login_mode);
					_delay_ms(250);
					
					if (timeout_flag == TRUE)
					{
						break;
					}
					if ((key_pressed <'0' || key_pressed >'9'))
					{
						LCD_clearscreen();
						LCD_vSend_string("Wrong input");
						_delay_ms(500);
						continue;
					}
					else
					{
						LCD_vSend_char(key_pressed);
						temp_ones = key_pressed-ASCII_ZERO;
						key_pressed = NOT_PRESSED;
					}
					temperature = temp_tens*10 + temp_ones;
					SPI_ui8TransmitRecive(SET_TEMPERATURE);
					_delay_ms(200);
					SPI_ui8TransmitRecive(temperature);
					LCD_clearscreen();
					LCD_vSend_string("Temperature Sent");
					_delay_ms(500);
				}
				show_menu = AIRCONDITIONING_MENU;
				break;
			}
		}
	}
}

ISR(TIMER0_COMP_vect)
{
	session_counter++;
}

