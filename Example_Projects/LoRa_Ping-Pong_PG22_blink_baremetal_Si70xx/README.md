
# LoRa Ping-Pong for Silabs Family 2 - Example Projects

## LoRa_Ping-Pong_PG22_blink_baremetal_Si70xx

Project File ready to use: `LoRa_Ping-Pong_PG22_blink_baremetal_Si70xx.sls`

Summary:
-	Silabs MCU Board: **PG22-DK2503A**
-	Semtech mbed shield: **SX1262MB2xAS**
-	Base Project: **Empty C Project**
-	Added Functions: *LoRa Radio API + Interrupts + Si70xx (RH&T sensor) + print functions*

## Files to Replace in the Basic Project when creating it

Replace the following file:
-	`app.c`

## Software Components to Add to Basic Project when creating it

To create a similar project, open the `.slcp` file, review the installed items and add the missing Software Components from the following lists.

-	To include only the LoRa Radio API and basic text functions:
```
	-	Platform > Peripheral > GPIO
	-	Platform > Driver > GPIOINT
	-	Application > Service > Simple timer service
	-	Application > Utility > Assert
	-	Application > Utility > Log
	-	Services > IOStream > IOStream
	-	Services > IOStream > IOStream: Retarget STDIO
	-	Services > IOStream > IOStream: USART > vcom
	-	Services > IOStream > IOStream: USART Core
```
-	To additionally include the Relative Humidity & Temperature sensor support:
```
	-	Third Party > Tiny printf
	-	Platform > Board Drivers > Si70xx - Temperature/Humidity Sensor
	-	Platform > Driver > I2CSPM > sensor
	-	Platform > Driver > I2CSPM Core
	-	Platform > Peripheral > I2C
	-	Application > Sensor > Relative Humidity and Temperature sensor
```
