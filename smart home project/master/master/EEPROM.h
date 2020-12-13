
/* EEPROM.h
 *
 * Created: 12/12/2020 11:54:09 م
 *  Author: ascom
 */ 

#ifndef HEADERS_EEPROM_H_
#define HEADERS_EEPROM_H_

#include "STD_Types.h"


uint8 EEPROM_ui8ReadByteFromAddress(const uint16 u16Address);


void EEPROM_vReadBlockFromAddress(const uint16 u16Address, uint8* dist,const uint16 size);


void EEPROM_vWriteByteToAddress(const uint16 u16Address, const uint8 u8Data);

void EEPROM_vWriteBlockToAddress(const uint16 u16Address, const uint8* u8Data,const uint16 size);

#endif //EEPROM.h