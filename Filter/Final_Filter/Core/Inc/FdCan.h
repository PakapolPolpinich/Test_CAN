/*
 * FdCan.h
 *
 *  Created on: Jun 4, 2025
 *      Author: Admin
 */

#ifndef INC_FDCAN_H_
#define INC_FDCAN_H_

#include "stm32h5xx_hal.h"
#include <stdio.h>
#include "uart.h"

extern FDCAN_HandleTypeDef hfdcan1;
extern FDCAN_FilterTypeDef sFilterConfig;

void MX_FDCAN1_Init(void);
void Set_Filter(uint8_t index,uint16_t Msg,uint16_t Msk);

#endif /* INC_FDCAN_H_ */
