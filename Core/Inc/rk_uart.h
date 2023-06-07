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
}m_ready_rx_TypeDef;

//typedef enum
//{
//	MAIN = 0,
//	RFID = 1
//}uarts_TypeDef;

typedef struct
{
	uint8_t rx_size;
	uint8_t tx_size;
	uint8_t m_tx[MESSAGE_MAX_SIZE];
	uint8_t m_rx[MESSAGE_MAX_SIZE];
	UART_HandleTypeDef *uart;
	m_ready_rx_TypeDef ready;
}message_TypeDef;

message_TypeDef* Init_UART(UART_HandleTypeDef *huart);
m_ready_rx_TypeDef Is_Message_Ready(message_TypeDef *message);
void Receive_Message(message_TypeDef *message);
void Send_Message(message_TypeDef *message);
#endif
