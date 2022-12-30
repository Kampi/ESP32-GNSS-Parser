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

#include "esp_err.h"

#include "gnss_parser_defs.h"
#include "gnss_parser_conf.h"

/** @brief Get a message filter for all message formats.
 */
#define GNSS_PARSER_MASK_ALL                        (GNSS_FORMAT_VTG | GNSS_FORMAT_GLL | GNSS_FORMAT_GSV | GNSS_FORMAT_RMC | GNSS_FORMAT_GSA | GNSS_FORMAT_GGA)

/** @brief              Initialize the GNSS receiver and the parser.
 *  @param p_Parser     Pointer to parser object
 *  @param FilterMask   Message filter mask
 *  @return             GNSS_ERR_OK when successful
 */
GNSS_Error_t GNSS_Parser_Init(GNSS_Parser_t& p_Parser, uint8_t FilterMask);

/** @brief          Deinitialize the GNSS receiver and the parser.
 *  @param p_Parser Pointer to parser object
 */
void GNSS_Parser_Deinit(GNSS_Parser_t& p_Parser);

/** @brief              Add user defined event handler for GNSS parser.
 *  @param p_Parser     Pointer to parser object
 *  @param EventHandler User defined event handler
 *  @param p_EventArgs  Pointer to handler specific arguments
 *  @return             GNSS_ERR_OK when successful
 */
GNSS_Error_t GNSS_Parser_AddEvent(GNSS_Parser_t& p_Parser, esp_event_handler_t EventHandler, void* p_EventArgs);

/** @brief              Remove user defined event handler for GNSS parser.
 *  @param p_Parser     Pointer to parser object
 *  @param EventHandler User defined event handler
 *  @return             GNSS_ERR_OK when successful
 */
GNSS_Error_t GNSS_Parser_RemoveEvent(GNSS_Parser_t& p_Parser, esp_event_handler_t EventHandler);

#endif /* GNSS_PARSER_H_ */