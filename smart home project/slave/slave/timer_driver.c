
/*
 * timer_driver.c
 *
 * Created: 13/12/2020 06:13:38 ص
 *  Author: ascom
 */ 
#include "timer_driver.h"

void timer0_initializeCTC(void)
{
	OCR0 = 78; 
	SET_BIT(TCCR0, WGM01);
	CLR_BIT(TCCR0, WGM00);
	CLR_BIT(TCCR0, CS01);
	SET_BIT(TCCR0, CS02);	
	sei();
	SET_BIT(TIMSK, OCIE0);
}

void timer0_stop(void)
{
	CLR_BIT(TCCR0, CS00);
	CLR_BIT(TCCR0, CS01);
	CLR_BIT(TCCR0, CS02);
}
void timer_initializefastpwm(void)
{
	OCR0 = 128;
	SET_BIT(TCCR0, WGM00);
	SET_BIT(TCCR0, WGM01);
	sei();
	SET_BIT(TCCR0, COM01); 
	SET_BIT(TIMSK,TOIE0);	
	SET_BIT(TCCR0, CS00);
	CLR_BIT(TCCR0, CS01);
	SET_BIT(TCCR0, CS02);
}
void change_dutycycle(float64 duty)
{
	OCR0 = (duty / 100) * 256;
}