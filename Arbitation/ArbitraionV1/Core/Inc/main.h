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
#include "stm32h5xx_hal.h"

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
#define B1_BLUE_USER_BUTTON_Pin GPIO_PIN_13
#define B1_BLUE_USER_BUTTON_GPIO_Port GPIOC
#define B1_BLUE_USER_BUTTON_EXTI_IRQn EXTI13_IRQn
#define T_VCP_RX_Pin GPIO_PIN_3
#define T_VCP_RX_GPIO_Port GPIOA
#define T_VCP_TX_Pin GPIO_PIN_4
#define T_VCP_TX_GPIO_Port GPIOA
#define BLUE_LED_Pin GPIO_PIN_5
#define BLUE_LED_GPIO_Port GPIOA
#define RED_LED_Pin GPIO_PIN_6
#define RED_LED_GPIO_Port GPIOA
#define YELLOW_LED_Pin GPIO_PIN_7
#define YELLOW_LED_GPIO_Port GPIOA
#define BCD_BIT2_Pin GPIO_PIN_10
#define BCD_BIT2_GPIO_Port GPIOB
#define BCD_BIT0_Pin GPIO_PIN_6
#define BCD_BIT0_GPIO_Port GPIOC
#define BCD_BIT3_Pin GPIO_PIN_7
#define BCD_BIT3_GPIO_Port GPIOC
#define EXTI8_BUTTON1_Pin GPIO_PIN_8
#define EXTI8_BUTTON1_GPIO_Port GPIOC
#define EXTI8_BUTTON1_EXTI_IRQn EXTI8_IRQn
#define GREEN_LED_Pin GPIO_PIN_9
#define GREEN_LED_GPIO_Port GPIOC
#define BCD_BIT1_Pin GPIO_PIN_8
#define BCD_BIT1_GPIO_Port GPIOA
#define EXTI3_BUTTON2_Pin GPIO_PIN_3
#define EXTI3_BUTTON2_GPIO_Port GPIOB
#define EXTI3_BUTTON2_EXTI_IRQn EXTI3_IRQn
#define EXTI4_BUTTON4_Pin GPIO_PIN_4
#define EXTI4_BUTTON4_GPIO_Port GPIOB
#define EXTI4_BUTTON4_EXTI_IRQn EXTI4_IRQn
#define EXTI5_BUTTON3_Pin GPIO_PIN_5
#define EXTI5_BUTTON3_GPIO_Port GPIOB
#define EXTI5_BUTTON3_EXTI_IRQn EXTI5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
