#include "rk_mm.h"
#include "main.h"

mm_TypeDef mm[MM_NUMBERS] = { 0 };                // массив для хранения структур магентометров
uint8_t mm_num = 0;                               // количество магнетометров
volatile uint16_t check_MM_Period = MM_PERIOD;    // период опроса магнетометров

void MM_Res(void)
{
	HAL_GPIO_WritePin(RESB_GPIO_Port, RESB_Pin, GPIO_PIN_RESET);    // формирование сигнала сброса
	HAL_Delay(1);
	HAL_GPIO_WritePin(RESB_GPIO_Port, RESB_Pin, GPIO_PIN_SET);
	HAL_Delay(2);
}

mm_TypeDef* MM_Init(I2C_HandleTypeDef *i2c_handle)                // инициализация магнетометра
{
	mm[mm_num].i2c_h = i2c_handle;                                  // назначить интерфейс i2c
	return &mm[mm_num++];                                           // увеличить количество назначенных интерфейсов
}

void MM_Enable(mm_TypeDef *mm_t)
{
	uint8_t chip_id = 0;
	uint8_t cmd = 0;
	chip_id = MM_Get_Chip_ID(mm_t);                    // прочитать ID магнетометра
	if(chip_id != 0)
	{
		cmd = 0x72;
		MM_Write_Register(mm_t, MM_CTRL, &cmd);           // включение осей и выбор частоты сэмплирования
		cmd = 0xDB;
		MM_Write_Register(mm_t, MM_OSR_DIG_FILT, &cmd);   // фильтры осей xy и оверсэмплинг
		cmd = 0xB3;
		MM_Write_Register(mm_t, MM_T_EN_DIG_FILT, &cmd);  // фильтр оси z и температурная компенсация
		MM_Get_All(mm_t);
		MM_Get_Stat(mm_t);                                // для запуска сэмплирования необходимо прочитать регистр STAT
	}
	else
		mm_t->error = 1;
}

void MM_Write_Register(mm_TypeDef *mm_t, uint8_t reg, uint8_t *data)
{
	HAL_I2C_Mem_Write(mm_t->i2c_h, MM_CHIP_ADDRESS, reg, SIZE_OF_MEM_ADDR, data, 1, TIMEOUT_I2C);
}

void MM_Read_Register(mm_TypeDef *mm_t, uint8_t reg, uint8_t *data)
{
	HAL_I2C_Mem_Read(mm_t->i2c_h, MM_CHIP_ADDRESS, reg, SIZE_OF_MEM_ADDR, data, 1, TIMEOUT_I2C);
}

void MM_Read_Multiple_Registers(mm_TypeDef *mm_t, uint8_t start_reg, uint8_t number_of_regs,
																uint8_t *data)
{
	HAL_I2C_Mem_Read(mm_t->i2c_h, MM_CHIP_ADDRESS, start_reg, SIZE_OF_MEM_ADDR, data, number_of_regs,
										TIMEOUT_I2C);
}

uint8_t MM_Get_Chip_ID(mm_TypeDef *mm_t)
{
	uint8_t mm_data_temp;
	MM_Read_Register(mm_t, MM_DID, &mm_data_temp);
	return mm_data_temp;
}

void MM_Get_Data(mm_TypeDef *mm_t)
{
	if((mm_t->i2c_h != NULL) && (mm_t->error != 1))
	{
		uint8_t mm_data_temp[8];
		MM_Read_Multiple_Registers(mm_t, MM_X_L, 6, mm_data_temp);
		mm_t->x = ((uint16_t) mm_data_temp[1] << 8) | mm_data_temp[0];
		mm_t->y = ((uint16_t) mm_data_temp[3] << 8) | mm_data_temp[2];
		mm_t->z = ((uint16_t) mm_data_temp[5] << 8) | mm_data_temp[4];
		MM_Read_Multiple_Registers(mm_t, MM_T_L, 2, mm_data_temp);
		mm_t->t = (((uint16_t) mm_data_temp[1] << 8) | mm_data_temp[0]) / 5;
	}
}

void MM_Get_All(mm_TypeDef *mm_t)
{
	uint8_t mm_data_temp[TOTAL_REG_NUMBER];
	MM_Read_Multiple_Registers(mm_t, 0, TOTAL_REG_NUMBER, mm_data_temp);
}

uint8_t MM_Get_Stat(mm_TypeDef *mm_t)
{
	uint8_t mm_data_temp;
	MM_Read_Register(mm_t, MM_STAT, &mm_data_temp);
	return mm_data_temp;
}

void MM_Poll_Period(void)
{
	if(check_MM_Period != 0)
	{
		check_MM_Period--;
		if(check_MM_Period == 0)
		{
			for(uint8_t i = 0; i < MM_NUMBERS; i++)
			{
				MM_Get_Data(&mm[i]);
			}
			check_MM_Period = MM_PERIOD;
		}
	}
}

uint8_t MM_Get_Motor_Pos()
{
	return (uint8_t)(mm[0].x >> 8);                         // здесь должна быть обработка данных с магнитометра
}

uint8_t MM_Get_Arm_Pos()
{
	return (uint8_t)(mm[1].x >> 8);                         // здесь должна быть обработка данных с магнитометра
}
