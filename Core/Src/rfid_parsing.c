#include <rk_parsing.h>

error_TypeDef Parse_Main_Message(cmd_TypeDef *command, message_TypeDef *message)
{
	error_TypeDef er = DATA_NO_ERROR;
	if(message->rx_size != message->m_rx[2] + SIZE_OFFSET)
	{
		er = SIZE_ERROR;
	}
	else if(message->m_rx[0] != PREABLE)
	{
		er = PREAMBLE_ERROR;
	}
	else if(message->m_rx[1] != ADDRESS)
	{
		er = ADDRESS_ERROR;
	}
	else if(message->m_rx[message->rx_size-1] != CRC8(message->m_rx, message->rx_size - 1))
	{
		er = CRC_ERROR;
	}
	else
	{
		command->cmd = message->m_rx[3];
		command->value = message->m_rx[4];
	}
	return er;
}

uint8_t CRC8(const uint8_t *data, int length)
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
}

void Send_State(message_TypeDef *message)
{
	message->m_tx[0] = PREABLE;
	message->m_tx[1] = ADDRESS;
	message->m_tx[2] = TX_PAYLOAD_SIZE;
	message->m_tx[3] = 0xFF;								// RFID Tag
	message->m_tx[4] = Get_Sensor_Byte();	// Switches status
	message->m_tx[5] = 0xFF;								// Motor hinge position
	message->m_tx[6] = 0xFF;								// Arm hinge position
	message->m_tx[7] = Get_Button_Counter();
	message->m_tx[8] = Get_Encoder_Value();
	message->m_tx[9] = CRC8(message->m_tx, TX_PAYLOAD_SIZE+SIZE_OFFSET-1);
	message->tx_size = TX_PAYLOAD_SIZE+SIZE_OFFSET;
	Send_Message(message);
}
