/*
 * timer.h
 *
 *  Created on: Apr 2, 2025
 *      Author: Shuvro
 */

#ifndef TIMER_H_
#define TIMER_H_
#define TIMx_SR_UIF			(1U<<0)
#define TIMx_SR_CC1F		(1U<<1)
void tim2_pa5_output_compare(void);
void tim3_pa6_input_capture(void);
void tim2_1hz_init(void);



#endif /* TIMER_H_ */
