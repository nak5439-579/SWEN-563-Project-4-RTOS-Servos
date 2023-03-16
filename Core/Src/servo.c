/*
 * servo.c
 *
 *  Created on: Nov 28, 2020
 *      Author: nkell
 */
#include "FreeRTOS.h"
#include "servo.h"
#include "task.h"
#include "stdint.h"
#include "tim.h"
#include "LED.h"
#include "usart.h"
#include "stdlib.h"
#include "variables.h"




uint8_t param_mask = 0x1F; //0001 1111
uint8_t opcode_mask = 0xE0;//1110 0000
int moving_scale = 200;//ms per position change
uint8_t buffer[];

void servo_1_task(void *params){
	set_s1_pos(s1_pos);
	uint8_t recipe[] = {
			0x20,0x25,0x20,0x23,0x80,0x21,0x24,0xA0,0x20,0x22,
			0x40,0x23,0x40,0x22,0x23,0x5F,0x5F,0x5F,0x24,0x00
	};

	uint8_t recipe1[] = {
			0x20,0x26,0x20,0x23,0x80,0x21,0x80,0x24,0xA0,0x20,0x22,
			0x40,0x23,0x40,0x22,0x23,0x5F,0x5F,0x5F,0x24,0x00
	};
	servo_control controls;
	controls.recipe_length = sizeof recipe / sizeof recipe[0];
	controls.current_instruction = 0;
	controls.in_loop = 0;
	s1_paused = 1;

	while(1){

		while(!(s1_end || s1_err || s1_paused)){
			Red_LED_Off();
			Green_LED_On();
			switch ((recipe[controls.current_instruction] & opcode_mask) >> 5) {
				case 1://mov
					if((recipe[controls.current_instruction] & param_mask) > 5){
						Red_LED_On();
						Green_LED_Off();
						controls.current_instruction = 0;
						controls.in_loop = 0;
						controls.loop_count = 0;
						s1_err = 1;
					} else {
						sprintf(buffer, "\033[2;1H\033[Kmov%d",recipe[controls.current_instruction] & param_mask);
						HAL_UART_Transmit(&huart2, (uint8_t *)buffer, strlen(buffer), 10000);
						set_s1_pos(recipe[controls.current_instruction] & param_mask);
					}
					break;
				case 2://wait
					sprintf(buffer, "\033[2;1H\033[Kwait%d",recipe[controls.current_instruction] & param_mask);
					HAL_UART_Transmit(&huart2, (uint8_t *)buffer, strlen(buffer), 10000);
					vTaskDelay(pdMS_TO_TICKS(100*(recipe[controls.current_instruction] & param_mask)));
					break;
				case 4://loop
					if(controls.in_loop){
						sprintf(buffer, "\033[2;1H\033[Klooperr%d",recipe[controls.current_instruction] & param_mask);
						HAL_UART_Transmit(&huart2, (uint8_t *)buffer, strlen(buffer), 10000);
						controls.current_instruction = 0;
						controls.in_loop = 0;
						controls.loop_count = 0;
						Red_LED_On();
						Green_LED_On();
						s1_err = 1;
					} else {
						sprintf(buffer, "\033[2;1H\033[Kloop%d",recipe[controls.current_instruction] & param_mask);
						HAL_UART_Transmit(&huart2, (uint8_t *)buffer, strlen(buffer), 10000);
						controls.in_loop = 1;
						controls.num_loops = recipe[controls.current_instruction] & param_mask;
						controls.loop_begin = controls.current_instruction;
					}
					break;
				case 5://end loop
					if(controls.loop_count < controls.num_loops){
						sprintf(buffer, "\033[2;1H\033[Kloopendback%d",recipe[controls.current_instruction] & param_mask);
						HAL_UART_Transmit(&huart2, (uint8_t *)buffer, strlen(buffer), 10000);
						controls.current_instruction = controls.loop_begin;
						controls.loop_count++;
					} else {
						sprintf(buffer, "\033[2;1H\033[Kloopendreal%d",recipe[controls.current_instruction] & param_mask);
						HAL_UART_Transmit(&huart2, (uint8_t *)buffer, strlen(buffer), 10000);
						controls.in_loop = 0;
						controls.num_loops = 0;
						controls.loop_count = 0;
					}
					break;
				case 0://rec end
					sprintf(buffer, "\033[2;1H\033[Kend%d",recipe[controls.current_instruction] & param_mask);
					HAL_UART_Transmit(&huart2, (uint8_t *)buffer, strlen(buffer), 10000);
					controls.current_instruction = 0;
					controls.in_loop = 0;
					controls.loop_count = 0;
					s1_end = 1;
					break;
				default:
					Red_LED_On();
					controls.current_instruction = 0;
					controls.in_loop = 0;
					controls.loop_count = 0;
					s1_err = 1;
					break;
			}

			if(!(s1_end || s1_err || s1_paused)){
				vTaskDelay(pdMS_TO_TICKS(500));//wait for "system cycle"
				controls.current_instruction++;
			}

		}
		osDelay(1);

	}





}


void servo_2_task(void* params){
	set_s2_pos(s2_pos);
	uint8_t recipe[] = {
			0x20,0x25,0x20,0x23,0x80,0x21,0x24,0xA0,0x20,0x22,
			0x40,0x23,0x40,0x22,0x23,0x5F,0x5F,0x5F,0x24,0x00
	};

	uint8_t recipe1[] = {
			0x20,0x26,0x20,0x23,0x80,0x21,0x80,0x24,0xA0,0x20,0x22,
			0x40,0x23,0x40,0x22,0x23,0x5F,0x5F,0x5F,0x24,0x00
	};
	servo_control controls;
	controls.recipe_length = sizeof recipe / sizeof recipe[0];
	controls.current_instruction = 0;
	controls.in_loop = 0;
	s2_paused = 1;

	while(1){

		while(!(s2_end || s2_err || s2_paused)){

			switch ((recipe[controls.current_instruction] & opcode_mask) >> 5) {
				case 1://mov
					if((recipe[controls.current_instruction] & param_mask) > 5){
						controls.current_instruction = 0;
						controls.in_loop = 0;
						controls.loop_count = 0;
						s2_err = 1;
					} else {
						set_s2_pos(recipe[controls.current_instruction] & param_mask);
					}
					break;
				case 2://wait
					vTaskDelay(pdMS_TO_TICKS(100*(recipe[controls.current_instruction] & param_mask)));
					break;
				case 4://loop
					if(controls.in_loop){
						controls.current_instruction = 0;
						controls.in_loop = 0;
						controls.loop_count = 0;
						s2_err = 1;
					} else {
						controls.in_loop = 1;
						controls.num_loops = recipe[controls.current_instruction] & param_mask;
						controls.loop_begin = controls.current_instruction;
					}
					break;
				case 5://end loop
					if(controls.loop_count < controls.num_loops){
						controls.current_instruction = controls.loop_begin;
						controls.loop_count++;
					} else {
						controls.in_loop = 0;
						controls.num_loops = 0;
						controls.loop_count = 0;
					}
					break;
				case 0://rec end
					controls.current_instruction = 0;
					controls.in_loop = 0;
					controls.loop_count = 0;
					s2_end = 1;
					break;
				default:
					Red_LED_On();
					controls.current_instruction = 0;
					controls.in_loop = 0;
					controls.loop_count = 0;
					s2_err = 1;
					break;
			}

			if(!(s2_end || s2_err || s2_paused)){
				vTaskDelay(pdMS_TO_TICKS(500));//wait for "system cycle"
				controls.current_instruction++;
			}

		}
		osDelay(1);

	}





}


void set_s1_pos(uint8_t pos){
	switch (pos) {
		case 0:
			TIM2->CCR1 = pos_0_pulse_time;
			vTaskDelay(pdMS_TO_TICKS(abs(s1_pos - pos) * moving_scale));
			s1_pos = pos;
			break;
		case 1:
			TIM2->CCR1 = pos_1_pulse_time;
			vTaskDelay(pdMS_TO_TICKS(abs(s1_pos - pos) * moving_scale));
			s1_pos = pos;
			break;
		case 2:
			TIM2->CCR1 = pos_2_pulse_time;
			vTaskDelay(pdMS_TO_TICKS(abs(s1_pos - pos) * moving_scale));
			s1_pos = pos;
			break;
		case 3:
			TIM2->CCR1 = pos_3_pulse_time;
			vTaskDelay(pdMS_TO_TICKS(abs(s1_pos - pos) * moving_scale));
			s1_pos = pos;
			break;
		case 4:
			TIM2->CCR1 = pos_4_pulse_time;
			vTaskDelay(pdMS_TO_TICKS(abs(s1_pos - pos) * moving_scale));
			s1_pos = pos;
			break;
		case 5:
			TIM2->CCR1 = pos_5_pulse_time;
			vTaskDelay(pdMS_TO_TICKS(abs(s1_pos - pos) * moving_scale));
			s1_pos = pos;
			break;
		default:
			break;
	}
}


void set_s2_pos(uint8_t pos){
	switch (pos) {
		case 0:
			TIM2->CCR2 = pos_0_pulse_time;
			vTaskDelay(pdMS_TO_TICKS(abs(s1_pos - pos) * moving_scale));
			s2_pos = pos;
			break;
		case 1:
			TIM2->CCR2 = pos_1_pulse_time;
			vTaskDelay(pdMS_TO_TICKS(abs(s1_pos - pos) * moving_scale));
			s2_pos = pos;
			break;
		case 2:
			TIM2->CCR2 = pos_2_pulse_time;
			vTaskDelay(pdMS_TO_TICKS(abs(s1_pos - pos) * moving_scale));
			s2_pos = pos;
			break;
		case 3:
			TIM2->CCR2 = pos_3_pulse_time;
			vTaskDelay(pdMS_TO_TICKS(abs(s1_pos - pos) * moving_scale));
			s2_pos = pos;
			break;
		case 4:
			TIM2->CCR2 = pos_4_pulse_time;
			vTaskDelay(pdMS_TO_TICKS(abs(s1_pos - pos) * moving_scale));
			s2_pos = pos;
			break;
		case 5:
			TIM2->CCR2 = pos_5_pulse_time;
			vTaskDelay(pdMS_TO_TICKS(abs(s1_pos - pos) * moving_scale));
			s2_pos = pos;
			break;
		default:
			break;
	}
}


