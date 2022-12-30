 /*
 * gnss_parser.h
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

#ifndef GNSS_PARSER_H_
#define GNSS_PARSER_H_

#include "esp_types.h"
#include "esp_event.h"
#include "esp_err.h"

#include "gnss_parser_defs.h"

/**
 * @brief Declare of NMEA Parser Event base
 *
 */
ESP_EVENT_DECLARE_BASE(ESP_NMEA_EVENT);

/**
 * @brief NMEA Parser Handle
 *
 */
typedef void *nmea_parser_handle_t;

/**
 * @brief NMEA Parser Event ID
 *
 */
typedef enum {
    GNSS_UPDATE, /*!< GNSS information has been updated */
    GNSS_UNKNOWN /*!< Unknown statements detected */
} nmea_event_id_t;

/** @brief          Initialize the GNSS receiver and the parser.
 *  @param p_Parser Pointer to parser object
 *  @return         GNSS_ERR_OK when successful
 */
GNSS_Error_t GNSS_Parser_Init(GNSS_Parser_t& p_Parser);

/** @brief          Deinitialize the GNSS receiver and the parser.
 *  @param p_Parser Pointer to parser object
 *  @return         GNSS_ERR_OK when successful
 */
GNSS_Error_t GNSS_Parser_Deinit(GNSS_Parser_t& p_Parser);

/**
 * @brief Add user defined handler for NMEA parser
 *
 * @param nmea_hdl handle of NMEA parser
 * @param event_handler user defined event handler
 * @param handler_args handler specific arguments
 * @return esp_err_t
 *  - ESP_OK: Success
 *  - ESP_ERR_NO_MEM: Cannot allocate memory for the handler
 *  - ESP_ERR_INVALIG_ARG: Invalid combination of event base and event id
 *  - Others: Fail
 */
esp_err_t nmea_parser_add_handler(nmea_parser_handle_t nmea_hdl, esp_event_handler_t event_handler, void *handler_args);

/**
 * @brief Remove user defined handler for NMEA parser
 *
 * @param nmea_hdl handle of NMEA parser
 * @param event_handler user defined event handler
 * @return esp_err_t
 *  - ESP_OK: Success
 *  - ESP_ERR_INVALIG_ARG: Invalid combination of event base and event id
 *  - Others: Fail
 */
esp_err_t nmea_parser_remove_handler(nmea_parser_handle_t nmea_hdl, esp_event_handler_t event_handler);

#endif /* GNSS_PARSER_H_ */