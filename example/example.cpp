 /*
 * example.cpp
 *
 *  Copyright (C) Daniel Kampert, 2022
 *	Website: www.kampis-elektroecke.de
 *  File info: GNSS parser example.
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

#include <esp_log.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>

#include <stdio.h>
#include <stdint.h>

#include "gnss_parser.h"

GNSS_Parser_t Parser;

static const char* TAG 							= "main";

static void GPS_EventHandler(void* p_EventHandler_Arg, esp_event_base_t EventBase, int32_t EventID, void* p_EventData)
{
    GNSS_Data_t* Data;

    switch(EventID)
    {
        case GNSS_EVENT_UPDATE:
        {
            Data = (GNSS_Data_t*)p_EventData;

            ESP_LOGI(TAG, "%d/%d/%d %d:%d:%d => \r\n"
                    "\t\t\t\t\t\tLatitude   = %.05f°N\r\n"
                    "\t\t\t\t\t\tLongitude = %.05f°E\r\n"
                    "\t\t\t\t\t\tAltitude   = %.02fm\r\n"
                    "\t\t\t\t\t\tSpeed      = %fm/s",
                    Data->Date.Year + 2000, Data->Date.Month, Data->Date.Day,
                    Data->Time.Hour + 1, Data->Time.Minute, Data->Time.Second,
                    Data->Latitude, Data->Longitude, Data->Altitude, Data->Speed);

            break;
        }
        case GNSS_EVENT_UNKNOWN:
        {
            ESP_LOGW(TAG, "Unknown statement:%s", (char*)p_EventData);

            break;
        }
        default:
        {
            break;
        }
    }
}

extern "C" void app_main(void)
{
    GNSS_Error_t Error;

    Parser = GNSS_PARSER_DEFAULT_CONF(CONFIG_GNSS_PARSER_UART_INTERFACE, CONFIG_GNSS_PARSER_UART_BAUD, CONFIG_GNSS_PARSER_UART_RX);

    esp_event_loop_create_default();

    Error = GNSS_Parser_Init(Parser, (GNSS_FORMAT_GSV | GNSS_FORMAT_RMC | GNSS_FORMAT_GSA | GNSS_FORMAT_GGA));
    if(Error == GNSS_ERR_OK)
    {
        Error = GNSS_Parser_AddEvent(Parser, GPS_EventHandler, NULL);
        if(Error == GNSS_ERR_OK)
        {
            vTaskDelay(60000 / portTICK_PERIOD_MS);

            GNSS_Parser_RemoveEvent(Parser, GPS_EventHandler);
            GNSS_Parser_Deinit(Parser);
        }
        else
        {
            ESP_LOGE(TAG, "Can not add event handler for GNSS parser. Error: 0x%X", Error);
        }

    }
    else
    {
        ESP_LOGE(TAG, "Can not initialize GNSS parser. Error: 0x%X", Error);
    }

    while(true)
    {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}