
/*
 * ADC_driver.c
 *
 * Created: 13/12/2020 05:55:12 ص
 *  Author: ascom
 */ 
#include <avr/io.h>
#include "std_macros.h"
#include "STD_Types.h"
void ADC_vinit(void)
{
	SET_BIT(ADMUX,REFS0);
	SET_BIT(ADMUX,REFS1);
	SET_BIT(ADCSRA,ADEN); 
	SET_BIT(ADCSRA,ADPS2);
	SET_BIT(ADCSRA,ADPS1);
	
}
uint16 ADC_u16Read(void)
{
	uint16 read_val; 
	SET_BIT(ADCSRA,ADSC); 
	while(IS_BIT_CLR(ADCSRA,ADIF));
	SET_BIT(ADCSRA,ADIF); 
	read_val=(ADCL); 
	read_val|=(ADCH<<8);
	return read_val ; 
}