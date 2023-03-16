/*
 * parser.c
 *
 *  Created on: Nov 28, 2020
 *      Author: nkell
 */

#include "FreeRTOS.h"
#include "variables.h"
#include "parser.h"
#include "LED.h"
#include "string.h"
#include "usart.h"
#include "servo.h"
#include "stdio.h"


	uint8_t buffer[100];
	uint8_t buf_in[20];

void parser_task(void *params){

	uint8_t char_in;
	uint8_t cnt_in = 0;

	while(1){
		char_in = '>';
		HAL_UART_Transmit(&huart2, &char_in, 1, portMAX_DELAY);
		while(char_in != '\r'){
			HAL_UART_Receive(&huart2, &char_in, 1, portMAX_DELAY);
			buf_in[cnt_in] = char_in;
			cnt_in++;
			HAL_UART_Transmit(&huart2, &char_in, 1, portMAX_DELAY);
			if(cnt_in > 19){
				cnt_in = 0;
				sprintf((char*)buffer, "Stop trying to overflow the buffer!!!!\r\n>");
				HAL_UART_Transmit(&huart2, buffer, strlen((char*)buffer), portMAX_DELAY);
			}
		}
		if(!(x_in_line(buf_in, cnt_in))){
			process_line(buf_in);
			buf_in[0] = 'n';
			buf_in[1] = 'n';
		}
		cnt_in = 0;
	}

}


int x_in_line(uint8_t buf[], uint8_t len){
	for(int i = 0;i<len;i++){
		if((buf[i] == 'x') || (buf[i] == 'X')){
				return 1;
		}
	}
	return 0;
}


void process_line(uint8_t buf[]){
	switch (buf[0]) {
		case 'p':
			Green_LED_Off();
			Red_LED_Off();
			s1_paused = 1;
			break;
		case 'P':
			Green_LED_Off();
			Red_LED_Off();
			s1_paused = 1;
			break;
		case 'c':
			s1_paused = 0;
			break;
		case 'C':
			s1_paused = 0;
			break;
		case 'r':
			if(s1_paused && (s1_pos != 0)){
				s1_pos++;
				set_s1_pos(s1_pos);
			}
			break;
		case 'R':
			if(s1_paused && (s1_pos != 0)){
				s1_pos++;
				set_s1_pos(s1_pos);
			}
			break;
		case 'l':
			if(s1_paused && (s1_pos != 5)){
				s1_pos--;
				set_s1_pos(s1_pos);
			}
			break;
		case 'L':
			if(s1_paused && (s1_pos != 5)){
				s1_pos--;
				set_s1_pos(s1_pos);
			}
			break;
		case 'n':

			break;
		case 'N':

			break;
		case 'b':
			s1_end = 0;
			s1_err = 0;
			break;
		case 'B':
			s1_end = 0;
			s1_err = 0;
			break;
		default:
			break;
	}


	switch (buf[1]) {
		case 'p':
			s2_paused = 1;
			break;
		case 'P':
			s2_paused = 1;
			break;
		case 'c':
			s2_paused = 0;
			break;
		case 'C':
			s2_paused = 0;
			break;
		case 'r':
			if(s2_paused && (s2_pos != 0)){
				s2_pos++;
				set_s2_pos(s2_pos);
			}
			break;
		case 'R':
			if(s2_paused && (s2_pos != 0)){
				s2_pos++;
				set_s2_pos(s2_pos);
			}
			break;
		case 'l':
			if(s2_paused && (s2_pos != 5)){
				s2_pos--;
				set_s2_pos(s2_pos);
			}
			break;
		case 'L':
			if(s2_paused && (s2_pos != 5)){
				s2_pos--;
				set_s2_pos(s2_pos);
			}
			break;
		case 'n':

			break;
		case 'N':

			break;
		case 'b':
			s2_end = 0;
			s2_err = 0;
			break;
		case 'B':
			s2_end = 0;
			s2_err = 0;
			break;
		default:
			break;
	}
}
