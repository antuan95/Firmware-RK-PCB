#ifndef __SWITCHES_H
#define __SWITCHES_H

#include <stdint.h>
#include <stdbool.h>

#define SWITCHES_NUMBER				3			// количество концевиков
#define CHECK_SWITCH_PERIOD			10			// период опроса концевиков SysTick x CHECK_SWITCH_PERIOD = 10мс
#define NUMBER_OF_READS				3			// количество считываний концевика

typedef enum									// возможные состояния концевиков
{
	PRESSED 	 = 0,
	RELEASED	 = 1
}switch_state_TypeDef;

typedef enum									// названия концевиков
{
	BUTTON = 0,
	ARM_SW,
	MOTOR_SW
	//ASS_DET,
	//L_RING,
}switch_name_TypeDef;

void Init_Switches(void);											// проинициализировать структуры концевиков
switch_state_TypeDef Get_Switch_State(switch_name_TypeDef name);	// получить текущее состояние концевика
uint8_t Get_Button_Counter(void);									// получить счетчик нажатия кнопки
void Check_Switches_Period(void);									// проверка
uint8_t Get_Sensor_Byte(void);										// байт состояния концевиков

#endif
