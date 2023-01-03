#ifndef __SWITCHES_H
#define __SWITCHES_H

#include <stdint.h>
#include <stdbool.h>

#define SWITCHES_NUMBER				5			// количество концевиков
#define CHECK_SWITCH_PERIOD			10			// период опроса концевиков SysTick x CHECK_SWITCH_PERIOD = 10мс

typedef enum									// возможные состояния концевиков
{
	PRESSED 	 = 0,
	RELEASED	 = 1
}switch_state_TypeDef;

typedef enum									// названия концевиков
{
	BUTTON = 0,
	ASS_DET,
	L_RING,
	ARM_SW,
	MOTOR_SW
}switch_name_TypeDef;

void Init_Switches(void);											// проинициализировать структуры концевиков
switch_state_TypeDef Get_Switch_State(switch_name_TypeDef);			// получить текущее состояние концевика
uint8_t Get_Button_Counter(void);									// получить счетчик нажатия кнопки
void Check_Switches_Period(void);									// проверка

#endif
