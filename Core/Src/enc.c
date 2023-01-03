#include "enc.h"
#include "main.h"

typedef struct
{
	int8_t state;
	bool pinA_Value;
	bool pinB_Value;
	uint32_t lastTurn;
	uint8_t counter;
}enc_TypeDef;

volatile enc_TypeDef enc;			  // структура энкодера

static void Set_Count(int8_t state)   // Устанавливаем значение счетчика
{
  if (state == 4 || state == -4)  // Если переменная state приняла заданное значение приращения
  {
    enc.counter += (int)(state / 4);      // Увеличиваем/уменьшаем счетчик
 //   enc.lastTurn = HAL_GetTick();         // Запоминаем последнее изменение
  }
}

uint8_t Get_Encoder_Value(void) // получить значение энкодера
{
	return enc.counter;
}

void Enc_Handler(uint16_t GPIO_Pin)
{
	//uint32_t ticks = HAL_GetTick();									// Если с момента последнего изменения состояния не прошло
	//if(ticks - enc.lastTurn < ENC_DEBOUNCE_PAUSE) return;			// достаточно времени - выходим из прерывания
	enc.pinA_Value = HAL_GPIO_ReadPin(EncA_GPIO_Port, EncA_Pin);	// Получаем состояние пинов A и B
	enc.pinB_Value = HAL_GPIO_ReadPin(EncB_GPIO_Port, EncB_Pin);

	if(GPIO_Pin == EncA_Pin)										// если пришло прерывание по пину А
	{
		if(((enc.state == 0) && (enc.pinA_Value == 0) && (enc.pinB_Value != 0)) ||
		   ((enc.state == 2) && (enc.pinA_Value != 0) && (enc.pinB_Value == 0)))
		{
			enc.state++;
		//	enc.lastTurn = HAL_GetTick();
		}
		else if(((enc.state == -1) && (enc.pinA_Value == 0) && (enc.pinB_Value == 0)) ||
				((enc.state == -3) && (enc.pinA_Value != 0) && (enc.pinB_Value != 0)))
		{
			enc.state--;
		//	enc.lastTurn = HAL_GetTick();
		}
	}
	if(GPIO_Pin == EncB_Pin)										// если пришло прерывание по пину B
	{
		if(((enc.state == 1) && (enc.pinA_Value == 0) && (enc.pinB_Value == 0)) ||
		   ((enc.state == 3) && (enc.pinA_Value != 0) && (enc.pinB_Value != 0)))
		{
			enc.state++;
		//	enc.lastTurn = HAL_GetTick();
		}
		else if(((enc.state == 0) && (enc.pinA_Value != 0) && (enc.pinB_Value == 0)) ||
				((enc.state == -2) && (enc.pinA_Value == 0) && (enc.pinB_Value != 0)))
		{
			enc.state--;
		//	enc.lastTurn = HAL_GetTick();
		}
	}
	 Set_Count(enc.state); // Проверяем не было ли полного шага из 4 изменений сигналов (2 импульсов)

	 if ((enc.pinA_Value != 0) && (enc.pinB_Value != 0) && (enc.state != 0))
		 enc.state = 0; // Если что-то пошло не так, возвращаем статус в исходное состояние
}
