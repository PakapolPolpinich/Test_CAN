/*
 * can.h
 *
 *  Created on: Jun 11, 2025
 *      Author: Admin
 */

#ifndef INC_CAN_H_
#define INC_CAN_H_

#include "stm32h5xx_hal.h"


typedef struct{
	FDCAN_TxHeaderTypeDef   TxHeader;
	FDCAN_RxHeaderTypeDef   RxHeader;
	uint32_t msgID;
	uint8_t dataTx[8];
	uint8_t dataRx[8];
} CAN_SET;

extern CAN_SET CAN_Payload;

void MX_FDCAN1_Init(void);
void TX_Send_CAN();

#endif /* INC_CAN_H_ */
