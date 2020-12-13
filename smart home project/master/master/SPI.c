
/*
 * SPI.C
 *
 * Created: 13/12/2020 12:14:35 ص
 *  Author: ascom
 */ 
#include <avr/io.h>
#include "SPI.h"
#include "STD_MESSAGES.h"
#include "std_macros.h"

void SPI_vInitMaster (void)
{
	DDRB |=(1<<SPI_MOSI) |(1<<SPI_SS)|(1<<SPI_SCK);
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void SPI_vInitSlave (void)
{
	DDRB |= (1<<SPI_MISO);
	SPCR |= (1<<SPE);
}

uint8 SPI_ui8TransmitRecive (uint8 data)
{
	SPDR = data;
	while (((SPSR&(1<<SPIF))>>SPIF)==0)	;
	
	return SPDR;
}