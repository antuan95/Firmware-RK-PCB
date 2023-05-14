#ifndef _RK_UART_H
#define _RK_UART_H

#include <stdint.h>
#include <stdbool.h>
#ifdef STM32G031xx
#include "stm32g031xx.h"
#endif
#ifdef STM32G407xx
#include "stm32f4xx_hal.h"
#endif

#define UARTS_NUMBER	2						// количество доступных уартов
#define MESSAGE_MAX_SIZE	16			// максимальная длина пакета по уарту


typedef enum
{
	DATA_NOT_READY = 0,
	DATA_READY = 1
}m_data_rx_TypeDef;

typedef enum
{
	MAIN = 0,
	RFID = 1
}uarts_TypeDef;

typedef struct
{
	uint8_t rx_size;
	uint8_t tx_size;
	uint8_t m_tx[MESSAGE_MAX_SIZE];
	uint8_t m_rx[MESSAGE_MAX_SIZE];
	uint8_t rfid_tx[MESSAGE_MAX_SIZE];
	uint8_t rfid_rx[MESSAGE_MAX_SIZE];
	UART_HandleTypeDef *uart;
	m_data_rx_TypeDef ready;
}message_TypeDef;

typedef struct
{
	uint8_t addr;
	uint8_t cmd;
	uint8_t value;
	uint8_t value_version; //версия fw rfid
	uint8_t uid_len;
	uint8_t value_tag;
	uint8_t value_sensors;
}cmd_TypeDef;

message_TypeDef* Init_UART(UART_HandleTypeDef *huart, uarts_TypeDef uart_number);
void Receive_Message(message_TypeDef *message);
void Send_Message(message_TypeDef *message);
#endif
