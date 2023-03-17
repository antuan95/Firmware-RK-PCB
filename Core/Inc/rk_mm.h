#ifndef _RK_MM_H
#define _RK_MM_H

#include <stdint.h>
#include <stdbool.h>
#ifdef STM32G031xx
#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_i2c.h"
#endif
#ifdef STM32F407xx
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"
#endif

#define TIMEOUT_I2C		1000
#define SIZE_OF_MEM_ADDR		1
//==================================
#ifdef STM32G031xx
#define MM_CHIP_ADDRESS		0x1A
#endif
#ifdef STM32F407xx
#define MM_CHIP_ADDRESS		0x94
#endif


#define MM_STAT								0x00
#define MM_X_L								0x01
#define MM_X_H								0x02
#define MM_Y_L								0x03
#define MM_Y_H								0x04
#define MM_Z_L								0x05
#define MM_Z_H								0x06
#define MM_T_H								0x07
#define MM_T_L								0x08
#define MM_STAT2							0x09
#define MM_CID								0x0A
#define MM_DID								0x0B
#define MM_CTRL								0x0E
#define MM_CUST_CTRL2					0x0F
#define MM_RST								0x11
#define MM_OSR_DIG_FILT				0x14
#define MM_T_EN_DIG_FILT			0x15

typedef struct
{
	I2C_HandleTypeDef *i2c_h;
	uint8_t tx_data[8];
	uint8_t rx_data[8];
	uint8_t error;
}mm_TypeDef;

typedef struct
{
	uint8_t x_l;
	uint8_t x_h;
	uint8_t y_l;
	uint8_t y_h;
	uint8_t z_l;
	uint8_t z_h;
	uint8_t t_l;
	uint8_t t_h;
}mm_data_TypeDef;

mm_TypeDef* MM_Init(I2C_HandleTypeDef *i2c_handle);
void MM_Enable(void);
void MM_Write_Register(uint8_t reg, uint8_t *data);
void MM_Read_Register(uint8_t reg, uint8_t *data);
uint8_t MM_Get_Chip_ID(void);
mm_data_TypeDef* MM_Get_Data(void);


#endif
