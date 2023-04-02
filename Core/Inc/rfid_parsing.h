#ifndef _PARSING_H
#define _PARSING_H

#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "switches.h"
#include "enc.h"
#include "rk_uart.h"

#define PREAMBLE							0x43
#define CMD_VERSION							0x01
#define CMD_TAG								0x02
#define SIZE_OFFSET							4 		//?
#define TX_PAYLOAD_SIZE_VERSION				4
#define TX_PAYLOAD_SIZE_TAG					4


typedef enum
{
	DATA_NO_ERROR = 0,
    COMMAND_ERROR = 1,
	SIZE_ERROR = 2,
	CRC_ERROR = 3,
	PREAMBLE_ERROR = 4
}error_RF_TypeDef;

error_RF_TypeDef Parse_RFID_Message(cmd_TypeDef *command, message_TypeDef *message);
uint8_t CRC8(const uint8_t *data,int length);
//void Send_State(message_TypeDef *message);
void Send_Request_RF_Version(message_TypeDef *message);
void Send_Request_RF_Tag(message_TypeDef *message);

#endif
