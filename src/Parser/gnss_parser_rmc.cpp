 /*
 * gnss_parser_rmc.cpp
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

#ifdef CONFIG_GNSS_PARSER_FORMAT_RMC
    void GNSS_Parser_Parse_RMC(GNSS_RT_t* const p_RunTime)
    {
        switch(p_RunTime->CurrentItemNumber)
        {
            // Process UTC time.
            case 1:
            {
                GNSS_Parser_Parse_UTC_Time(p_RunTime);

                break;
            }
            // Process valid status.
            case 2:
            {
                p_RunTime->Parent.isValid = (p_RunTime->Item[0] == 'A');

                break;
            }
            // Process Latitude.
            case 3:
            {
                p_RunTime->Parent.Latitude = GNSS_Parser_ParseLatitudeLongitude(p_RunTime);

                break;
            }
            // Process Latitude north (1) / south (-1) information.
            case 4:
            {
                if((p_RunTime->Item[0] == 'S') || (p_RunTime->Item[0] == 's'))
                {
                    p_RunTime->Parent.Latitude *= -1;
                }

                break;
            }
            // Process Longitude.
            case 5:
            {
                p_RunTime->Parent.Longitude = GNSS_Parser_ParseLatitudeLongitude(p_RunTime);

                break;
            }
            // Process Longitude east (1) / west(-1) information.
            case 6:
            {
                if((p_RunTime->Item[0] == 'W') || (p_RunTime->Item[0] == 'w'))
                {
                    p_RunTime->Parent.Longitude *= -1;
                }

                break;
            }
            // Process ground speed.
            case 7:
            {
                p_RunTime->Parent.Speed = strtof(p_RunTime->Item, NULL) * 1.852;

                break;
            }
            // Process true course over ground.
            case 8:
            {
                p_RunTime->Parent.CoG = strtof(p_RunTime->Item, NULL);

                break;
            }
            // Process date.
            case 9:
            {
                p_RunTime->Parent.Date.Day = GNSS_Parser_ASCII2Digit(p_RunTime->Item + 0);
                p_RunTime->Parent.Date.Month = GNSS_Parser_ASCII2Digit(p_RunTime->Item + 2);
                p_RunTime->Parent.Date.Year = GNSS_Parser_ASCII2Digit(p_RunTime->Item + 4);

                break;
            }
            // Process magnetic variation.
            case 10:
            {
                p_RunTime->Parent.Variation = strtof(p_RunTime->Item, NULL);

                break;
            }
            default:
            {
                break;
            }
        }
    }
#endif