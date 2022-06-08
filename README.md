# Garden-irrigation-system
AI system to detect pedestrians using cell phones.

# Purpose of the project

The aim of the project was to create a controller based on STM32 microcontroller with wireless communication using ESP8266-01s wi-fi module. Communication with the wi-fi module is done using AT commands sent from the STM32 controller. The controller has five 10A/230V relays which can be used for example to control valves in the irrigation system. The system has outputs for communication using communication protocols such as: USART, SPI, ADC, I2C, USB. The device can be programmed using the SWD interface, for this purpose the ST-LINK/V2 programmer should be used. The device can be powered by 5V, using the USB input, or by 12V power supply with a power not less than 12W. The power supply should be connected to the screw terminal marked +12V. It is possible to connect USB and +12V power supply simultaneously.


# Schematic diagram, design and fabrication of printed circuit board.
The PCB design was created using KiCad, and the PCB itself was manufactured by JLCPCB.

![image](https://user-images.githubusercontent.com/39679208/172722525-ec354f2c-25cb-40b5-9c6d-37f369abe421.png)
![image](https://user-images.githubusercontent.com/39679208/172722790-d1720943-8272-4403-b8cf-291901139f01.png)
![image](https://user-images.githubusercontent.com/39679208/172722809-4e057f95-bd90-4697-8e33-384ab7f51704.png)
![image](https://user-images.githubusercontent.com/39679208/172722820-c28b2057-f82b-4321-bcd2-d58261b30b7d.png)

# Code logic
In order to efficiently communicate with the ESP8266 module, a cyclic buffer was used, which is filled by the DMA module, which significantly speeds up communication because the DMA module works in parallel with the controller and does not take up CPU time. To create responses cJSON module was used, which allows to create and process JSON type objects. There was also created a module that controls relays allowing for efficient control of devices connected to the controller and a module that creates server responses. All modules are used by the WIFI communication module, which using a created HTTP client allows for remote control of the controller.

In the future I plan to add modules to control devices connected to the UART, SPI, ADC and I2C outputs.

# Video presentation
[![VIDEO](https://user-images.githubusercontent.com/39679208/172725087-13d86de8-cf54-4c30-a5c7-df5a1a5d5b1d.png)](https://youtu.be/u0HHk7Kp1-Y)


# How to run this project
To run this project you should use the cube IDE, select the stm32f103c8t6 uC, compile it using the built-in compiler and upload it to the board using the ST-LINK/V2 programmer.

You can also compile it using CMakefile...

TODO: Add CMake file.


