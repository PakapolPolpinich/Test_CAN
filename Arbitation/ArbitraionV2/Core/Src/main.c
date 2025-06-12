
/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define NODE_A

CAN_SET CAN_Payload;

#ifdef NODE_A
	const uint16_t pri_id_address[3]  = {0x7E0U,0x7A0U,0x119U};
	const uint16_t std_ext_address[3] = {0x7E0U,0x2A5U,0x001U};
#endif

#ifdef NODE_B
	const uint16_t pri_id_address[3]  = {0x7E1U,0x1A2U,0x149U};
	const uint32_t std_ext_address[3] = {0x1F800000U,0x10A5462U,0x00040000U};
#endif


void Pri_ID();
void Std_Ext();
void Pass_Arb();
void Data_Remote();
void Change_ID();
void Check_flag();


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


typedef enum { IT_EVENT, WAIT_TO_IT } Status;
typedef enum { STARTSENDCAN,FINISHCAN }Senddata;
typedef enum { CAN_GO,GOEXTI5,} Protect;

volatile Status   Exti_Event = IT_EVENT;
volatile Senddata Can_send = FINISHCAN;
volatile Protect NOT_GO = CAN_GO;

uint8_t ID_Position 	= 0U;
volatile uint8_t sw_mode = 0U;


#define MAX_ID_POSITION	(sizeof(pri_id_address) / sizeof(pri_id_address[0]))
#define RESET_ID_POSITION (0U)


int main(void)
{
 /*set up*/
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_FDCAN1_Init();
 /*setting LED*/
  BCD_Display(ID_Position+1);
  Close_led();
  GPIOC->ODR &= ~(1UL << 10U);

  while (1)
  {
	/*State machine*/
  	while(Exti_Event == IT_EVENT){
		if(Can_send == STARTSENDCAN ){
			TX_Send_CAN();
		}else{
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
		}
		Check_flag();
	}
  }

}



/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin){

		if (GPIO_Pin == EXTI8_BUTTON1_Pin) {
			mode = ID_VS_ID;
			NOT_GO = CAN_GO;
		}
		if (GPIO_Pin == EXTI3_BUTTON2_Pin) {
			mode = STD_VS_EXD;
			NOT_GO = CAN_GO;
		}
		if (GPIO_Pin == EXTI4_BUTTON4_Pin) {
			mode = CHANGE_ADD;
			if((EXTI->FPR1 & (1U << 5U)) != 0U){
				NOT_GO = GOEXTI5;
			}else {
				NOT_GO = CAN_GO;
			}
		}
		if (GPIO_Pin == EXTI5_BUTTON3_Pin) {
			if(NOT_GO != GOEXTI5 ){
				if(sw_mode == 0){
					mode = ID_Pass_ARB;
				}else{
					mode = DATE_REMOTE;
				}
				sw_mode ^= 1;
			}
		}
		if (GPIO_Pin == GPIO_PIN_15) {
			Can_send = STARTSENDCAN;
		}

//		if (GPIO_Pin == EXTI5_BUTTON3_Pin){
//			mode = ID_Pass_ARB;
//		}
		Exti_Event = IT_EVENT;
}


/*Setting about compare between ID VS ID standard */
void Pri_ID(){
	 CAN_Payload.msgID = pri_id_address[ID_Position];
	 CAN_Payload.TxHeader.IdType = FDCAN_STANDARD_ID;
	 CAN_Payload.TxHeader.DataLength = FDCAN_DLC_BYTES_8;
	 CAN_Payload.TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	 prev_mode = mode;
}
/*Setting about what happen after arbitration field*/
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

/*Setting about compare between Standard ID VS  Extended ID  */
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

/*Setting about compare between Data frame VS  Remote frame  */

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

/*Change Address ID  */

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
	if(mode == CHANGE_ADD && Can_send != STARTSENDCAN ){
		Exti_Event = IT_EVENT;
		mode = prev_mode;
	}else {
		Can_send = FINISHCAN;
		Exti_Event = WAIT_TO_IT;
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
