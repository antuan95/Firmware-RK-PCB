#include <rk_parsing.h>

volatile uint32_t checkrfidPeriod = CHECK_RFID_PERIOD;    // переменная формирует период опроса,
																													// период SysTick x checkSwitchPeriod = 10мс
uint8_t RFID_Flag = RESET;
data_main_TypeDef data_main = { 0 };                       // данные с main_pcb
data_rfid_TypeDef data_rfid = { 0 };                       // данные с rfid

error_TypeDef Parse_Main_Message(message_TypeDef *message)
{
	error_TypeDef er = DATA_NO_ERROR;
	if(message->rx_size != message->m_rx[2] + SIZE_OFFSET)
	{
		er = SIZE_ERROR;
	}
	else if(message->m_rx[0] != PREAMBLE)
	{
		er = PREAMBLE_ERROR;
	}
	else if(message->m_rx[1] != ADDRESS)
	{
		er = ADDRESS_ERROR;
	}
	else if(message->m_rx[message->rx_size - 1] != CRC8(message->m_rx, message->rx_size - 1))
	{
		er = CRC_ERROR;
	}
	else
	{
		data_main.cmd = message->m_rx[3];
		data_main.value = message->m_rx[4];
		switch(data_main.cmd)
		{
		case GET_STATE:
			Send_State(message);
			break;
		case LED:
			Led(1, data_main.value);
			break;
		default:
			break;
		}
	}
	return er;
}

error_RF_TypeDef Parse_RFID_Message(message_TypeDef *message)
{
	error_RF_TypeDef er = RF_DATA_NO_ERROR;
	if(message->rx_size != message->m_rx[1] + RF_SIZE_OFFSET)
	{
		er = RF_SIZE_ERROR;
	}
	else if(message->m_rx[0] != RF_PREAMBLE)
	{
		er = RF_PREAMBLE_ERROR;
	}
	else if((message->m_rx[2] != CMD_TAG_UID) && (message->m_rx[2] != CMD_VERSION))
	{
		er = RF_COMMAND_ERROR;
	}
	else if(message->m_rx[message->rx_size - 1] != CRC8(message->m_rx, message->rx_size - 1))
	{
		er = RF_CRC_ERROR;
	}
	else
	{
		data_rfid.cmd = message->m_rx[2];
		if(data_rfid.cmd == CMD_VERSION)
		{
			for(uint8_t i = 0; i < TAG_VER_SIZE; i++)
			{
				data_rfid.tag_version[i] = message->m_rx[i + RFID_PAYLOAD_VERSION_OFFSET];
			}
		}
		else if(data_rfid.cmd == CMD_TAG_UID)
		{
			data_rfid.uid_len = message->m_rx[3];
			for(uint8_t i = 0; i < data_rfid.uid_len; i++)
			{
				data_rfid.tag_uid[i] = message->m_rx[i + RFID_PAYLOAD_TAG_OFFSET];
			}
			data_rfid.value_sensors = message->m_rx[data_rfid.uid_len + RFID_PAYLOAD_TAG_OFFSET];
		}
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
	uint8_t sensors = Get_Sensor_Byte();
	message->m_tx[0] = PREAMBLE;
	message->m_tx[1] = ADDRESS;
	message->m_tx[2] = TX_PAYLOAD_SIZE;
	message->m_tx[3] = Get_Tag_UID();						// RFID Tag
	message->m_tx[4] = (sensors | ((data_rfid.value_sensors << 4) & 0x70));
	message->m_tx[5] = MM_Get_Motor_Pos();								// Motor hinge position
	message->m_tx[6] = MM_Get_Arm_Pos();								// Arm hinge position
	message->m_tx[7] = Get_Button_Counter();
	message->m_tx[8] = Get_Encoder_Value();
	message->m_tx[9] = CRC8(message->m_tx, TX_PAYLOAD_SIZE + SIZE_OFFSET - 1);
	message->tx_size = TX_PAYLOAD_SIZE + SIZE_OFFSET;
	Send_Message(message);
}

void Send_Request_RF_Version(message_TypeDef *message)
{
	message->m_tx[0] = RF_PREAMBLE;
	message->m_tx[1] = RF_TX_PAYLOAD_SIZE_VERSION;
	message->m_tx[2] = 0x01;
	message->m_tx[3] = CRC8(message->m_tx, RF_TX_PAYLOAD_SIZE_VERSION + RF_SIZE_OFFSET - 1);
	message->tx_size = 4;
	Send_Message(message);
}

void Send_Request_RF_Tag(message_TypeDef *message)
{
	if(RFID_Flag == SET)
	{
		message->m_tx[0] = RF_PREAMBLE;
		message->m_tx[1] = RF_TX_PAYLOAD_SIZE_TAG;
		message->m_tx[2] = 0x02;								// RFID Tag
		message->m_tx[3] = CRC8(message->m_tx, RF_TX_PAYLOAD_SIZE_TAG + RF_SIZE_OFFSET - 1);    //преамб+длина+команда
		message->tx_size = 4;
		Send_Message(message);
		RFID_Flag = RESET;
	}
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

uint8_t Get_Tag_UID(void)
{
	return data_rfid.tag_uid[0];
}

void Led(uint8_t led, GPIO_PinState status)
{
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, status);
}
