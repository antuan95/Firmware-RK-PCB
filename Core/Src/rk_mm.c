#include "rk_mm.h"
#include "main.h"

mm_TypeDef mm_t;

mm_data_TypeDef mm_data;

mm_TypeDef* MM_Init(I2C_HandleTypeDef *i2c_handle)
{
	HAL_GPIO_WritePin(RESB_GPIO_Port, RESB_Pin, GPIO_PIN_SET);
	HAL_Delay(5);
	mm_t.i2c_h = i2c_handle;
	MM_Enable();
	return &mm_t;
}

void MM_Enable(void)
{
	uint8_t chip_id = 0;
	uint8_t cmd = 0x72;
	chip_id = MM_Get_Chip_ID();
	if(chip_id != 0)
		MM_Write_Register(MM_CTRL, &cmd);
	else
		mm_t.error = 1;
}

void MM_Write_Register(uint8_t reg, uint8_t *data)
{
 	HAL_I2C_Mem_Write(mm_t.i2c_h, MM_CHIP_ADDRESS, reg, SIZE_OF_MEM_ADDR, data, 1, TIMEOUT_I2C);
}

void MM_Read_Register(uint8_t reg, uint8_t *data)
{
	HAL_I2C_Mem_Read(mm_t.i2c_h, MM_CHIP_ADDRESS, reg, SIZE_OF_MEM_ADDR, data, 1, TIMEOUT_I2C);
}

void MM_Read_Multiple_Registers(uint8_t start_reg, uint8_t number_of_regs, uint8_t *data)
{
	HAL_I2C_Mem_Read_IT(mm_t.i2c_h, MM_CHIP_ADDRESS, start_reg | 0x80,
									 SIZE_OF_MEM_ADDR, data, number_of_regs);
}

uint8_t MM_Get_Chip_ID(void)
{
	MM_Read_Register(MM_DID, mm_t.rx_data);
	return mm_t.rx_data[0];
}

mm_data_TypeDef* MM_Get_Data(void)
{
	MM_Read_Multiple_Registers(MM_X_L, 8, mm_t.rx_data);
	mm_data.x_l = mm_t.rx_data[0];
	mm_data.x_h = mm_t.rx_data[1];
	mm_data.y_l = mm_t.rx_data[2];
	mm_data.y_h = mm_t.rx_data[3];
	mm_data.z_l = mm_t.rx_data[4];
	mm_data.z_h = mm_t.rx_data[5];
	mm_data.t_l = mm_t.rx_data[6];
	mm_data.t_h = mm_t.rx_data[7];
	return &mm_data;
}
