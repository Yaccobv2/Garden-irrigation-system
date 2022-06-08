/*
 * relayControl.c
 *
 *  Created on: Feb 21, 2022
 *      Author: kubex
 */


#include <relayControl.h>
#include "main.h"


void relay_gpio_controller(bool run, GPIO_TypeDef* actuator_port, GPIO_TypeDef* actuator_pin){

	if(run){
		HAL_GPIO_WritePin(actuator_port, actuator_pin, GPIO_PIN_SET);
	}

	if(!run){
			HAL_GPIO_WritePin(actuator_port, actuator_pin, GPIO_PIN_RESET);
	}

}

bool relay_controller(int id, bool state){
	switch(id)
	{
		case 1:
			relay_gpio_controller(state, RELAY1_GPIO_Port, RELAY1_Pin);
			return true;

		case 2:
			relay_gpio_controller(state, RELAY2_GPIO_Port, RELAY2_Pin);
			return true;

		case 3:
			relay_gpio_controller(state, RELAY3_GPIO_Port, RELAY3_Pin);
			return true;

		case 4:
			relay_gpio_controller(state, RELAY4_GPIO_Port, RELAY4_Pin);
			return true;

		case 5:
			relay_gpio_controller(state, RELAY5_GPIO_Port, RELAY5_Pin);
			return true;

		default:
			return false;


	}
}


int relay_state(int id){
	switch(id)
	{
		case 1:
			if(HAL_GPIO_ReadPin(RELAY1_GPIO_Port, RELAY1_Pin)== GPIO_PIN_SET) return 1;
			else return 0;

		case 2:
			if(HAL_GPIO_ReadPin(RELAY2_GPIO_Port, RELAY2_Pin)== GPIO_PIN_SET) return 1;
			else return 0;

		case 3:
			if(HAL_GPIO_ReadPin(RELAY3_GPIO_Port, RELAY3_Pin)== GPIO_PIN_SET) return 1;
			else return 0;

		case 4:
			if(HAL_GPIO_ReadPin(RELAY4_GPIO_Port, RELAY4_Pin)== GPIO_PIN_SET) return 1;
			else return 0;

		case 5:
			if(HAL_GPIO_ReadPin(RELAY5_GPIO_Port, RELAY5_Pin)== GPIO_PIN_SET) return 1;
			else return 0;

		default:
			return -1;


	}
}
