/*
 * ESP8266_HAL.c
 *
 *  Created on: Apr 14, 2020
 *      Author: Controllerstech
 */


#include <UartRingbuffer.h>
#include <USBConnection.h>
#include <ESP8266_HAL.h>
#include <cJSON.h>
#include <networkOutputHandler.h>
#include <relayControl.h>
#include "stdio.h"
#include "string.h"


//extern UART_HandleTypeDef huart1;

//#define wifi_uart &huart1


char buffer[20];


char *Basic_inclusion = "<!DOCTYPE html> <html>\n<head><meta name=\"viewport\"\
		content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n\
		<title>LED CONTROL</title>\n<style>html { font-family: Helvetica; \
		display: inline-block; margin: 0px auto; text-align: center;}\n\
		body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\
		h3 {color: #444444;margin-bottom: 50px;}\n.button {display: block;\
		width: 80px;background-color: #1abc9c;border: none;color: white;\
		padding: 13px 30px;text-decoration: none;font-size: 25px;\
		margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n\
		.button-on {background-color: #1abc9c;}\n.button-on:active \
		{background-color: #16a085;}\n.button-off {background-color: #34495e;}\n\
		.button-off:active {background-color: #2c3e50;}\np {font-size: 14px;color: #888;margin-bottom: 10px;}\n\
		</style>\n</head>\n<body>\n<h1>ESP8266 LED CONTROL</h1>\n";

char *LED_ON = "<p>LED Status: ON</p><a class=\"button button-off\" href=\"/ledoff\">OFF</a>";
char *LED_OFF = "<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/ledon\">ON</a>";
char *Relay6_ON = "<p>RELAY 6 Status: ON</p><a class=\"button button-off\" href=\"//relay1off\">OFF</a>";
char *Relay6_OFF = "<p>RELAY 6  Status: OFF</p><a class=\"button button-on\" href=\"//relay1on\">ON</a>";
char *Terminate = "</body></html>";

char *LED_JSON = "{\"ledStatus\":0}";
char *AUTH_JSON = "{\"AUTHOTIZATION_STATUS\":\"WRONG CREDENTIALS\"}";

char *USER_LOGIN = "admin";
char *USER_PASSWORD = "admin";


/*****************************************************************************************************************************************/

void ESP_Init (char *SSID, char *PASSWD)
{
	HAL_Delay(5000);

	char data[80];

	Ringbuf_init();

	Uart_sendstring("AT+RST\r\n");

	//Uart_sendstring("RESETTING.", pc_uart);
	Usb_Sendstring("RESETTING.\n");

	for (int i=0; i<5; i++)
	{
		//Uart_sendstring(".", pc_uart);
		Usb_Sendstring(".");
		HAL_Delay(1000);
	}

	/********* AT **********/
	Uart_sendstring("AT\r\n");
	while(!(Wait_for("AT\r\r\n\r\nOK\r\n")));
	//Uart_sendstring("AT---->OK\n\n", pc_uart);
	Usb_Sendstring("AT---->OK\n\n");


	/********* AT+CWMODE=1 **********/
	Uart_sendstring("AT+CWMODE=1\r\n");
	while (!(Wait_for("AT+CWMODE=1\r\r\n\r\nOK\r\n")));
	//Uart_sendstring("CW MODE---->1\n\n", pc_uart);
	Usb_Sendstring("CW MODE---->1\n\n");


	/********* AT+CWJAP="SSID","PASSWD" **********/
	//Uart_sendstring("connecting... to the provided AP\n", pc_uart);
	Usb_Sendstring("connecting... to the provided AP\n");
	sprintf (data, "AT+CWJAP=\"%s\",\"%s\"\r\n", SSID, PASSWD);
	Uart_sendstring(data);
	while (!(Wait_for("WIFI GOT IP\r\n\r\nOK\r\n")));
	sprintf (data, "Connected to,\"%s\"\n\n", SSID);
	Uart_sendstring(data);


	/********* AT+CIFSR **********/
	Uart_sendstring("AT+CIFSR\r\n");
	while (!(Wait_for("CIFSR:STAIP,\"")));
	while (!(Copy_upto("\"",buffer)));
	while (!(Wait_for("OK\r\n")));
	int len = strlen (buffer);
	buffer[len-1] = '\0';
	sprintf (data, "IP ADDR: %s\n\n", buffer);
	//Uart_sendstring(data, pc_uart);
	Usb_Sendstring(data);


	Uart_sendstring("AT+CIPMUX=1\r\n");
	while (!(Wait_for("AT+CIPMUX=1\r\r\n\r\nOK\r\n")));
	//Uart_sendstring("CIPMUX---->OK\n\n", pc_uart);
	Usb_Sendstring("CIPMUX---->OK\n\n");

	Uart_sendstring("AT+CIPSERVER=1,80\r\n");
	while (!(Wait_for("OK\r\n")));
	//Uart_sendstring("CIPSERVER---->OK\n\n", pc_uart);
	Usb_Sendstring("CIPSERVER---->OK\n\n");

	//Uart_sendstring("Now Connect to the IP ADRESS\n\n", pc_uart);
	Usb_Sendstring("Now Connect to the IP ADRESS\n\n");

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 1);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 0);
}




int Server_Send (char *str, int Link_ID)
{
	int len = strlen (str);
	char data[80];
	sprintf (data, "AT+CIPSEND=%d,%d\r\n", Link_ID, len);
	Uart_sendstring(data);
	while (!(Wait_for(">")));
	Uart_sendstring (str);
	while (!(Wait_for("SEND OK")));
	sprintf (data, "AT+CIPCLOSE=5\r\n");
	Uart_sendstring(data);
	while (!(Wait_for("OK\r\n")));
	return 1;
}

void Server_Handle (char *str, int Link_ID)
{
	cJSON *json = cJSON_CreateObject();
	char datatosend[1024] = {0};
	if (!(strcmp (str, "auth_error")))
	{
		add_string_object_to_json(json, "AUTHOTIZATION_STATUS","WRONG CREDENTIALS");
		add_string_object_to_json(json, "ACTION","FAILED");
		sprintf(datatosend, get_message(json));
		Server_Send(datatosend, Link_ID);
	}
	else if (!(strcmp (str, "ledon")))
	{

		add_string_object_to_json(json, "AUTHOTIZATION_STATUS","OK");
		add_string_object_to_json(json, "ACTION","SUCCESS");
		add_string_object_to_json(json, "ledStatus","1");
		sprintf(datatosend, get_message(json));
//		sprintf (datatosend, LED_JSON);
//		strcat(datatosend, LED_ON);
//		strcat(datatosend, Terminate);
		Server_Send(datatosend, Link_ID);
	}

	else if (!(strcmp (str, "ledoff")))
	{
//		sprintf (datatosend, Basic_inclusion);
//		strcat(datatosend, LED_OFF);
//		strcat(datatosend, Terminate);
		add_string_object_to_json(json, "AUTHOTIZATION_STATUS","OK");
		add_string_object_to_json(json, "ACTION","SUCCESS");
		add_string_object_to_json(json, "ledStatus","0");
		sprintf(datatosend, get_message(json));
		Server_Send(datatosend, Link_ID);
	}

	else if (!(strcmp (str, "relayON")))
	{
		add_string_object_to_json(json, "AUTHOTIZATION_STATUS","OK");
		add_string_object_to_json(json, "ACTION","SUCCESS");
		sprintf(datatosend, get_message(json));
		Server_Send(datatosend, Link_ID);
	}

	else if (!(strcmp (str, "relayOFF")))
	{
		add_string_object_to_json(json, "AUTHOTIZATION_STATUS","OK");
		add_string_object_to_json(json, "ACTION","SUCCESS");
		sprintf(datatosend, get_message(json));
		Server_Send(datatosend, Link_ID);
	}

	else if (!(strcmp (str, "argument_error")))
	{
		add_string_object_to_json(json, "AUTHOTIZATION_STATUS","OK");
		add_string_object_to_json(json, "ACTION","FAILED");
		add_string_object_to_json(json, "ERROR","WRONG QUERYSTRING ARGUMENT");
		sprintf(datatosend, get_message(json));
		Server_Send(datatosend, Link_ID);
	}

	else if (!(strcmp (str, "relay_error")))
	{
		add_string_object_to_json(json, "AUTHOTIZATION_STATUS","OK");
		add_string_object_to_json(json, "ACTION","FAILED");
		add_string_object_to_json(json, "ERROR","WRONG RELAY NUMBER");
		sprintf(datatosend, get_message(json));
		Server_Send(datatosend, Link_ID);
	}

	else if (!(strcmp (str, "relays_status")))
	{
		add_string_object_to_json(json, "AUTHOTIZATION_STATUS","OK");
		char relayName[6] = {0};
		char idString[1];
		for(int i=1; i<6;i++){

			int state = relay_state(i);
			itoa(i,idString,10);
			sprintf(relayName, "RELAY");
			strcat(relayName, idString);

			if(state != -1) cJSON_AddNumberToObject(json, relayName, state);
			else add_string_object_to_json(json, relayName,"NONE");
		}

		sprintf(datatosend, get_message(json));
		Server_Send(datatosend, Link_ID);
	}

	else
	{
		sprintf (datatosend, Basic_inclusion);
		strcat(datatosend, LED_OFF);
//		strcat(datatosend, Relay6_OFF);
		strcat(datatosend, Terminate);
		Server_Send(datatosend, Link_ID);
	}

}

void Server_Start (void)
{
	char buftocopyinto[64] = {0};
	char Link_ID;
	while (!(Get_after("+IPD,", 1, &Link_ID)));
	Link_ID -= 48;
	while (!(Copy_upto(" HTTP/1.1", buftocopyinto)));

	if(Look_for("/favicon.ico", buftocopyinto) == 1){}

	else if(Request_Auth(buftocopyinto)){
		if (Look_for("/ledon", buftocopyinto) == 1)
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 1);
			Server_Handle("ledon",Link_ID);
		}

		else if (Look_for("/ledoff", buftocopyinto) == 1)
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 0);
			Server_Handle("ledoff",Link_ID);
		}
		else if (Look_for("/relayON", buftocopyinto) == 1)
		{
			int ID = get_relay_number(buftocopyinto);
			if(ID != -1){
				if(relay_controller(ID,true)) Server_Handle("relayON",Link_ID);
				else Server_Handle("relay_error",Link_ID);
			}
			else Server_Handle("argument_error",Link_ID);

		}
		else if (Look_for("/relayOFF", buftocopyinto) == 1)
		{
			int ID = get_relay_number(buftocopyinto);
			if(ID != -1){
				if(relay_controller(ID,false)) Server_Handle("relayON",Link_ID);
				else Server_Handle("relay_error",Link_ID);
			}
			else Server_Handle("argument_error",Link_ID);

		}
		else if (Look_for("/relaysStatus", buftocopyinto) == 1)
		{
			Server_Handle("relays_status",Link_ID);

		}
		else
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 0);
			Server_Handle("/ ", Link_ID);
		}
	}
	else{
		Server_Handle("auth_error", Link_ID);
	}
}

bool Request_Auth( char *inputbuffer){
	char logindata[64] = {0};
	sprintf(logindata,"login=");
	if (Look_for("login=admin", inputbuffer) == 1)
		{

		sprintf(logindata,"password=");
		if (Look_for("password=admin", inputbuffer) == 1)
				{
					return true;
				}
		return false;
		}
	return false;

}

int get_relay_number(char *inputbuffer){
	char inputdata[64] = {0};

	sprintf(inputdata,"relay=");
	if (Look_for(inputdata, inputbuffer) == 1){

		char Relay_ID;
		Get_after(inputdata, 1, &Relay_ID);
		return (int)(Relay_ID - '0');
	}

	return -1;
}

