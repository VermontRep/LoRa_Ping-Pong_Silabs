/*!
 * \file      ping_pong.c
 *
 * \brief     Ping-Pong implementation
 *
 * \author    Claudio Sonaglio ( Vermont )
 *
 */
#include <string.h>
#include "board.h"
#include "gpio.h"
#include "delay.h"
#include "timer.h"
#include "radio.h"
#include "app_log.h"
#include "spi-board.h"
#include "sx126x-board.h"
#include "board-config.h"
#include "ping_pong.h"

const uint8_t PingMsg[] = "PING";
const uint8_t PongMsg[] = "PONG";

uint16_t BufferSize = BUFFER_SIZE;
uint8_t Buffer[BUFFER_SIZE];

States_t State = LOWPOWER;

int8_t RssiValue = 0;
int8_t SnrValue = 0;

/*!
 * Radio events function pointer
 */
static RadioEvents_t RadioEvents;

/*!
 * \brief Function executed to do all Ping Pong initializations
 */
void ping_pong_init( void )
{
  SpiInit( &SX126x.Spi, SPI_1, RADIO_MOSI, RADIO_MISO, RADIO_SCLK, RADIO_NSS );
  SX126xIoInit( );

  // Radio initialization
  RadioEvents.TxDone = OnTxDone;
  RadioEvents.RxDone = OnRxDone;
  RadioEvents.TxTimeout = OnTxTimeout;
  RadioEvents.RxTimeout = OnRxTimeout;
  RadioEvents.RxError = OnRxError;

  Radio.Init( &RadioEvents );

  Radio.SetChannel( RF_FREQUENCY );

#if defined( USE_MODEM_LORA )

  Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                                 LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                                 LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                                 true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );

  Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                                 LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                                 LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                                 0, true, 0, 0, LORA_IQ_INVERSION_ON, true );

  Radio.SetMaxPayloadLength( MODEM_LORA, BUFFER_SIZE );

#elif defined( USE_MODEM_FSK )

  Radio.SetTxConfig( MODEM_FSK, TX_OUTPUT_POWER, FSK_FDEV, 0,
                                FSK_DATARATE, 0,
                                FSK_PREAMBLE_LENGTH, FSK_FIX_LENGTH_PAYLOAD_ON,
                                true, 0, 0, 0, 3000 );

  Radio.SetRxConfig( MODEM_FSK, FSK_BANDWIDTH, FSK_DATARATE,
                                0, FSK_AFC_BANDWIDTH, FSK_PREAMBLE_LENGTH,
                                0, FSK_FIX_LENGTH_PAYLOAD_ON, 0, true,
                                0, 0,false, true );

  Radio.SetMaxPayloadLength( MODEM_FSK, BUFFER_SIZE );

#else
  #error "Please define a frequency band in the compiler options."
#endif

  Radio.Rx( RX_TIMEOUT_VALUE );
}

/*!
 * \brief Function executed to do all Ping Pong loop functions
 */
void ping_pong_loop( void )
{
    static bool isMaster = PING_PONG_IS_MASTER;
    static uint8_t i;

    switch( State )
    {
    case RX:
        if( isMaster == true )
        {
            if( BufferSize > 0 )
            {
                if( strncmp( ( const char* )Buffer, ( const char* )PongMsg, 4 ) == 0 )
                {
                    // Indicates on a LED that the received frame is a PONG
                    //GpioToggle( &Led1 );

                    // Send the next PING frame
                    Buffer[0] = 'P';
                    Buffer[1] = 'I';
                    Buffer[2] = 'N';
                    Buffer[3] = 'G';
                    // We fill the buffer with numbers for the payload
                    for( i = 4; i < BufferSize; i++ )
                    {
                        Buffer[i] = i - 4;
                    }
                    DelayMs( PING_PONG_WAIT_PAUSE );
                    Radio.Send( Buffer, BufferSize );
                    app_log("Master received PONG, now sending PING\n");
                }
                else if( strncmp( ( const char* )Buffer, ( const char* )PingMsg, 4 ) == 0 )
                { // A master already exists then become a slave
                    isMaster = false;
                    //GpioToggle( &Led2 ); // Set LED off
                    Radio.Rx( RX_TIMEOUT_VALUE );
                }
                else // valid reception but neither a PING or a PONG message
                {    // Set device as master ans start again
                    isMaster = true;
                    Radio.Rx( RX_TIMEOUT_VALUE );
                }
            }
        }
        else
        {
            if( BufferSize > 0 )
            {
                if( strncmp( ( const char* )Buffer, ( const char* )PingMsg, 4 ) == 0 )
                {
                    // Indicates on a LED that the received frame is a PING
                    //GpioToggle( &Led1 );

                    // Send the reply to the PONG string
                    Buffer[0] = 'P';
                    Buffer[1] = 'O';
                    Buffer[2] = 'N';
                    Buffer[3] = 'G';
                    // We fill the buffer with numbers for the payload
                    for( i = 4; i < BufferSize; i++ )
                    {
                        Buffer[i] = i - 4;
                    }
                    DelayMs( PING_PONG_WAIT_PAUSE );
                    Radio.Send( Buffer, BufferSize );
                    app_log("Slave received PING, now sending PONG\n");
                }
                else // valid reception but not a PING as expected
                {    // Set device as master and start again
                    isMaster = true;
                    Radio.Rx( RX_TIMEOUT_VALUE );
                }
            }
        }
        State = LOWPOWER;
        break;
    case TX:
        // Indicates on a LED that we have sent a PING [Master]
        // Indicates on a LED that we have sent a PONG [Slave]
        //GpioToggle( &Led2 );
        Radio.Rx( RX_TIMEOUT_VALUE );
        State = LOWPOWER;
        break;
    case RX_TIMEOUT:
    case RX_ERROR:
        if( isMaster == true )
        {
            // Send the next PING frame
            Buffer[0] = 'P';
            Buffer[1] = 'I';
            Buffer[2] = 'N';
            Buffer[3] = 'G';
            for( i = 4; i < BufferSize; i++ )
            {
                Buffer[i] = i - 4;
            }
            DelayMs( PING_PONG_WAIT_PAUSE );
            Radio.Send( Buffer, BufferSize );
            app_log("Master sending start PING\n");
        }
        else
        {
            Radio.Rx( RX_TIMEOUT_VALUE );
        }
        State = LOWPOWER;
        break;
    case TX_TIMEOUT:
        Radio.Rx( RX_TIMEOUT_VALUE );
        State = LOWPOWER;
        break;
    case LOWPOWER:
    default:
        // Set low power
        break;
    }

    // BoardLowPowerHandler( );
    // Process Radio IRQ
    if( Radio.IrqProcess != NULL )
    {
        Radio.IrqProcess( );
    }
}

void OnTxDone( void )
{
    Radio.Sleep( );
    State = TX;
}

void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
    Radio.Sleep( );
    BufferSize = size;
    memcpy( Buffer, payload, BufferSize );
    RssiValue = rssi;
    SnrValue = snr;
    State = RX;
}

void OnTxTimeout( void )
{
    Radio.Sleep( );
    State = TX_TIMEOUT;
}

void OnRxTimeout( void )
{
    Radio.Sleep( );
    State = RX_TIMEOUT;
}

void OnRxError( void )
{
    Radio.Sleep( );
    State = RX_ERROR;
}
