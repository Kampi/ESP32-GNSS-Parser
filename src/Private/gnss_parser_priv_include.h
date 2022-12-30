 /*
 * gnss_parser_priv_include.h
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

#ifndef GNSS_PARSER_PRIV_INCLUDE_H_
#define GNSS_PARSER_PRIV_INCLUDE_H_

#include <stdint.h>
#include <stdbool.h>

#include "gnss_parser_defs.h"

/** @brief GNSS parser library run time structure.
 */
typedef struct
{
    uint8_t CurrentItemPosition;                    /**< Current position in item. */
    uint8_t CurrentItemNumber;                      /**< Current item number. */
    bool isAsterisk;                                /**< Asterisk detected flag. */
    uint8_t CRC;                                    /**< Calculated CRC value. */
    uint8_t ReceivedMessages;                       /**< Received messages mask. */
    uint8_t SatNumber;                              /**< Satellite number. */
    uint8_t SatCount;                               /**< Satellite count. */
    GNSS_Format_t CurrentFormat;                    /**< Current message format ID. */
    char Item[16];                                  /**< Current item. */
    GNSS_Data_t Parent;                             /**< Parent class. */
} GNSS_RT_t;

/** @brief          Converter two continuous numeric character into a number.
 *  @param p_ASCII  Pointer to numeric character
 *  @return         Number
 */
inline __attribute__((always_inline)) uint8_t GNSS_Parser_ASCII2Digit(const char* p_ASCII)
{
    return 10 * (p_ASCII[0] - '0') + (p_ASCII[1] - '0');
}

/** @brief              Parse latitude or longitude format of latitude in NMEA is ddmm.sss and longitude is dddmm.sss.
 *  @param p_RunTime    Pointer to run time object
 *  @return             Latitude or Longitude value (degree)
 */
float GNSS_Parser_ParseLatitudeLongitude(GNSS_RT_t* p_RunTime);

/** @brief              Parse UTC time in GNSS statements
 *  @param p_RunTime    Pointer to run time object
 */
void GNSS_Parser_Parse_UTC_Time(GNSS_RT_t* p_RunTime);

#endif /* GNSS_PARSER_PRIV_INCLUDE_H_ */