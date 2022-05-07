/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include "em_common.h"
#include "app_log.h"
#include "app_assert.h"
#include "sl_status.h"
#include "sl_simple_timer.h"
#include "sl_sensor_rht.h"
#include "ping_pong.h"

#define PING_PONG_INTERVAL_SEC   1

// Periodic timer handle.
static sl_simple_timer_t ping_pong_periodic_timer;

// Periodic timer callback.
static void ping_pong_periodic_timer_cb(sl_simple_timer_t *timer, void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{

  sl_status_t sc;

  sl_sensor_rht_init();

  sc = sl_simple_timer_start(&ping_pong_periodic_timer,
                             PING_PONG_INTERVAL_SEC * 1000,
                             ping_pong_periodic_timer_cb,
                             NULL,
                             true);
  app_assert(sc == SL_STATUS_OK,
                "[E: 0x%04x] Failed to start periodic timer\n",
                (int)sc);

  // Send first indication.
  ping_pong_periodic_timer_cb(&ping_pong_periodic_timer, NULL);

  ping_pong_init();
  blink_init();
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{

  ping_pong_loop();
  blink_process_action();

}

/**************************************************************************//**
 * Timer callback
 * Called periodically to time periodic temperature measurements and indications.
 *****************************************************************************/
static void ping_pong_periodic_timer_cb(sl_simple_timer_t *timer, void *data)
{
  (void)data;
  (void)timer;
  sl_status_t sc;
  int32_t temperature = 0;
  uint32_t humidity = 0;

  // Measure temperature; units are % and milli-Celsius.
  sc = sl_sensor_rht_get(&humidity, &temperature);
  if (sc != SL_STATUS_OK) {
    app_log("Warning! Invalid RHT reading: %lu %ld\n", humidity, temperature);
  }

  app_log("Temperature / Humidity: %5.2f C / %5.2f RH\n", temperature/1000.0, humidity/1000.0);

}
