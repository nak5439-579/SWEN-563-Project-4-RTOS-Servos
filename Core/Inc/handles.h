/*
 * handles.h
 *
 *  Created on: Nov 10, 2020
 *      Author: nkell
 */

#ifndef INC_HANDLES_H_
#define INC_HANDLES_H_

extern SemaphoreHandle_t xUSART;
extern SemaphoreHandle_t xBuffer;
//extern QueueHandle_t customer_queue;
extern TaskHandle_t servo1_task_handle;
extern TaskHandle_t servo2_task_handle;
extern TaskHandle_t input_parser_task_handle;




#endif /* INC_HANDLES_H_ */
