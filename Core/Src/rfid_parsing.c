#include <rfid_parsing.h>
#include "rk_uart.h"

volatile int checkrfidPeriod = CHECK_RFID_PERIOD;	// переменная формирует период опроса,
															// период SysTick x checkSwitchPeriod = 10мс
uint8_t RFID_Flag;
uint8_t sensors = 0;

error_RF_TypeDef Parse_RFID_Message(cmd_TypeDef *command, message_TypeDef *message)
{
	error_RF_TypeDef er = RF_DATA_NO_ERROR;
	if(message->rx_size != message->m_rx[1] + RF_SIZE_OFFSET)	//сдвиг подумать, какой должен быть
	{
		er = RF_SIZE_ERROR;
	}
	else if(message->m_rx[0] != RF_PREAMBLE)
	{
		er = RF_PREAMBLE_ERROR;
	}
	else if((message->m_rx[2] != CMD_TAG) && (message->m_rx[2] != CMD_VERSION))
	{
		er = RF_COMMAND_ERROR;
	}
	else if(message->m_rx[message->rx_size-1] != CRC8(message->m_rx, message->rx_size - 1))
	{
		er = RF_CRC_ERROR;
	}
	else
	{//Добавить условия для двух типов принимаемых сообщений
		command->value_sensors = message->m_rx[5];
		sensors |= (command->value_sensors) << 4;
		command->value_tag = message->m_rx[4];
		command->uid_len = message->m_rx[3];
		command->cmd = message->m_rx[2];
		/* Добавить парсинг версии прошивки
	message->rfid_tx[3] = 0x01;								// RFID FW Version 1st byte
	message->rfid_tx[4] = 0x09;								// RFID FW Version 2d byte
	message->rfid_tx[5] = 0x00;								// RFID FW Version 3d byte		 */

	}
	return er;
}

/*uint8_t CRC8(const uint8_t *data, int length)
{
	uint8_t crc = 0x00;
	uint8_t extract;
	uint8_t sum;
	for(int i = 0; i < length; i++)
	{
		extract = *data;
		for(uint8_t tempI = 8; tempI; tempI--)
		{
			sum = (crc ^ extract) & 0x01;
			crc >>= 1;
			if(sum)
				crc ^= 0x8C;
			extract >>= 1;
		}
		data++;
	}
	return crc;
}*/
void Send_Request_RF_Version(message_TypeDef *message)
{
	message->m_tx[0] = RF_PREAMBLE;
	message->m_tx[1] = RF_TX_PAYLOAD_SIZE_VERSION;
	message->m_tx[2] = 0x01;
	message->m_tx[3] = CRC8(message->m_tx, RF_TX_PAYLOAD_SIZE_VERSION+RF_SIZE_OFFSET-1);
	message->tx_size = 4;
	Send_Message(message);
}

void Send_Request_RF_Tag(message_TypeDef *message)
{
	message->m_tx[0] = RF_PREAMBLE;
	message->m_tx[1] = RF_TX_PAYLOAD_SIZE_TAG;
	message->m_tx[2] = 0x02;								// RFID Tag
	message->m_tx[3] = CRC8(message->m_tx, RF_TX_PAYLOAD_SIZE_TAG+RF_SIZE_OFFSET-1); //преамб+длина+команда
	message->tx_size = 4;
	Send_Message(message);
}

void RFID_Period(void)
{
	if(checkrfidPeriod != 0)
	{
		checkrfidPeriod--;
		if(checkrfidPeriod == 0)
		{
			RFID_Flag = SET;
			checkrfidPeriod = CHECK_RFID_PERIOD;
		}
	}
}
