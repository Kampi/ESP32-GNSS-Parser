 /*
 * gnss_parser_priv.cpp
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

#include "gnss_parser_defs.h"
#include "gnss_parser_priv_include.h"

float GNSS_Parser_ParseLatitudeLongitude(GNSS_RT_t* p_RunTime)
{
    float ll = strtof(p_RunTime->Item, NULL);
    int deg = ((int)ll) / 100;
    float min = ll - (deg * 100);
    ll = deg + min / 60.0f;

    return ll;
}

void GNSS_Parser_Parse_UTC_Time(GNSS_RT_t* p_RunTime)
{
    p_RunTime->Parent.Time.Hour = GNSS_Parser_ASCII2Digit(p_RunTime->Item + 0);
    p_RunTime->Parent.Time.Minute = GNSS_Parser_ASCII2Digit(p_RunTime->Item + 2);
    p_RunTime->Parent.Time.Second = GNSS_Parser_ASCII2Digit(p_RunTime->Item + 4);

    if(p_RunTime->Item[6] == '.')
    {
        uint8_t i;
        uint16_t Temp;

        Temp = 0;
        i = 7;

        while(p_RunTime->Item[i])
        {
            Temp = 10 * Temp + p_RunTime->Item[i] - '0';
            i++;
        }

        p_RunTime->Parent.Time.Thousand = Temp;
    }
}