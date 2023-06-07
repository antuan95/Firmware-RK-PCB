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
#define MM_STAT2							0x07
#define MM_T_L								0x08
#define MM_T_H								0x09
#define MM_CID								0x0A
#define MM_DID								0x0B
#define MM_CTRL								0x0E
#define MM_CUST_CTRL2					0x0F
#define MM_RST								0x11
#define MM_OSR_DIG_FILT				0x14
#define MM_T_EN_DIG_FILT			0x15


#define TOTAL_REG_NUMBER			0x16

#define MM_NUMBERS            2

#define MM_PERIOD             1000

typedef struct
{
	I2C_HandleTypeDef *i2c_h;
	int16_t x;
	int16_t y;
	int16_t z;
	int16_t t;
	uint8_t error;
}mm_TypeDef;

mm_TypeDef* MM_Init(I2C_HandleTypeDef *i2c_handle);
void MM_Enable(mm_TypeDef *mm_t);
void MM_Write_Register(mm_TypeDef *mm_t, uint8_t reg, uint8_t *data);
void MM_Read_Register(mm_TypeDef *mm_t, uint8_t reg, uint8_t *data);
uint8_t MM_Get_Chip_ID(mm_TypeDef *mm_t);
void MM_Get_Data(mm_TypeDef *mm_t);
void MM_Get_All(mm_TypeDef *mm_t);
uint8_t MM_Get_Stat(mm_TypeDef *mm_t);
void MM_Res(void);
void MM_Poll_Period(void);
uint8_t MM_Get_Motor_Pos(void);
uint8_t MM_Get_Arm_Pos(void);


#endif
