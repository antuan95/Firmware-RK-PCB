/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Ass_det_Pin GPIO_PIN_0
#define Ass_det_GPIO_Port GPIOA
#define L_Ring_Pin GPIO_PIN_1
#define L_Ring_GPIO_Port GPIOA
#define Main_TX_Pin GPIO_PIN_2
#define Main_TX_GPIO_Port GPIOA
#define Main_RX_Pin GPIO_PIN_3
#define Main_RX_GPIO_Port GPIOA
#define Button_Pin GPIO_PIN_4
#define Button_GPIO_Port GPIOA
#define RS485_control_Pin GPIO_PIN_6
#define RS485_control_GPIO_Port GPIOA
#define EncA_Pin GPIO_PIN_7
#define EncA_GPIO_Port GPIOA
#define EncA_EXTI_IRQn EXTI4_15_IRQn
#define EncB_Pin GPIO_PIN_0
#define EncB_GPIO_Port GPIOB
#define EncB_EXTI_IRQn EXTI0_1_IRQn
#define RESB_Pin GPIO_PIN_1
#define RESB_GPIO_Port GPIOB
#define Arm_sw_Pin GPIO_PIN_3
#define Arm_sw_GPIO_Port GPIOB
#define Motor_sw_Pin GPIO_PIN_4
#define Motor_sw_GPIO_Port GPIOB
#define Motor_SCL_Pin GPIO_PIN_6
#define Motor_SCL_GPIO_Port GPIOB
#define Motor_SDA_Pin GPIO_PIN_7
#define Motor_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
