
#include "can.h"

FDCAN_HandleTypeDef hfdcan1;

void MX_FDCAN1_Init(void)
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
    while(1){};
  }
  /* USER CODE BEGIN FDCAN1_Init 2 */
  if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK) {
    	 /* Notification Error */
	  while(1){};

   }
   if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
   {
 	  /* Notification Error */
	   while(1){};

   }

   for (uint8_t i = 0; i < 8 ;i++){
   	  CAN_Payload.dataTx[i] = i;
   }

  /* USER CODE END FDCAN1_Init 2 */

}

void TX_Send_CAN(){
  /* ----- Prepare CAN data --------------------------------------- */
  CAN_Payload.TxHeader.Identifier = CAN_Payload.msgID; //CAN_Payload.msgID
  CAN_Payload.TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  CAN_Payload.TxHeader.BitRateSwitch = FDCAN_BRS_OFF;         // Disable BRS
  CAN_Payload.TxHeader.FDFormat = FDCAN_CLASSIC_CAN;          // Use Classical CAN frame
  CAN_Payload.TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
  CAN_Payload.TxHeader.MessageMarker = 0;
  //CAN_Payload.dataTx[0] ^= 1;
//  GPIOC->ODR |= (1UL << 10U);
//  while((GPIOC->IDR & (1UL << 12U)) == 0U){}

  if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &CAN_Payload.TxHeader,CAN_Payload.dataTx)!= HAL_OK)
  {

  }
  GPIOC->ODR &= ~(1UL << 10U);

}


void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
	if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
	{
		/* Retreive Rx messages from RX FIFO0 */
		if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &CAN_Payload.RxHeader,CAN_Payload.dataRx) != HAL_OK)
		{
			/* Reception Error */
			while(1){};

		}
		if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
		{
			/* Notification Error */
			while(1){};

		}
	}
}


