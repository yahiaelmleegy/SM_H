
/*
 * LED.h
 *
 * Created: 13/12/2020 06:04:16 ص
 *  Author: ascom
 */ 
#ifndef LED_H_
#define LED_H_
#include "DIO.h"
#include "STD_Types.h"

void LED_vInit(uint8 portname,uint8 pinnumber);

void LED_vTurnOn(uint8 portname,uint8 pinnumber);//

void LED_vTurnOff(uint8 portname,uint8 pinnumber);

void LED_vToggle(uint8 portname,uint8 pinnumber);//

uint8 LED_u8ReadStatus(uint8 portname,uint8 pinnumber);

#endif //LED.h