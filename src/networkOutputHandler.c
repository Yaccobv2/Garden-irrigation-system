/*
 * networkOutputHandler.c
 *
 *  Created on: 21 lut 2022
 *      Author: kubex
 */

#include <networkOutputHandler.h>

void add_string_object_to_json(cJSON* json, char* name, char* value){

	cJSON_AddStringToObject(json, name, value);
}

char* get_message(cJSON* json){
	char *string = cJSON_Print(json);
		if (string == NULL)
		{
		   string = "{\"status\":\"JSON PARSER ERROR\"}";
		   return string;
		}
	return string;

}
