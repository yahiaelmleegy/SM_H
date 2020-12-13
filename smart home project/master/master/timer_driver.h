
/*
 * timer_driver.h
 *
 * Created: 13/12/2020 12:20:58 ص
 *  Author: ascom
 */ 
#ifndef TIMER_DRIVER_H_
#define TIMER_DRIVER_H
#include <avr/io.h>
#include <avr/interrupt.h>
#include "std_macros.h"
#include "STD_Types.h"

void timer0_initializeCTC(void);

void timer0_stop(void);

void timer_initializefastpwm(void);

void change_dutycycle(float64 duty);

#endif //timer_driver.h