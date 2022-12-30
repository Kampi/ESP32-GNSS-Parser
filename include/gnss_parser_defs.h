 /*
 * gnss_parser_defs.h
 *
 *  Copyright (C) Daniel Kampert, 2022
 *	Website: www.kampis-elektroecke.de
 *  File info: GNSS receiver and parser for ESP32.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * Errors and commissions should be reported to DanielKampert@kampis-elektroecke.de
 */

#ifndef GNSS_PARSER_DEFS_H_
#define GNSS_PARSER_DEFS_H_

#include <string>
#include <stdint.h>

#include <esp_event.h>
#include <esp_event_base.h>

#include <driver/gpio.h>
#include <driver/uart.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include <freertos/queue.h>

#include "gnss_parser_errors.h"

#include <sdkconfig.h>

/** @brief 
 */
#define GNSS_MAX_SATELLITES_IN_VIEW                                 16

/** @brief 
 */
#define GNSS_MAX_SATELLITES_IN_USE                                  12

/** @brief GNSS parser event definitions.
 */
typedef enum 
{
    GNSS_EVENT_UPDATE,                                              /**< GPS information has been updated. */
    GNSS_EVENT_UNKNOWN                                              /**< Unknown statements detected. */
} GNSS_Event_t;

/** @brief GNSS message format definitions.
 */
typedef enum
{
    GNSS_FORMAT_UNKNOWN = 0,                                        /**< Unknown message format. */
    GNSS_FORMAT_GGA = (0x01 << 0x00),                               /**< GGA format. */
    GNSS_FORMAT_GSA = (0x01 << 0x01),                               /**< GSA format. */
    GNSS_FORMAT_RMC = (0x01 << 0x02),                               /**< RMC format. */
    GNSS_FORMAT_GSV = (0x01 << 0x03),                               /**< GSV format. */
    GNSS_FORMAT_GLL = (0x01 << 0x04),                               /**< GLL format. */
    GNSS_FORMAT_VTG = (0x01 << 0x05)                               /**< VTG format. */
} GNSS_Format_t;

/** @brief GNSS fix type definitions.
 */
typedef enum
{
    GNSS_FIX_INVALID,                                               /**< Not fixed. */
    GNSS_FIX_GPS,                                                   /**< GPS. */
    GNSS_FIX_DGPS,                                                  /**< Differential GPS. */
} GNSS_Fix_t;

/** @brief GNSS fix mode definitions.
 */
typedef enum
{
    GNSS_MODE_INVALID = 1,                                          /**< Not fixed. */
    GNSS_MODE_2D,                                                   /**< 2D GNSS. */
    GNSS_MODE_3D                                                    /**< 3D GNSS. */
} GNSS_FixMode_t;

/** @brief GNSS satellite information object definition.
 */
typedef struct
{
    uint8_t Number;                                                 /**< Satellite number. */
    uint8_t Elevation;                                              /**< Satellite elevation. */
    uint16_t Azimuth;                                               /**< Satellite azimuth. */
    uint8_t SNR;                                                    /**< Satellite signal noise ratio. */
} GNSS_Satellite_t;

/** @brief GNSS time object definition.
 */
typedef struct
{
    uint8_t Hour;                                                   /**< Hour. */
    uint8_t Minute;                                                 /**< Minute. */
    uint8_t Second;                                                 /**< Second. */
    uint16_t Thousand;                                              /**< Thousand seconds. */
} GNSS_Time_t;

/** @brief GNSS date object definition.
 */
typedef struct
{
    uint8_t Day;                                                    /**< Day (start from 1). */
    uint8_t Month;                                                  /**< Month (start from 1). */
    uint16_t Year;                                                  /**< Year (start from 2000). */
} GNSS_Date_t;

/** @brief GNSS data object definition.
 */
typedef struct
{
    float Latitude;                                                 /**< Latitude (degrees). */
    float Longitude;                                                /**< Longitude (degrees). */
    float Altitude;                                                 /**< Altitude (meters). */
    GNSS_Fix_t Fix;                                                 /**< Fix status. */
    uint8_t Satellites;                                             /**< Number of satellites in use. */
    GNSS_Time_t Time;                                               /**< Time in UTC. */
    GNSS_Date_t Date;                                               /**< Fix date. */
    GNSS_FixMode_t FixMode;                                         /**< Fix mode. */
    uint8_t SatsID[GNSS_MAX_SATELLITES_IN_USE];                     /**< ID list of all satellites in use. */
    float dop_h;                                                    /**< Horizontal dilution of precision. */
    float dop_p;                                                    /**< Position dilution of precision.  */
    float dop_v;                                                    /**< Vertical dilution of precision.  */
    uint8_t SatsInView;                                             /**< Number of satellites in view. */
    GNSS_Satellite_t SatDescriptions[GNSS_MAX_SATELLITES_IN_VIEW];  /**< Information of satellites in view. */
    bool Valid;                                                     /**< GNSS validity. */
    float Speed;                                                    /**< Ground speed (m/s). */
    float CoG;                                                      /**< Course over ground. */
    float Variation;                                                /**< Magnetic variation. */
} GNSS_Data_t;

/** @brief GNSS parser device object definition.
 */
typedef struct
{
    struct
    {
        uart_port_t Interface;                                      /**< Serial interface used by the device GNSS receiver driver. */
        gpio_num_t Rx;                                              /**< Rx pin number (MCU). */
        uint32_t Baudrate;                                          /**< Baud rate used by the GNSS module. */
    } UART;
    struct
    {
        uint8_t MessageFilter;                                      /**< Message filter mask.
                                                                         NOTE: Managed by the driver. */
        TaskHandle_t Handle;                                        /**< Handle for the UART receive task.
                                                                         NOTE: Managed by the driver. */
        QueueHandle_t MessageQueue;                                 /**< Module Rx message queue used by the receiving task.
                                                                         NOTE: Managed by the driver. */
        bool isInitialized;                                         /**< #true when the device driver is initialized.
                                                                         NOTE: Managed by the driver. */
        uint8_t* p_Buffer;                                          /**< Run time buffer for the UART event loop.
                                                                         NOTE: Managed by the driver. */
        esp_event_loop_handle_t EventLoop_Handle;                   /**< Event loop handle.
                                                                         NOTE: Managed by the driver. */
        esp_event_handler_instance_t EventLoop_Instance;            /**< Event loop handle.
                                                                         NOTE: Managed by the driver. */
    } Internal;
} GNSS_Parser_t;

/** @brief Declaration of the event base for the GNSS parser.
 */
ESP_EVENT_DECLARE_BASE(GNSS_PARSER_EVENT);

#endif /* GNSS_PARSER_DEFS_H_ */