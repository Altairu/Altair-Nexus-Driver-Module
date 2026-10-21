/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32h7xx_hal.h"

#include "stm32h7xx_nucleo.h"
#include <stdio.h>

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SOLENOID8_Pin GPIO_PIN_2
#define SOLENOID8_GPIO_Port GPIOE
#define SOLENOID9_Pin GPIO_PIN_3
#define SOLENOID9_GPIO_Port GPIOE
#define SOLENOID10_Pin GPIO_PIN_4
#define SOLENOID10_GPIO_Port GPIOE
#define SOLENOID1_Pin GPIO_PIN_2
#define SOLENOID1_GPIO_Port GPIOF
#define SOLENOID2_Pin GPIO_PIN_3
#define SOLENOID2_GPIO_Port GPIOF
#define SOLENOID3_Pin GPIO_PIN_4
#define SOLENOID3_GPIO_Port GPIOF
#define SOLENOID4_Pin GPIO_PIN_15
#define SOLENOID4_GPIO_Port GPIOF
#define LIMIT_SW1_Pin GPIO_PIN_0
#define LIMIT_SW1_GPIO_Port GPIOG
#define LIMIT_SW2_Pin GPIO_PIN_1
#define LIMIT_SW2_GPIO_Port GPIOG
#define ENC3_CS_Pin GPIO_PIN_10
#define ENC3_CS_GPIO_Port GPIOE
#define ENC4_CS_Pin GPIO_PIN_12
#define ENC4_CS_GPIO_Port GPIOE
#define EXT_LED3_Pin GPIO_PIN_15
#define EXT_LED3_GPIO_Port GPIOE
#define EXT_LED1_Pin GPIO_PIN_10
#define EXT_LED1_GPIO_Port GPIOD
#define EXT_LED2_Pin GPIO_PIN_11
#define EXT_LED2_GPIO_Port GPIOD
#define ENC1_CS_Pin GPIO_PIN_14
#define ENC1_CS_GPIO_Port GPIOD
#define ENC2_CS_Pin GPIO_PIN_15
#define ENC2_CS_GPIO_Port GPIOD
#define LIMIT_SW3_Pin GPIO_PIN_2
#define LIMIT_SW3_GPIO_Port GPIOG
#define LIMIT_SW4_Pin GPIO_PIN_3
#define LIMIT_SW4_GPIO_Port GPIOG
#define LIMIT_SW5_Pin GPIO_PIN_4
#define LIMIT_SW5_GPIO_Port GPIOG
#define LIMIT_SW6_Pin GPIO_PIN_5
#define LIMIT_SW6_GPIO_Port GPIOG
#define LIMIT_SW7_Pin GPIO_PIN_8
#define LIMIT_SW7_GPIO_Port GPIOG
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define SOLENOID5_Pin GPIO_PIN_2
#define SOLENOID5_GPIO_Port GPIOD
#define SOLENOID6_Pin GPIO_PIN_3
#define SOLENOID6_GPIO_Port GPIOD
#define SOLENOID7_Pin GPIO_PIN_4
#define SOLENOID7_GPIO_Port GPIOD
#define LIMIT_SW8_Pin GPIO_PIN_9
#define LIMIT_SW8_GPIO_Port GPIOG
#define LIMIT_SW9_Pin GPIO_PIN_10
#define LIMIT_SW9_GPIO_Port GPIOG
#define LIMIT_SW10_Pin GPIO_PIN_12
#define LIMIT_SW10_GPIO_Port GPIOG
#define LIMIT_SW11_Pin GPIO_PIN_14
#define LIMIT_SW11_GPIO_Port GPIOG
#define LIMIT_SW12_Pin GPIO_PIN_15
#define LIMIT_SW12_GPIO_Port GPIOG

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
