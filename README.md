
# LoRa Ping-Pong for Silabs Family 2

## Introduction

This repository details all steps, files and information needed to generate a new Simplicity Studio v5 project for the Family 2 devices and add the LoRa Radio API to it and create a basic example of a Ping-Pong application.

## HW Requirements

The HW required for this is listed here:
-	Silabs development board, for example:
	-	EFM32PG22 Dev Kit - **BRD2503A**
	-	Thunder Board Sense 2 BG22 - **BRD4184A**
	-	Bluetooth radio board MG22 - **BR4182A** (requires base board **BRD4001A**)
	-	etc
-	Semtech mbed shield, for example:
	-	**SX1262MB2xAS**
	-	**SX1272MB2xAS**
	-	etc
-	Adaption boards or wire connections between the target MCU board and the mbed shield. We recommend the following interposer boards:
	-	Vermont adapter for Thunderboard > Adapts the pins from the Thunderboard to the EXT spacing
	-	Vermont Semtech Silabs LoRa Adapter - Easiest option is the version with configurable connections which can be used with any shield / MCU board by reprogramming the jumper 0 ohm resistors but the previous version with fixed connections (2018) can also be used for specific cases.

## Basic Steps

The steps required are:
1.  Create a basic project from the Launcher into Simplicity Studio v5 for the development board selected, using SDK3.1.2 or SDK4.0.1, such as:
	-	Bluetooth SoC Thermometer (default name: `soc_thermometer`) for BG22 or MG22
	-	Empty C Project (default name: `empty`) for PG22
	-	etc

2.  Add Software Components for specific peripheral / functions support for anything not originally included into the starting project above. Open `.slcp` file and review all the details.
	-	Into the folder [Example_Projects](https://github.com/VermontRep/LoRa_Ping-Pong_Silabs/tree/master/Example_Projects) there are ready projects but also instructions on which Components to add for such examples.

3.	Copy the following files and folders from the `To_Copy` folder:
```
port_ping_pong_board
vendor
ping_pong.c
ping_pong.h
```
4.	Add the following list of include directories for both C Compiler and Assembly:
```
"${workspace_loc:/${ProjName}/vendor/Ping_Pong-4.4.7/src/boards}"
"${workspace_loc:/${ProjName}/vendor/Ping_Pong-4.4.7/src/system}"
"${workspace_loc:/${ProjName}/vendor/Ping_Pong-4.4.7/src/radio}"
"${workspace_loc:/${ProjName}/port_ping_pong_board}"
```

5.	Add the following list of defines into the Preprocessor list for C Compiler:
```
PING_PONG_IS_MASTER=true (**or false**)
USE_MODEM_LORA=1
REGION_AU915=1
SX1262MBXCAS=1
BLUETOOTH_ID_PRESENT=1
```

-	**The first one PING_PONG_IS_MASTER=true shall be changed to each side - one must be true while the other is false (slave)**
-	**This last one BLUETOOTH_ID_PRESENT=1 shall only be added if the Bluetooth ID is present in the MCU (such as in BG22 or MG22 series)**

6.	Exclude from Build the unused files for the radio board support, by selecting the radio folder or only the `.c file`. The location of such folders is `vendor/LoRaMac-node-4.4.7/src/radio` and at this level there are the transceiver folders `SX126x`, `SX1272` and `SX1276`.
	Example: if using the **SX1262**, exclude from Build, the SX1272 and SX1276 support files / folders.
	
7.	Configure the mbed shield pins properly into the file `board-config.h` located into the folder `port_ping_pong_board` for your specific case. The pins can be modified by either commenting / uncommenting the define lines for the BRDxxxx pins into the EXT connector that lead to the LoRa transceiver signals in the mbed shield or by typing the proper pins in the case such defines aren´t already available.

8.	Modify the application file `app.c` (or maybe some other related file, such as `blink.c` when using Blink Baremetal as base project) to properly do the application processing of the LoRaWAN protocol coordinated with the original application tasks.
	-	There are example of such application files into the [Example_Projects](https://github.com/VermontRep/LoRa_Ping-Pong_Silabs/tree/master/Example_Projects) folder to help identify possible processing alternatives.

## Ready Example Projects

There are also ready to use example projects in the `.sls` format inside the folder [Example_Projects](https://github.com/VermontRep/LoRa_Ping-Pong_Silabs/tree/master/Example_Projects) to use through the Simplicity Studio v5 **Import** function.

## Code Inheritance

The code available here uses the concepts inherited from the following repositories:
-	[LoRaMac-node_Silabs](https://github.com/VermontRep/LoRaMac-node_Silabs)
-	[LoRaMac-node](https://github.com/Lora-net/LoRaMac-node).
