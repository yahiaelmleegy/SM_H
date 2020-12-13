
/*
 * EEPROM.c
 *
 * Created: 12/12/2020 11:56:12 م
 *  Author: ascom
 */ 
#include "EEPROM.h"
#include <avr/io.h>

uint8 EEPROM_ui8ReadByteFromAddress(const uint16 u16Address)
{
	while (((EECR & (1 << EEWE))>>EEWE)==1)
	;
	EEAR = u16Address;
	EECR |= (1 << EERE);
	return EEDR;
}

void EEPROM_vReadBlockFromAddress(const uint16 uiAddress, uint8* dist,const uint16 size)
{
	while (((EECR & (1 << EEWE))>>EEWE)==1)
	;

	uint16 counter = 0;

	while (counter < size) 
	{
		EEAR = uiAddress + counter;
		EECR |= (1 << EERE);
		*(dist + counter) = EEDR;

		counter++;
	}
}

void EEPROM_vWriteByteToAddress(const uint16 uiAddress, const uint8 uiData)
{

	while ( ( (EECR & (1 << EEWE) ) >>EEWE ) == 1)
	;
	EEAR = uiAddress;


	EECR |= (1 << EERE);
	if (EEDR != uiData)
	{
		EEDR = uiData;

		
		EECR |= (1 << EEMWE);
		
		EECR |= (1 << EEWE);
	}
	else
	{

	}

}

void EEPROM_vWriteBlockToAddress(const uint16 uiAddress, const uint8* uiData,const uint16 size)
{
	uint16 counter = 0;
	while (counter < size)
	{
		
		while (( (EECR & (1 << EEWE) ) >>EEWE ) == 1)
		;
		
		EEAR = uiAddress + counter;

		
		EECR |= (1 << EERE);
		if (EEDR != (*(uiData + counter)))
		{
			EEDR = *(uiData + counter);

	
			EECR |= (1 << EEMWE);
			
			EECR |= (1 << EEWE);
		}
		else
		{

		}
		counter++;
	}
}