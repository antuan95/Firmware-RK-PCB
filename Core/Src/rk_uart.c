#include "main.h"
#include "rk_uart.h"

message_TypeDef messages[UARTS_NUMBER];

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size)
{
//	OUT_PIN_GPIO_Port->BSRR |= (uint32_t) OUT_PIN_Pin << 16U;
	for(uint8_t i = 0; i < UARTS_NUMBER; i++)
	{
		if(huart == messages[i].uart)
		{
			messages[i].rx_size = size;
			messages[i].ready = DATA_READY;
		}
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == messages[MAIN].uart)
	{
//		HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	for(uint8_t i = 0; i < UARTS_NUMBER; i++)
	{
		if(huart == messages[i].uart)
		{
			messages[i].ready = DATA_NOT_READY;
			HAL_UARTEx_ReceiveToIdle_IT(huart, messages[i].m_rx, MESSAGE_MAX_SIZE);
		}
	}
}

message_TypeDef* Init_UART(UART_HandleTypeDef *huart, uarts_TypeDef uart_number)
{
	messages[uart_number].uart = huart;
	return &messages[uart_number];
}

void Receive_Message(message_TypeDef *message)
{
	if(message->uart != NULL)
	{
			message->rx_size = 0;
			message->ready = DATA_NOT_READY;
			HAL_UARTEx_ReceiveToIdle_IT(message->uart,
										message->m_rx,
										MESSAGE_MAX_SIZE);
	}

}

void Send_Message(message_TypeDef *message)
{
		HAL_UART_Transmit_IT(message->uart,
						 	 message->m_tx,
							 message->tx_size);

}
