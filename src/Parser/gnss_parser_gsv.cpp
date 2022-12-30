 /*
 * gnss_parser_gsv.cpp
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

#ifdef CONFIG_GNSS_PARSER_FORMAT_GSV
    void GNSS_Parser_Parse_GSV(GNSS_RT_t* const p_RunTime)
    {
        switch(p_RunTime->CurrentItemNumber)
        {
            // Process total GSV numbers.
            case 1:
            {
                p_RunTime->SatCount = static_cast<uint8_t>(strtol(p_RunTime->Item, NULL, 10));

                break;
            }
            // Process current GSV statement number.
            case 2:
            {
                p_RunTime->SatNumber = static_cast<uint8_t>(strtol(p_RunTime->Item, NULL, 10));

                break;
            }
            // Process satellites in view.
            case 3:
            {
                p_RunTime->Parent.SatsInView = static_cast<uint8_t>(strtol(p_RunTime->Item, NULL, 10));

                break;
            }
            default:
            {
                if((p_RunTime->CurrentItemNumber >= 4) && (p_RunTime->CurrentItemNumber <= 19))
                {
                    uint8_t Index;
                    uint8_t CurrentItemNumber = p_RunTime->CurrentItemNumber - 4; /* Normalize item number from 4-19 to 0-15 */

                    Index = 4 * (p_RunTime->SatNumber - 1) + CurrentItemNumber / 4;

                    if(Index < GNSS_MAX_SATELLITES_IN_VIEW)
                    {
                        switch(CurrentItemNumber % 4)
                        {
                            case 0:
                            {
                                p_RunTime->Parent.SatDescriptions[Index].Number = static_cast<uint8_t>(strtol(p_RunTime->Item, NULL, 10));

                                break;
                            }
                            case 1:
                            {
                                p_RunTime->Parent.SatDescriptions[Index].Elevation = static_cast<uint8_t>(strtol(p_RunTime->Item, NULL, 10));

                                break;
                            }
                            case 2:
                            {
                                p_RunTime->Parent.SatDescriptions[Index].Azimuth = static_cast<uint16_t>(strtol(p_RunTime->Item, NULL, 10));

                                break;
                            }
                            case 3:
                            {
                                p_RunTime->Parent.SatDescriptions[Index].SNR = static_cast<uint8_t>(strtol(p_RunTime->Item, NULL, 10));

                                break;
                            }
                            default:
                            {
                                break;
                            }
                        }
                    }
                }
                break;
            }
        }
    }
#endif