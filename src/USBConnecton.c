/*
 * USBConnecton.c
 *
 *  Created on: Feb 19, 2022
 *      Author: kubex
 */

#include "USBConnection.h"
#include <string.h>

void Usb_Sendstring(char *data){

	CDC_Transmit_FS((uint8_t *)data, strlen(data));
}
