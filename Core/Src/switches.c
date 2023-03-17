#include "switches.h"
#include "main.h"

typedef struct					// структура концевика
{
	uint16_t pin;				// пин
	GPIO_TypeDef* port;			// порт
	switch_state_TypeDef state; // состояние концевика
	uint8_t checkCounter;		// временный счетчик
	uint8_t previousState;		// предыдущее считанной состояние
	uint8_t counter;			// счетчик нажатий (для кнопки)
}switch_TypeDef;


volatile switch_TypeDef switches[SWITCHES_NUMBER] = {0};	// массив структур концевиков
volatile uint8_t checkSwitchPeriod = CHECK_SWITCH_PERIOD;	// переменная формирует период опроса,
															// период SysTick x checkSwitchPeriod = 10мс

void Init_Switches(void)									// заполняем порты и пины в структурах концевиков
{
	switches[BUTTON].pin = Button_Pin;
	switches[BUTTON].port = Button_GPIO_Port;
	switches[ASS_DET].pin = Ass_det_Pin;
	switches[ASS_DET].port = Ass_det_GPIO_Port;
	switches[L_RING].pin = L_Ring_Pin;
	switches[L_RING].port = L_Ring_GPIO_Port;
	switches[ARM_SW].pin = Arm_sw_Pin;
	switches[ARM_SW].port = Arm_sw_GPIO_Port;
	switches[MOTOR_SW].pin = Motor_sw_Pin;
	switches[MOTOR_SW].port = Motor_sw_GPIO_Port;
}

static void Check_Switches(void)
{
	for(uint8_t i = 0; i < SWITCHES_NUMBER; i++)  // проверяем в цикле все концевики
	{
		switch_state_TypeDef tempState;
		tempState = HAL_GPIO_ReadPin(switches[i].port, switches[i].pin); // считываем состояние пина
		if(tempState == switches[i].previousState)			// если состяние такое же, как в предыдущем опросе
		{
			switches[i].checkCounter++;						// увеличиваем временный счетчик
			if(switches[i].checkCounter == NUMBER_OF_READS)	// если 3 раза подряд считали одинаковое состояние
			{
				if((switches[i].state == RELEASED) && (tempState == PRESSED))
				{
					switches[i].counter++;					// для кнопки увеливаем счетчик нажатий
				}
				switches[i].state = switches[i].previousState;	// сохраняем текущее состояние
				switches[i].checkCounter = 0;					// сбрасываем временный счетчик
			}
		}
		else										// если текущее состояние пина отличается от предыдущего
		{
			switches[i].previousState = tempState;  // запоминаем новое состояние
			switches[i].checkCounter = 0;			// сбрасываем временный счетчик
		}
	}
}

void Check_Switches_Period(void)					// в функции формируется период опроса
{
	if(checkSwitchPeriod != 0)
	{
		checkSwitchPeriod--;
		if(checkSwitchPeriod == 0)
		{
			Check_Switches();
			checkSwitchPeriod = CHECK_SWITCH_PERIOD;
		}
	}
}

switch_state_TypeDef Get_Switch_State(switch_name_TypeDef name)
{
	return switches[name].state;
}

uint8_t Get_Button_Counter(void)
{
	return switches[BUTTON].counter;
}

uint8_t Get_Sensor_Byte(void)
{
	uint8_t byte = 0;
	GPIO_PinState led_status;
	for(uint8_t i = 0; i < SWITCHES_NUMBER; i++)
	{
		byte |= (switches[i].state << i);
	}
	led_status = HAL_GPIO_ReadPin(LED_GPIO_Port, LED_Pin);
	byte |= (led_status << SWITCHES_NUMBER);
	return byte;
}
