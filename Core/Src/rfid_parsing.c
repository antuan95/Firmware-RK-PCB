#include <rfid_parsing.h>

error_RF_TypeDef Parse_RFID_Message(cmd_TypeDef *command, message_TypeDef *message)
{
	error_RF_TypeDef er = DATA_NO_ERROR;
	if(message->rx_size != message->rfid_rx[1] + SIZE_OFFSET)
	{
		er = SIZE_ERROR;
	}
	else if(message->rfid_rx[0] != PREAMBLE)
	{
		er = PREAMBLE_ERROR;
	}
	else if(message->rfid_rx[2] != CMD_TAG || message->rfid_rx[2] != CMD_VERSION)
	{
		er = COMMAND_ERROR;
	}
	else if(message->rfid_rx[message->rx_size-1] != CRC8(message->rfid_rx, message->rx_size - 1))
	{
		er = CRC_ERROR;
	}
	else
	{//Добавить условия для двух типов принимаемых сообщений
		command->value_sensors = message->rfid_rx[4];
		command->value_tag = message->rfid_rx[3];
		command->cmd = message->rfid_rx[2];
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
	message->rfid_tx[0] = PREAMBLE;
	message->rfid_tx[1] = TX_PAYLOAD_SIZE_VERSION;
	message->rfid_tx[2] = 0x01;
	message->rfid_tx[1] = CRC8(message->rfid_tx, TX_PAYLOAD_SIZE_VERSION+SIZE_OFFSET-1);
	Send_Message(message);
}

void Send_Request_RF_Tag(message_TypeDef *message)
{
	message->rfid_tx[0] = PREAMBLE;
	message->rfid_tx[1] = TX_PAYLOAD_SIZE_TAG;
	message->rfid_tx[2] = 0x02;								// RFID Tag
	message->rfid_tx[3] = CRC8(message->rfid_tx, TX_PAYLOAD_SIZE_TAG+SIZE_OFFSET-1);
	Send_Message(message);
}
