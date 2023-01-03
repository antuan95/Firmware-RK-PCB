#ifndef __ENC_H
#define __ENC_H

#include <stdint.h>
#include <stdbool.h>

#define ENC_DEBOUNCE_PAUSE 		5	// задержка обработки - антидребезг

void Enc_Handler(uint16_t);		  // обработчик прерываний пинов энкодера
uint8_t Get_Encoder_Value(void);  // возвращает значение инкрементального энкодера

#endif
