#ifndef _PARSING_H
#define _PARSING_H

#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "switches.h"
#include "enc.h"
#include "rk_uart.h"
#include "rk_mm.h"

// rk<->main
#define PREAMBLE              0xAA
#define SIZE_OFFSET           0x04
#define TX_PAYLOAD_SIZE       0x06
#define GET_STATE             0x71
#define LED                   0x72
#define ADDRESS               0x01

// rk<->rfid
#define RF_PREAMBLE                 0x43
#define CMD_VERSION                 0x01
#define CMD_TAG_UID                 0x02
#define RF_SIZE_OFFSET              3     //преамбула, длина пакета, crc// сравнение того, что приняли, с тем, что должны были принять
#define RF_TX_PAYLOAD_SIZE_VERSION  1
#define RF_TX_PAYLOAD_SIZE_TAG      1
#define RFID_PAYLOAD_VERSION_OFFSET 3     // начало данных с версией смещено на
                                          // RFID_PAYLOAD_VERSION_OFFSET от начала пакета
#define RFID_PAYLOAD_TAG_OFFSET     4     // начало данных с id метки смещено на
                                          // RFID_PAYLOAD_TAG_UID_OFFSET от начала пакета
#define MAX_UID_LEN                 10
#define TAG_VER_SIZE                3

#define CHECK_RFID_PERIOD           500   // период опроса rfid метки

typedef enum
{
	DATA_NO_ERROR = 0,
	ADDRESS_ERROR = 1,
	SIZE_ERROR = 2,
	CRC_ERROR = 3,
	PREAMBLE_ERROR = 4
} error_TypeDef;

typedef enum
{
	RF_DATA_NO_ERROR = 0,
	RF_COMMAND_ERROR = 1,
	RF_SIZE_ERROR = 2,
	RF_CRC_ERROR = 3,
	RF_PREAMBLE_ERROR = 4
} error_RF_TypeDef;

typedef struct
{
	uint8_t addr;
	uint8_t cmd;
	uint8_t value;
} data_main_TypeDef;

typedef struct
{
	uint8_t cmd;
	uint8_t uid_len;
	uint8_t tag_uid;
	uint8_t tag_version[TAG_VER_SIZE];
	uint8_t value_sensors;
} data_rfid_TypeDef;

error_TypeDef Parse_Main_Message(message_TypeDef *message);
error_RF_TypeDef Parse_RFID_Message(message_TypeDef *message);
uint8_t CRC8(const uint8_t *data, int length);
void Send_Request_RF_Version(message_TypeDef *message);
void Send_Request_RF_Tag(message_TypeDef *message);
void RFID_Period(void);
void Send_State(message_TypeDef *message);
void Send_State_RF(message_TypeDef *message);
void Led(uint8_t led, GPIO_PinState status);
uint8_t Get_Tag_UID();

#endif
