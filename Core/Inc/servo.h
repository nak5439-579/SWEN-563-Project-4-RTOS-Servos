/*
 * servo.h
 *
 *  Created on: Nov 28, 2020
 *      Author: nkell
 */

#ifndef INC_SERVO_H_
#define INC_SERVO_H_

typedef struct{
	uint8_t recipe_length;
	uint8_t loop_begin;//the index to loop back to
	uint8_t current_instruction;//the current "index" of the recipie
	uint8_t num_loops;//the number of loops to make
	uint8_t loop_count;//the number of times looped
	uint8_t in_loop;
} servo_control;

void servo_1_task(void *params);
void servo_2_task(void *params);
void set_s1_pos(uint8_t pos);
void set_s2_pos(uint8_t pos);
void the_corner_where_bad_recipies_go_to_think_about_their_errors();
#endif /* INC_SERVO_H_ */
