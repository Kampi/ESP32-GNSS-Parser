 /*
 * gnss_parser_gga.cpp
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

#ifdef CONFIG_GNSS_PARSER_FORMAT_GGA
    void GNSS_Parser_Parse_GGA(GNSS_RT_t* const p_RunTime)
    {
        switch(p_RunTime->CurrentItemNumber)
        {
            // Process UTC time.
            case 1:
            {
                GNSS_Parser_Parse_UTC_Time(p_RunTime);

                break;
            }
            // Process Latitude.
            case 2:
            {
                p_RunTime->Parent.Latitude = GNSS_Parser_ParseLatitudeLongitude(p_RunTime);

                break;
            }
            // Process Latitude north (1) / south (-1) information.
            case 3:
            {
                if((p_RunTime->Item[0] == 'S') || (p_RunTime->Item[0] == 's'))
                {
                    p_RunTime->Parent.Latitude *= -1;
                }
                break;
            }
            // Process Longitude.
            case 4:
            {
                p_RunTime->Parent.Longitude = GNSS_Parser_ParseLatitudeLongitude(p_RunTime);
                break;
            }
            // Process Longitude east (1) / west (-1) information.
            case 5:
            {
                if((p_RunTime->Item[0] == 'W') || (p_RunTime->Item[0] == 'w'))
                {
                    p_RunTime->Parent.Longitude *= -1;
                }

                break;
            }
            // Process fix status.
            case 6:
            {
                p_RunTime->Parent.Fix = static_cast<GNSS_Fix_t>(strtol(p_RunTime->Item, NULL, 10));

                break;
            }
            // Process satellites in use.
            case 7:
            {
                p_RunTime->Parent.Satellites = static_cast<uint8_t>(strtol(p_RunTime->Item, NULL, 10));

                break;
            }
            // Process HDOP.
            case 8:
            {
                p_RunTime->Parent.DoP_H = strtof(p_RunTime->Item, NULL);

                break;
            }
            // Process Altitude.
            case 9:
            {
                p_RunTime->Parent.Altitude = strtof(p_RunTime->Item, NULL);

                break;
            }
            // Process Altitude above ellipsoid.
            case 11:
            {
                p_RunTime->Parent.Altitude += strtof(p_RunTime->Item, NULL);

                break;
            }
            default:
            {
                break;
            }
        }
    }
#endif