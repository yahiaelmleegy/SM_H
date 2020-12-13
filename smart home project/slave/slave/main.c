/*
 * slave.c
 *
 * Created: 13/12/2020 01:02:09 ص
 * Author : ascom
 */ 

#include "SPI.h"
#include "STD_MESSAGES.h"
#include "DIO.h"
#include "LED.h"
#include "ADC_driver.h"
#include "timer_driver.h"
#include "APP_slave_Macros.h"

volatile uint16 required_temperature=24;
volatile uint16 temp_sensor_reading=0; 
volatile uint8 counter=0;
volatile uint8 last_air_conditioning_value=AIR_CONDTIONING_OFF; 

int main(void)
{
	ADC_vinit();
	timer0_initializeCTC();
	SPI_vInitSlave();
	
	LED_vInit(AIR_COND_PORT,AIR_COND_PIN);
	LED_vInit(TV_PORT,TV_PIN);
	LED_vInit(ROOM1_PORT,ROOM1_PIN);
	LED_vInit(ROOM2_PORT,ROOM2_PIN);
	LED_vInit(ROOM3_PORT,ROOM3_PIN);
	LED_vInit(ROOM4_PORT,ROOM4_PIN);
	
	uint8 request = DEFAULT_ACK;
	uint8 response = DEFAULT_ACK;
	
	while(1)
	{
		request = SPI_ui8TransmitRecive(DEFAULT_ACK);
		switch (request)
		{
			case ROOM1_STATUS:
			if (LED_u8ReadStatus(ROOM1_PORT,ROOM1_PIN)==0)
			{
				response = OFF_STATUS;
			}
			else if (LED_u8ReadStatus(ROOM1_PORT,ROOM1_PIN)==1)
			{
				response = ON_STATUS;
			}
			else
			{
			}
			SPI_ui8TransmitRecive(response);
			break;
			case ROOM2_STATUS:
			if (LED_u8ReadStatus(ROOM2_PORT,ROOM2_PIN)==0)
			{
				response = OFF_STATUS;
			}
			else if (LED_u8ReadStatus(ROOM2_PORT,ROOM2_PIN)==1)
			{
				response = ON_STATUS;
			}
			else
			{
			}
			SPI_ui8TransmitRecive(response);
			break;
			case ROOM3_STATUS:
			if (LED_u8ReadStatus(ROOM3_PORT,ROOM3_PIN)==0)
			{
				response = OFF_STATUS;
			}
			else if (LED_u8ReadStatus(ROOM3_PORT,ROOM3_PIN)==1)
			{
				response = ON_STATUS;
			}
			else
			{
			}
			SPI_ui8TransmitRecive(response);
			break;
			case ROOM4_STATUS:
			if (LED_u8ReadStatus(ROOM4_PORT,ROOM4_PIN)==0)
			{
				response = OFF_STATUS;
			}
			else if (LED_u8ReadStatus(ROOM4_PORT,ROOM4_PIN)==1)
			{
				response = ON_STATUS;
			}
			else
			{
			}
			SPI_ui8TransmitRecive(response);
			break;
			case AIR_COND_STATUS:
			if (LED_u8ReadStatus(AIR_COND_PORT,AIR_COND_PIN)==0)
			{
				response = OFF_STATUS;
			}
			else if (LED_u8ReadStatus(AIR_COND_PORT,AIR_COND_PIN)==1)
			{
				response = ON_STATUS;
			}
			else
			{
			}
			SPI_ui8TransmitRecive(response);
			break;
			case TV_STATUS:
			if (LED_u8ReadStatus(TV_PORT,TV_PIN)==0)
			{
				response = OFF_STATUS;
			}
			else if (LED_u8ReadStatus(TV_PORT,TV_PIN)==1)
			{
				response = ON_STATUS;
			}
			else
			{
			}
			SPI_ui8TransmitRecive(response);
			break;
			
			case ROOM1_TURN_ON:
			LED_vTurnOn(ROOM1_PORT,ROOM1_PIN);
			break;//break the switch 
			case ROOM2_TURN_ON:
			LED_vTurnOn(ROOM1_PORT,ROOM2_PIN);
			break;
			case ROOM3_TURN_ON:
			LED_vTurnOn(ROOM3_PORT,ROOM3_PIN);
			break; 
			case ROOM4_TURN_ON:
			LED_vTurnOn(ROOM4_PORT,ROOM4_PIN);//turn on the room(4) led
			break;//break the switch 
			case AIR_COND_TURN_ON:
			timer0_initializeCTC();
			LED_vTurnOn(AIR_COND_PORT,AIR_COND_PIN);//turn on the air cond led
			break;//break the switch 
			case TV_TURN_ON:
			LED_vTurnOn(TV_PORT,TV_PIN);
			break;//break the switch 			
			case ROOM1_TURN_OFF:
			LED_vTurnOff(ROOM1_PORT,ROOM1_PIN);
			break;//break the switch 
			case ROOM2_TURN_OFF:
			LED_vTurnOff(ROOM2_PORT,ROOM2_PIN);
			break;//break the switch 
			case ROOM3_TURN_OFF:
			LED_vTurnOff(ROOM3_PORT,ROOM3_PIN);
			break;//break the switch 
			case ROOM4_TURN_OFF:
			LED_vTurnOff(ROOM4_PORT,ROOM4_PIN);
			break;
			case AIR_COND_TURN_OFF:
			timer0_stop();
			LED_vTurnOff(AIR_COND_PORT,AIR_COND_PIN);
			break;
			case TV_TURN_OFF:
			LED_vTurnOff(TV_PORT,TV_PIN);
			break;
			
			case SET_TEMPERATURE:
			required_temperature = SPI_ui8TransmitRecive(DEFAULT_ACK);
			break;
		}
	}
}

ISR(TIMER0_COMP_vect)
{
	counter++;
	if(counter>=10)
	{
		counter=0;
		temp_sensor_reading=(0.25*ADC_u16Read());
		if (temp_sensor_reading>=(required_temperature+1))
		{
			LED_vTurnOn(AIR_COND_PORT,AIR_COND_PIN);
			last_air_conditioning_value=AIR_CONDTIONING_ON;
		}
		else if (temp_sensor_reading<=(required_temperature-1))
		{
			LED_vTurnOff(AIR_COND_PORT,AIR_COND_PIN);
			last_air_conditioning_value=AIR_CONDTIONING_OFF;
		}
		else if (required_temperature==temp_sensor_reading)
		{
			if (last_air_conditioning_value==AIR_CONDTIONING_ON)
			{
				LED_vTurnOn(AIR_COND_PORT,AIR_COND_PIN);
			}
			else if (last_air_conditioning_value==AIR_CONDTIONING_OFF)
			{
				LED_vTurnOff(AIR_COND_PORT,AIR_COND_PIN);
			}
		}
	}
}

