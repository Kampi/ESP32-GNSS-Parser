 /*
 * gnss_parser_collection.h
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

#ifndef GNSS_PARSER_COLLECTION_H_
#define GNSS_PARSER_COLLECTION_H_

#include "gnss_parser_defs.h"
#include "../Private/gnss_parser_priv_include.h"

#include <sdkconfig.h>

#ifdef CONFIG_GNSS_PARSER_FORMAT_GGA
    /** @brief              Parse a GGA message.
     *  @param p_RunTime    Pointer to GNSS parser run time object
     */
    void GNSS_Parser_Parse_GGA(GNSS_RT_t* const p_RunTime);
#endif

#ifdef CONFIG_GNSS_PARSER_FORMAT_GSA
    /** @brief              Parse a GSA message.
     *  @param p_RunTime    Pointer to GNSS parser run time object
     */
    void GNSS_Parser_Parse_GSA(GNSS_RT_t* const p_RunTime);
#endif

#ifdef CONFIG_GNSS_PARSER_FORMAT_GSV
    /** @brief              Parse a GSV message.
     *  @param p_RunTime    Pointer to GNSS parser run time object
     */
    void GNSS_Parser_Parse_GSV(GNSS_RT_t* const p_RunTime);
#endif

#ifdef CONFIG_GNSS_PARSER_FORMAT_RMC
    /** @brief              Parse a RMC message.
     *  @param p_RunTime    Pointer to GNSS parser run time object
     */
    void GNSS_Parser_Parse_RMC(GNSS_RT_t* const p_RunTime);
#endif

#ifdef CONFIG_GNSS_PARSER_FORMAT_GLL
    /** @brief              Parse a GLL message.
     *  @param p_RunTime    Pointer to GNSS parser run time object
     */
    void GNSS_Parser_Parse_GLL(GNSS_RT_t* const p_RunTime);
#endif

#ifdef CONFIG_GNSS_PARSER_FORMAT_VTG
    /** @brief              Parse a VTG message.
     *  @param p_RunTime    Pointer to GNSS parser run time object
     */
    void GNSS_Parser_Parse_VTG(GNSS_RT_t* const p_RunTime);
#endif

#endif /* GNSS_PARSER_COLLECTION_H_ */