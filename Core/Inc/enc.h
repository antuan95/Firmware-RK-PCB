#ifndef __ENC_H
#define __ENC_H

#include <stdint.h>
#include <stdbool.h>
#ifdef STM32G031xx
#include "stm32g0xx_hal.h"
#endif
#ifdef STM32F407xx
#include "stm32f4xx_hal.h"
#endif


#define ENC_DEBOUNCE_PAUSE 		5	// задержка обработки - антидребезг
#define DEBOUNCE_READY			0
#define DEBOUNCE_BUSY			1

void Enc_Handler();										        // обработчик прерываний пинов энкодера
uint8_t Get_Encoder_Value(void);  			  					// возвращает значение инкрементального энкодера
void Enc_Debounce(TIM_HandleTypeDef *tim, uint16_t GPIO_Pin);	// антидребезг

#endif
