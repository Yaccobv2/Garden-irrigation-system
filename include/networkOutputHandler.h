/*
 * COSTANTS.h
 *
 *  Created on: 21 lut 2022
 *      Author: kubex
 */

#ifndef INC_NETWORKOUTPUTHANDLER_H_
#define INC_NETWORKOUTPUTHANDLER_H_

#include <cJSON.h>

void add_string_object_to_json(cJSON* json, char* name, char* value);
char* get_message(cJSON* json);

#endif /* INC_NETWORKOUTPUTHANDLER_H_ */
