/*
 * Timer.h
 *
 *  Created on: May 28, 2025
 *      Author: Admin
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include"stm32h5xx.h"

extern uint16_t count;
void Timer2_init();
void TIM2_IRQHandler(void);
void Timer3_init();

#endif /* INC_TIMER_H_ */
