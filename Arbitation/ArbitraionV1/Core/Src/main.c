/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct{
	FDCAN_TxHeaderTypeDef   TxHeader;
	FDCAN_RxHeaderTypeDef   RxHeader;
	uint32_t msgID;
	uint8_t dataTx[8];
	uint8_t dataRx[8];
} CAN_SET;

CAN_SET CAN_Payload;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NODE_B
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

FDCAN_HandleTypeDef hfdcan1;

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_FDCAN1_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */
void BCD_Display(uint8_t bcd_value);
void TX_Send_CAN();
void Pri_ID();
void Std_Ext();
void Pass_Arb();
void Close_led();
void Turn_LED(uint16_t LED);
void Change_ID();
void Check_flag();
void Data_Remote();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
typedef enum {
    ID_VS_ID = 1U,
    STD_VS_EXD,
    ID_Pass_ARB,
	DATE_REMOTE,
    CHANGE_ADD,
    START_SEND_CAN
} State;

volatile State mode 	= ID_VS_ID;
State prev_mode 		= ID_VS_ID;

volatile uint8_t flag 	= 1U;
uint8_t ID_Position 	= 0U;
volatile uint8_t sw_mode = 0U;


#ifdef NODE_A
	const uint16_t pri_id_address[3]  = {0x7E0U,0x7A0U,0x119U};
	const uint16_t std_ext_address[3] = {0x7E0U,0x2A5U,0x001U};
#endif

#ifdef NODE_B
	const uint16_t pri_id_address[3]  = {0x7E1U,0x1A2U,0x149U};
	const uint32_t std_ext_address[3] = {0x1F800000U,0x10A5462U,0x00040000U};
#endif

#define MAX_ID_POSITION	(sizeof(pri_id_address) / sizeof(pri_id_address[0]))
#define RESET_ID_POSITION (0U)

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FDCAN1_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  BCD_Display(ID_Position+1);
  Close_led();

  for (uint8_t i = 0; i < 8 ;i++){
	  CAN_Payload.dataTx[i] = i;
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	while(flag == 1){
		switch (mode)
    		{
    		case ID_VS_ID:
    			Turn_LED(BLUE_LED_Pin);
    			Pri_ID();
    			break;
    		case STD_VS_EXD:
    			Turn_LED(RED_LED_Pin);
    			Std_Ext();
    			break;
    		case ID_Pass_ARB:
    			Turn_LED(YELLOW_LED_Pin);
    			Pass_Arb();
    			break;
    		case DATE_REMOTE:
    			Turn_LED(GREEN_LED_Pin);
    			Data_Remote();
    			break;
    		case CHANGE_ADD:
    			Change_ID();
    			break;
    		default:
    			break;
    		}
		Check_flag();
	}
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLL1_SOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 3;
  RCC_OscInitStruct.PLL.PLLN = 50;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1_VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1_VCORANGE_WIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the programming delay
  */
  __HAL_FLASH_SET_PROGRAM_DELAY(FLASH_PROGRAMMING_DELAY_2);
}

/**
  * @brief FDCAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_FDCAN1_Init(void)
{

  /* USER CODE BEGIN FDCAN1_Init 0 */

  /* USER CODE END FDCAN1_Init 0 */

  /* USER CODE BEGIN FDCAN1_Init 1 */

  /* USER CODE END FDCAN1_Init 1 */
  hfdcan1.Instance = FDCAN1;
  hfdcan1.Init.ClockDivider = FDCAN_CLOCK_DIV1;
  hfdcan1.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan1.Init.AutoRetransmission = DISABLE;
  hfdcan1.Init.TransmitPause = DISABLE;
  hfdcan1.Init.ProtocolException = DISABLE;
  hfdcan1.Init.NominalPrescaler = 25;
  hfdcan1.Init.NominalSyncJumpWidth = 1;
  hfdcan1.Init.NominalTimeSeg1 = 13;
  hfdcan1.Init.NominalTimeSeg2 = 2;
  hfdcan1.Init.DataPrescaler = 1;
  hfdcan1.Init.DataSyncJumpWidth = 1;
  hfdcan1.Init.DataTimeSeg1 = 1;
  hfdcan1.Init.DataTimeSeg2 = 1;
  hfdcan1.Init.StdFiltersNbr = 0;
  hfdcan1.Init.ExtFiltersNbr = 0;
  hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN1_Init 2 */
  if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK) {
    	 /* Notification Error */
    	Error_Handler();
   }
   if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
   {
 	  /* Notification Error */
  	Error_Handler();
   }

  /* USER CODE END FDCAN1_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
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

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin){

		if (GPIO_Pin == EXTI8_BUTTON1_Pin) {
			mode = ID_VS_ID;
		}
		if (GPIO_Pin == EXTI3_BUTTON2_Pin) {
			mode = STD_VS_EXD;
		}
		if (GPIO_Pin == EXTI4_BUTTON4_Pin) {
			mode = CHANGE_ADD;
		}
		if (GPIO_Pin == B1_BLUE_USER_BUTTON_Pin) {
//			if(sw_mode == 0){
//				mode = ID_Pass_ARB;
//			}else{
//				mode = DATE_REMOTE;
//			}
//			sw_mode ^= 1;
			mode = DATE_REMOTE;
		}
		if (GPIO_Pin == GPIO_PIN_15) {
			TX_Send_CAN();
		}

		if (GPIO_Pin == EXTI5_BUTTON3_Pin){
			mode = ID_Pass_ARB;
		}
		flag = 1;
}
//void EXTI3_IRQHandler(void)
//{
//    if (EXTI->FPR1 & (1U << 3U))  // Check pending
//    {
//        EXTI->FPR1 = (1U << 3U);  // Clear interrupt flag
//        mode = STD_VS_EXD;
//        flag = 1;
//    }
//}
//
//void EXTI4_IRQHandler(void)
//{
//    if (EXTI->FPR1 & (1U << 4U))
//    {
//        EXTI->FPR1 = (1U << 4U);
//        mode = CHANGE_ADD;
//        flag = 1;
//    }
//}
//
//void EXTI5_IRQHandler(void)
//{
//    if (EXTI->FPR1 & (1U << 5U))
//    {
//        EXTI->FPR1 = (1U << 5U);
//        mode = CHANGE_ADD;
//        flag = 1;
//    }
//}
//
//void EXTI8_IRQHandler(void)
//{
//    if (EXTI->FPR1 & (1U << 8U))
//    {
//        EXTI->FPR1 = (1U << 8U);
//        mode = ID_VS_ID;
//        flag = 1;
//    }
//}
//
//void EXTI13_IRQHandler(void)
//{
//    if (EXTI->FPR1 & (1U << 13U))
//    {
//        EXTI->FPR1 = (1U << 13U);
//        mode = ID_Pass_ARB;
//        flag = 1;
//    }
//}



void BCD_Display(uint8_t bcd_value)
{
    bcd_value &= 0x0F;/*set bit*/

    HAL_GPIO_WritePin(BCD_BIT0_GPIO_Port, BCD_BIT0_Pin, (bcd_value & (1 << 0)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BCD_BIT1_GPIO_Port, BCD_BIT1_Pin, (bcd_value & (1 << 1)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BCD_BIT2_GPIO_Port, BCD_BIT2_Pin, (bcd_value & (1 << 2)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BCD_BIT3_GPIO_Port, BCD_BIT3_Pin, (bcd_value & (1 << 3)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void Pri_ID(){
	 CAN_Payload.msgID = pri_id_address[ID_Position];
	 CAN_Payload.TxHeader.IdType = FDCAN_STANDARD_ID;
	 CAN_Payload.TxHeader.DataLength = FDCAN_DLC_BYTES_8;
	 CAN_Payload.TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	 prev_mode = mode;
}

void Pass_Arb(){
	CAN_Payload.msgID = 0x7E2;
	CAN_Payload.TxHeader.IdType = FDCAN_STANDARD_ID;
	CAN_Payload.TxHeader.DataLength = FDCAN_DLC_BYTES_8;

#ifdef NODE_A
	CAN_Payload.TxHeader.DataLength = FDCAN_DLC_BYTES_1;
#endif

#ifdef NODE_B
	CAN_Payload.TxHeader.DataLength = FDCAN_DLC_BYTES_2;
#endif
	CAN_Payload.TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	prev_mode = mode;
	ID_Position = 0;
	BCD_Display(1);

}


void Std_Ext(){

#ifdef NODE_A
	CAN_Payload.TxHeader.IdType = FDCAN_STANDARD_ID;
#endif

#ifdef NODE_B
	CAN_Payload.TxHeader.IdType = FDCAN_EXTENDED_ID;
#endif
	CAN_Payload.msgID = std_ext_address[ID_Position];

	CAN_Payload.TxHeader.DataLength = FDCAN_DLC_BYTES_8;
	CAN_Payload.TxHeader.TxFrameType = FDCAN_DATA_FRAME;

	prev_mode = mode;
}

void Data_Remote(){
	CAN_Payload.msgID = 0x7EA;
	CAN_Payload.TxHeader.DataLength = FDCAN_DLC_BYTES_8;

#ifdef NODE_A
	CAN_Payload.TxHeader.TxFrameType = FDCAN_DATA_FRAME;
#endif

#ifdef NODE_B
	CAN_Payload.TxHeader.TxFrameType = FDCAN_REMOTE_FRAME;
#endif
	ID_Position = 0;
	BCD_Display(1);
}


void TX_Send_CAN(){
  /* ----- Prepare CAN data --------------------------------------- */
  CAN_Payload.TxHeader.Identifier = CAN_Payload.msgID; //CAN_Payload.msgID
  CAN_Payload.TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  CAN_Payload.TxHeader.BitRateSwitch = FDCAN_BRS_OFF;         // Disable BRS
  CAN_Payload.TxHeader.FDFormat = FDCAN_CLASSIC_CAN;          // Use Classical CAN frame
  CAN_Payload.TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
  CAN_Payload.TxHeader.MessageMarker = 0;
  CAN_Payload.dataTx[0] ^= 1;
  if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &CAN_Payload.TxHeader,CAN_Payload.dataTx)!= HAL_OK)
  {

  }

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

void Change_ID(){
	if(prev_mode == ID_Pass_ARB ){
	    ID_Position = RESET_ID_POSITION;
	}else{
	    ID_Position++;
	    if (ID_Position >= MAX_ID_POSITION) {
	    	ID_Position = RESET_ID_POSITION;
	    }
	}
	BCD_Display(ID_Position+1);
}

void Check_flag(){
	if(mode == CHANGE_ADD){
		flag = 1;
		mode = prev_mode;
	}else {
		flag = 0;
	}
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
	if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
	{
		/* Retreive Rx messages from RX FIFO0 */
		if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &CAN_Payload.RxHeader,CAN_Payload.dataRx) != HAL_OK)
		{
			/* Reception Error */
			Error_Handler();
		}
		if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
		{
			/* Notification Error */
			Error_Handler();
		}
	}
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
