
# LoRa Ping-Pong for Silabs Family 2 - To Copy

## Introduction

The following folders have the **LoRa P2P** Radio API with the needed files to adapt to Silicon Labs projects and a P2P communication using a Ping-Pong application.

## Folders

Here the folders and the overview of their contents:
-	[port_ping_pong_board](https://github.com/VermontRep/LoRa_Ping-Pong_Silabs/To_Copy/port_ping_pong_board) - files that interface the Radio API and the Silicon Labs project files
-	[vendor](https://github.com/VermontRep/LoRa_Ping-Pong_Silabs/To_Copy/vendor) - only radio folders and files from the original [LoRaWAN stack repository](https://github.com/Lora-net/LoRaMac-node/tree/master/src), frozen to release 4.4.7.

## Files

The files:
-	ping_pong.c
-	ping_pong.h
implement a basic P2P protocol between 2 boards, one configured as Master and the other as Slave that talk to each other over LoRa messages.