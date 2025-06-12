
#include "gpio.h"

void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, BLUE_LED_Pin|RED_LED_Pin|YELLOW_LED_Pin|BCD_BIT1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(BCD_BIT2_GPIO_Port, BCD_BIT2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, BCD_BIT0_Pin|BCD_BIT3_Pin|GREEN_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : B1_BLUE_USER_BUTTON_Pin EXTI8_BUTTON1_Pin */
  GPIO_InitStruct.Pin = B1_BLUE_USER_BUTTON_Pin|EXTI8_BUTTON1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : BLUE_LED_Pin RED_LED_Pin YELLOW_LED_Pin BCD_BIT1_Pin */
  GPIO_InitStruct.Pin = BLUE_LED_Pin|RED_LED_Pin|YELLOW_LED_Pin|BCD_BIT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : BCD_BIT2_Pin */
  GPIO_InitStruct.Pin = BCD_BIT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BCD_BIT2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PB15 EXTI3_BUTTON2_Pin EXTI4_BUTTON4_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_15|EXTI3_BUTTON2_Pin|EXTI4_BUTTON4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : BCD_BIT0_Pin BCD_BIT3_Pin GREEN_LED_Pin */
  GPIO_InitStruct.Pin = BCD_BIT0_Pin|BCD_BIT3_Pin|GREEN_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : EXTI5_BUTTON3_Pin */
  GPIO_InitStruct.Pin = EXTI5_BUTTON3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(EXTI5_BUTTON3_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PC10 */
   GPIO_InitStruct.Pin = GPIO_PIN_10;
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
   HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

   /*Configure GPIO pin : PC12 */
   GPIO_InitStruct.Pin = GPIO_PIN_12;
   GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);


  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI5_IRQn);

  HAL_NVIC_SetPriority(EXTI8_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI8_IRQn);

  HAL_NVIC_SetPriority(EXTI13_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI13_IRQn);

  HAL_NVIC_SetPriority(EXTI15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}


void BCD_Display(uint8_t bcd_value)
{
    bcd_value &= 0x0F;/*set bit*/

    HAL_GPIO_WritePin(BCD_BIT0_GPIO_Port, BCD_BIT0_Pin, (bcd_value & (1 << 0)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BCD_BIT1_GPIO_Port, BCD_BIT1_Pin, (bcd_value & (1 << 1)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BCD_BIT2_GPIO_Port, BCD_BIT2_Pin, (bcd_value & (1 << 2)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BCD_BIT3_GPIO_Port, BCD_BIT3_Pin, (bcd_value & (1 << 3)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void Close_led(){
	HAL_GPIO_WritePin(GPIOA, BLUE_LED_Pin|RED_LED_Pin|YELLOW_LED_Pin,RESET);
	HAL_GPIO_WritePin(GPIOC, GREEN_LED_Pin, RESET);
}

void Turn_LED(uint16_t LED){
	Close_led();
	if(LED == GREEN_LED_Pin){
		HAL_GPIO_WritePin(GPIOC,LED,SET);
	}else{
		HAL_GPIO_WritePin(GPIOA,LED,SET);
	}

}

