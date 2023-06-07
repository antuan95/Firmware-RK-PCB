#include "main.h"
#include "rk_uart.h"

message_TypeDef messages[UARTS_NUMBER] = {0};
uint8_t uart_number = 0;

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size)
{
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

message_TypeDef* Init_UART(UART_HandleTypeDef *huart)
{
	messages[uart_number].uart = huart;
	return &messages[uart_number++];
}

m_ready_rx_TypeDef Is_Message_Ready(message_TypeDef *message)
{
	return message->ready;
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
	else
	{

	}

}

void Send_Message(message_TypeDef *message)
{
	if(message->uart != NULL)
	{
		HAL_UART_Transmit_IT(message->uart,
						 	 message->m_tx,
							 message->tx_size);
	}
	else
	{

	}

}
