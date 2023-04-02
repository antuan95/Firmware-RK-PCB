#ifndef _PARSING_H
#define _PARSING_H

#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "switches.h"
#include "enc.h"
#include "rk_uart.h"

#define PREABLE								0xAA
#define SIZE_OFFSET							4
#define TX_PAYLOAD_SIZE						6

#define ADDRESS								1

typedef enum
{
	DATA_NO_ERROR = 0,
	ADDRESS_ERROR = 1,
	SIZE_ERROR = 2,
	CRC_ERROR = 3,
	PREAMBLE_ERROR = 4
}error_TypeDef;

error_TypeDef Parse_Main_Message(cmd_TypeDef *command, message_TypeDef *message);
uint8_t CRC8(const uint8_t *data,int length);
void Send_State(message_TypeDef *message);
//void Send_State_RF(message_TypeDef *message);

#endif
