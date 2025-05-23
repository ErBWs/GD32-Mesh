/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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
#define LED1_Pin GPIO_PIN_3
#define LED1_GPIO_Port GPIOE
#define MQ2_Pin GPIO_PIN_10
#define MQ2_GPIO_Port GPIOF
#define MQ2_ADC_Pin GPIO_PIN_1
#define MQ2_ADC_GPIO_Port GPIOC
#define LED4_Pin GPIO_PIN_5
#define LED4_GPIO_Port GPIOA
#define LORA_AUX_Pin GPIO_PIN_5
#define LORA_AUX_GPIO_Port GPIOC
#define LORA_M0_Pin GPIO_PIN_0
#define LORA_M0_GPIO_Port GPIOB
#define LORA_M1_Pin GPIO_PIN_1
#define LORA_M1_GPIO_Port GPIOB
#define LED3_Pin GPIO_PIN_3
#define LED3_GPIO_Port GPIOG
#define LED2_Pin GPIO_PIN_7
#define LED2_GPIO_Port GPIOD
#define AHT10_SCL_Pin GPIO_PIN_8
#define AHT10_SCL_GPIO_Port GPIOB
#define AHT10_SDA_Pin GPIO_PIN_9
#define AHT10_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
