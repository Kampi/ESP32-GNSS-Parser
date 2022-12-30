 /*
 * gnss_parser_gsa.cpp
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

#include "gnss_parser_collection.h"

#ifdef CONFIG_GNSS_PARSER_FORMAT_GSA
    void GNSS_Parser_Parse_GSA(GNSS_RT_t* const p_RunTime)
    {
        switch(p_RunTime->CurrentItemNumber)
        {
            // Process fix mode.
            case 2:
            {
                p_RunTime->Parent.FixMode = static_cast<GNSS_FixMode_t>(strtol(p_RunTime->Item, NULL, 10));

                break;
            }
            // Process PDOP.
            case 15:
            {
                p_RunTime->Parent.dop_p = strtof(p_RunTime->Item, NULL);

                break;
            }
            // Process HDOP.
            case 16:
            {
                p_RunTime->Parent.dop_h = strtof(p_RunTime->Item, NULL);

                break;
            }
            // Process VDOP.
            case 17:
            {
                p_RunTime->Parent.dop_v = strtof(p_RunTime->Item, NULL);

                break;
            }
            // Parse satellite IDs.
            default:
            {
                if((p_RunTime->CurrentItemNumber >= 3) && (p_RunTime->CurrentItemNumber <= 14))
                {
                    p_RunTime->Parent.SatsID[p_RunTime->CurrentItemNumber - 3] = static_cast<uint8_t>(strtol(p_RunTime->Item, NULL, 10));
                }

                break;
            }
        }
    }
#endif