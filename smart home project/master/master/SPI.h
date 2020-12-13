
/*
 * SPI.H
 *
 * Created: 13/12/2020 12:15:09 ص
 *  Author: ascom
 */ 
#ifndef SPI_H_
#define SPI_H_

#include "STD_Types.h"
#include "STD_MESSAGES.h"
#include "std_macros.h"

#define SPI_SCK  7
#define SPI_MISO 6
#define SPI_MOSI 5
#define SPI_SS   4

void SPI_vInitMaster (void);

void SPI_vInitSlave (void);

uint8 SPI_ui8TransmitRecive (uint8 data);

#endif //SPI.H