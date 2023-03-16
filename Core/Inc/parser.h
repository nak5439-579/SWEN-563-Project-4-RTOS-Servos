/*
 * parser.h
 *
 *  Created on: Nov 28, 2020
 *      Author: nkell
 */

#ifndef INC_PARSER_H_
#define INC_PARSER_H_

void parser_task(void *params);
int x_in_line(uint8_t buf[], uint8_t len);
void process_line(uint8_t buf[]);


#endif /* INC_PARSER_H_ */
