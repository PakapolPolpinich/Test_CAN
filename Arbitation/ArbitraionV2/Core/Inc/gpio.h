/*
 * gpio.h
 *
 *  Created on: Jun 11, 2025
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

void MX_GPIO_Init(void);

void BCD_Display(uint8_t bcd_value);
void Close_led();
void Turn_LED(uint16_t LED);


#endif /* INC_GPIO_H_ */
