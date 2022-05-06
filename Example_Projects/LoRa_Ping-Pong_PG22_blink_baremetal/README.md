
# LoRa Ping-Pong for Silabs Family 2 - Example Projects

## LoRa_Ping_Pong_PG22_blink_baremetal

Project File ready to use: `LoRa_Ping_Pong_PG22_blink_baremetal.sls`

Summary:
-	Silabs MCU Board: **PG22-DK2503A**
-	Semtech mbed shield: **SX1262MB2xAS**
-	Base Project: **Empty C Project**
-	Added Functions: *LoRa Radio API + Interrupts + print functions*

## Files to Replace in the Basic Project when creating it

Replace the following file:
-	`app.c`

## Software Components to Add to Basic Project when creating it

To create a similar project, open the `.slcp` file, review the installed items and add the missing Software Components from the following lists.

-	To include only the LoRa Radio API and basic text functions:
```
	-	Platform > Peripheral > GPIO
	-	Platform > Driver > GPIOINT
	-	Application > Services > Simple timer service
	-	Application > Utility > Assert
	-	Application > Utility > Log
	-	Services > IOStream > IOStream
	-	Services > IOStream > IOStream: Retarget STDIO
	-	Services > IOStream > IOStream: USART > vcom
	-	Services > IOStream > IOStream: USART Core
```
