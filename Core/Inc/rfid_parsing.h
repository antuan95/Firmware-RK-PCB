#ifndef _RF_PARSING_H
#define _RF_PARSING_H

#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "switches.h"
#include "enc.h"
#include "rk_uart.h"

#define RF_PREAMBLE							0x43
#define CMD_VERSION							0x01
#define CMD_TAG								0x02
#define RF_SIZE_OFFSET							3 		//преамбула, длина пакета, crc// сравнение того, что приняли, с тем, что должны были принять
#define RF_TX_PAYLOAD_SIZE_VERSION				4
#define RF_TX_PAYLOAD_SIZE_TAG					1 //должен быть 1. Длина

#define CHECK_RFID_PERIOD					  500 // период опроса концевиков SysTick x CHECK_RFID_PERIOD = 500мс


typedef enum
{
	RF_DATA_NO_ERROR = 0,
    RF_COMMAND_ERROR = 1,
	RF_SIZE_ERROR = 2,
	RF_CRC_ERROR = 3,
	RF_PREAMBLE_ERROR = 4
}error_RF_TypeDef;

error_RF_TypeDef Parse_RFID_Message(cmd_TypeDef *command, message_TypeDef *message);
uint8_t CRC8(const uint8_t *data,int length);
//void Send_State(message_TypeDef *message);
void Send_Request_RF_Version(message_TypeDef *message);
void Send_Request_RF_Tag(message_TypeDef *message);
void RFID_Period(void);

#endif
