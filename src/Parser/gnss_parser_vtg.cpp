 /*
 * gnss_parser_vtg.cpp
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

#ifdef CONFIG_GNSS_PARSER_FORMAT_VTG
    void GNSS_Parser_Parse_VTG(GNSS_RT_t* const p_RunTime)
    {
        switch(p_RunTime->CurrentItemNumber)
        {
            // Process true course over ground.
            case 1:
            {
                p_RunTime->Parent.CoG = strtof(p_RunTime->Item, NULL);

                break;
            }
            // Process magnetic variation.
            case 3:
            {
                p_RunTime->Parent.Variation = strtof(p_RunTime->Item, NULL);

                break;
            }
            // Process ground speed.
            case 5:
            {
                p_RunTime->Parent.Speed = strtof(p_RunTime->Item, NULL) * 1.852;

                break;
            }
            // Process ground speed in unit m/s.
            case 7:
            {
                p_RunTime->Parent.Speed = strtof(p_RunTime->Item, NULL) / 3.6;

                break;
            }
            default:
            {
                break;
            }
        }
    }
#endif