/*
 * gpio.h
 *
 *  Created on: May 27, 2025
 *      Author: Admin
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include "stm32h5xx_hal.h"


#define B1_BLUE_USER_BUTTON_Pin GPIO_PIN_13
#define B1_BLUE_USER_BUTTON_GPIO_Port GPIOC
#define B1_BLUE_USER_BUTTON_EXTI_IRQn EXTI13_IRQn
#define T_VCP_RX_Pin GPIO_PIN_3
#define T_VCP_RX_GPIO_Port GPIOA
#define T_VCP_TX_Pin GPIO_PIN_4
#define T_VCP_TX_GPIO_Port GPIOA
#define USER_LED_Pin GPIO_PIN_5
#define USER_LED_GPIO_Port GPIOA


void MX_GPIO_Init();


#endif /* INC_GPIO_H_ */
