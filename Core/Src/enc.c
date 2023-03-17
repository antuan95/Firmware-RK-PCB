#include "enc.h"
#include "main.h"

typedef struct
{
	int8_t state;
	bool pinA_Value;
	bool pinB_Value;
	uint8_t counter;
}enc_TypeDef;

volatile enc_TypeDef enc;			  // структура энкодера
volatile bool debounce;				  // статус для процесса антидребезга
uint16_t pin;						  // сработавший пин

static void Set_Count(int8_t state)   // Устанавливаем значение счетчика
{
  if (state == 4 || state == -4)  // Если переменная state приняла заданное значение приращения
  {
    enc.counter += (int8_t)(state / 4);      // Увеличиваем/уменьшаем счетчик
  }
}

uint8_t Get_Encoder_Value(void) // получить значение энкодера
{
	return enc.counter;
}

void Enc_Debounce(TIM_HandleTypeDef *tim, uint16_t GPIO_Pin)
{
	if(debounce == DEBOUNCE_READY)
	{
		pin = GPIO_Pin;						// сохраняем значение сработавшего пина
		__HAL_TIM_CLEAR_IT(tim, TIM_IT_UPDATE);
		__HAL_TIM_SET_COUNTER(tim, 0);
		HAL_TIM_Base_Start_IT(tim);			// запускаем таймер на антидребезг
		debounce = DEBOUNCE_BUSY;			// блокируем обработку сигналов на время антидребезга
	}
}

void Enc_Handler()
{
	enc.pinA_Value = HAL_GPIO_ReadPin(EncA_GPIO_Port, EncA_Pin);	// Получаем состояние пинов A и B
	enc.pinB_Value = HAL_GPIO_ReadPin(EncB_GPIO_Port, EncB_Pin);

	if(pin == EncA_Pin)										// если пришло прерывание по пину А
	{
		if(((enc.state == 0) && (enc.pinA_Value == 0) && (enc.pinB_Value != 0)) ||
		   ((enc.state == 2) && (enc.pinA_Value != 0) && (enc.pinB_Value == 0)))
		{
			enc.state++;
		}
		else if(((enc.state == -1) && (enc.pinA_Value == 0) && (enc.pinB_Value == 0)) ||
				((enc.state == -3) && (enc.pinA_Value != 0) && (enc.pinB_Value != 0)))
		{
			enc.state--;
		}
	}
	if(pin == EncB_Pin)										// если пришло прерывание по пину B
	{
		if(((enc.state == 1) && (enc.pinA_Value == 0) && (enc.pinB_Value == 0)) ||
		   ((enc.state == 3) && (enc.pinA_Value != 0) && (enc.pinB_Value != 0)))
		{
			enc.state++;
		}
		else if(((enc.state == 0) && (enc.pinA_Value != 0) && (enc.pinB_Value == 0)) ||
				((enc.state == -2) && (enc.pinA_Value == 0) && (enc.pinB_Value != 0)))
		{
			enc.state--;
		}
	}
	 Set_Count(enc.state); // Проверяем не было ли полного шага из 4 изменений сигналов (2 импульсов)

	 if ((enc.pinA_Value != 0) && (enc.pinB_Value != 0) && (enc.state != 0))
		 enc.state = 0; // Если что-то пошло не так, возвращаем статус в исходное состояние

	 debounce = DEBOUNCE_READY;
}
