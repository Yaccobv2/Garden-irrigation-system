/*
 * relayControl.h
 *
 *  Created on: Feb 21, 2022
 *      Author: kubex
 */

#ifndef INC_RELAYCONTROL_H_
#define INC_RELAYCONTROL_H_

#include <stdbool.h>
#include "stm32f1xx_hal.h"


void relay_gpio_controller(bool run,GPIO_TypeDef* actuator_port, GPIO_TypeDef* actuator_pin);
bool relay_controller(int id, bool state);
int relay_state(int id);

#endif /* INC_RELAYCONTROL_H_ */
