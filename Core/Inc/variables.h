/*
 * variables.h
 *
 *  Created on: Nov 28, 2020
 *      Author: nkell
 */

#ifndef INC_VARIABLES_H_
#define INC_VARIABLES_H_

extern uint32_t pos_0_pulse_time;//project doc says this starts at 400, but datasheet says 1000
extern uint32_t pos_1_pulse_time;
extern uint32_t pos_2_pulse_time;
extern uint32_t pos_3_pulse_time;
extern uint32_t pos_4_pulse_time;
extern uint32_t pos_5_pulse_time;
extern uint8_t s1_paused, s2_paused, s1_err, s2_err, s1_pos, s2_pos, s1_end, s2_end;

extern char* mnemonic_map[];
extern int mnemonic_map_answers[];

#endif /* INC_VARIABLES_H_ */
