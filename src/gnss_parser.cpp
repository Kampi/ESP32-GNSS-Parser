 /*
 * gnss_parser_defs.cpp
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

#include <esp_log.h>

#include <string.h>

#include "gnss_parser.h"
#include "Parser/gnss_parser_collection.h"
#include "Private/gnss_parser_priv_include.h"

/** @brief UART configuration for the GNSS receiver.
 */
static uart_config_t _GNSS_UART_Config = {
    .baud_rate              = 9600,
    .data_bits              = UART_DATA_8_BITS,
    .parity                 = UART_PARITY_DISABLE,
    .stop_bits              = UART_STOP_BITS_1,
    .flow_ctrl              = UART_HW_FLOWCTRL_DISABLE,
    .rx_flow_ctrl_thresh    = 0,
#if(defined CONFIG_PM_ENABLE) && (defined CONFIG_IDF_TARGET_ESP32)
    #ifdef SOC_UART_SUPPORT_REF_TICK
        .source_clk         = UART_SCLK_REF_TICK,
    #else
        .source_clk         = UART_SCLK_RTC,
    #endif
#else
    .source_clk             = UART_SCLK_APB,
#endif
};

/**
 * @brief Define of NMEA Parser Event base
 *
 */
ESP_EVENT_DEFINE_BASE(ESP_NMEA_EVENT);

static GNSS_RT_t _GNSS_Parser_RunTime;

static const char* TAG = "GNSS-Parser";

/** @brief  Parse a received item.
 *  @return #true when successful
 */
static bool GNSS_Receiver_ParseItem(void)
{
    if((_GNSS_Parser_RunTime.CurrentItemNumber == 0) && (_GNSS_Parser_RunTime.Item[0] == '$'))
    {
        #ifdef CONFIG_NMEA_STATEMENT_GGA
            if(strstr(_GNSS_Parser_RunTime.Item, "GGA"))
            {
                GNSS_Parser_Parse_GGA(&_GNSS_Parser_RunTime);
                _GNSS_Parser_RunTime.CurrentFormat = GNSS_FORMAT_GGA;
            }
        #endif

        #ifdef CONFIG_GNSS_PARSER_FORMAT_GSA
            if(strstr(_GNSS_Parser_RunTime.Item, "GSA"))
            {
                GNSS_Parser_Parse_GSA(&_GNSS_Parser_RunTime);
                _GNSS_Parser_RunTime.CurrentFormat = GNSS_FORMAT_GSA;
            }
        #endif

        #ifdef CONFIG_GNSS_PARSER_FORMAT_GSV
            if(strstr(_GNSS_Parser_RunTime.Item, "GSV"))
            {
                GNSS_Parser_Parse_GSV(&_GNSS_Parser_RunTime);
                _GNSS_Parser_RunTime.CurrentFormat = GNSS_FORMAT_GSV;
            }
        #endif

        #ifdef CONFIG_GNSS_PARSER_FORMAT_RMC
            if(strstr(_GNSS_Parser_RunTime.Item, "RMC"))
            {
                GNSS_Parser_Parse_RMC(&_GNSS_Parser_RunTime);
                _GNSS_Parser_RunTime.CurrentFormat = GNSS_FORMAT_RMC;
            }
        #endif

        #ifdef CONFIG_GNSS_PARSER_FORMAT_GLL
            if(strstr(_GNSS_Parser_RunTime.Item, "GLL"))
            {
                GNSS_Parser_Parse_GLL(&_GNSS_Parser_RunTime);
                _GNSS_Parser_RunTime.CurrentFormat = GNSS_FORMAT_GLL;
            }
        #endif

        #ifdef CONFIG_GNSS_PARSER_FORMAT_VTG
            if(strstr(_GNSS_Parser_RunTime.Item, "VTG"))
            {
                GNSS_Parser_Parse_VTG(&_GNSS_Parser_RunTime);
                _GNSS_Parser_RunTime.CurrentFormat = GNSS_FORMAT_VTG;
            }
        #endif
        else
        {
            _GNSS_Parser_RunTime.CurrentFormat = GNSS_FORMAT_UNKNOWN;
        }

        return true;
    }
    else
    {
        return false;
    }
}

/** @brief          Parse NMEA statements from the GNSS receiver.
 *  @param p_Parser Pointer to GNSS parser object
 */
static void GNSS_Receiver_Decode(GNSS_Parser_t* p_Parser)
{
    const uint8_t* c = p_Parser->Internal.p_Buffer;

    while(*c)
    {
        // Each new line begins with a "$". Use this to reset the run time environment.
        if(*c == '$')
        {
            _GNSS_Parser_RunTime.isAsterisk = false;
            _GNSS_Parser_RunTime.CurrentItemNumber = 0;
            _GNSS_Parser_RunTime.CurrentItemPosition = 0;
            _GNSS_Parser_RunTime.CurrentFormat = GNSS_FORMAT_UNKNOWN;
            _GNSS_Parser_RunTime.CRC = 0;
            _GNSS_Parser_RunTime.SatCount = 0;
            _GNSS_Parser_RunTime.SatNumber = 0;
            _GNSS_Parser_RunTime.Item[_GNSS_Parser_RunTime.CurrentItemPosition++] = *c;
            _GNSS_Parser_RunTime.Item[_GNSS_Parser_RunTime.CurrentItemPosition] = '\0';
        }
        // Each item is seperated by a ",".
        else if(*c == ',')
        {
            GNSS_Receiver_ParseItem();

            _GNSS_Parser_RunTime.CRC ^= static_cast<uint8_t>(*c);
            _GNSS_Parser_RunTime.CurrentItemPosition = 0;
            _GNSS_Parser_RunTime.Item[0] = '\0';
            _GNSS_Parser_RunTime.CurrentItemNumber++;
        }
        // All characters before the asterisk are used by the CRC.
        else if(*c == '*')
        {
            GNSS_Receiver_ParseItem();

            _GNSS_Parser_RunTime.isAsterisk = true;
            _GNSS_Parser_RunTime.CurrentItemPosition = 0;
            _GNSS_Parser_RunTime.Item[0] = '\0';
            _GNSS_Parser_RunTime.CurrentItemNumber++;
        }
        // End of line reached. The line must be proccessed now.
        else if(*c == '\r')
        {
            uint8_t CRC;

            CRC = static_cast<uint8_t>(strtol(_GNSS_Parser_RunTime.Item, NULL, 16));

            if(true)
            {
                switch(_GNSS_Parser_RunTime.CurrentFormat)
                {
                    #ifdef CONFIG_GNSS_PARSER_FORMAT_GGA
                        case GNSS_FORMAT_GGA:
                        {
                            ESP_LOGI(TAG, "GGA parsed");
                            _GNSS_Parser_RunTime.MessageFormat |= 0x01 << GNSS_FORMAT_GGA;

                            break;
                        }
                    #endif
                    #ifdef CONFIG_GNSS_PARSER_FORMAT_GSA
                        case GNSS_FORMAT_GSA:
                        {
                            _GNSS_Parser_RunTime.MessageFormat |= 0x01 << GNSS_FORMAT_GSA;
                            break;
                        }
                    #endif
                    #ifdef CONFIG_GNSS_PARSER_FORMAT_RMC
                        case GNSS_FORMAT_RMC:
                        {
                            _GNSS_Parser_RunTime.MessageFormat |= 0x01 << GNSS_FORMAT_RMC;
                            break;
                        }
                    #endif
                    #ifdef CONFIG_GNSS_PARSER_FORMAT_GSV
                        case GNSS_FORMAT_GSV:
                        {
                            if(_GNSS_Parser_RunTime.SatNumber == _GNSS_Parser_RunTime.SatCount)
                            {
                                _GNSS_Parser_RunTime.MessageFormat |= 0x01 << GNSS_FORMAT_GSV;
                            }

                            break;
                        }
                    #endif
                    #ifdef CONFIG_GNSS_PARSER_FORMAT_GLL
                        case GNSS_FORMAT_GLL:
                        {
                            _GNSS_Parser_RunTime.MessageFormat |= 0x01 << GNSS_FORMAT_GLL;

                            break;
                        }
                    #endif
                    #ifdef CONFIG_NMEA_STATEMENT_VTG
                        case GNSS_FORMAT_VTG:
                        {
                            _GNSS_Parser_RunTime.MessageFormat |= 0x01 << GNSS_FORMAT_VTG;

                            break;
                        }
                    #endif
                    default:
                    {
                        break;
                    }
                }

                /* Check if all statements have been parsed */
                if(((_GNSS_Parser_RunTime.MessageFormat) & _GNSS_Parser_RunTime.AllStatements) == _GNSS_Parser_RunTime.AllStatements)
                {
                    _GNSS_Parser_RunTime.MessageFormat = GNSS_FORMAT_UNKNOWN;
                    /*
                    esp_event_post_to(_GNSS_Parser_RunTime.event_loop_hdl, ESP_NMEA_EVENT, GNSS_UPDATE,
                                      &(_GNSS_Parser_RunTime.Parent), sizeof(GNSS_t), 100 / portTICK_PERIOD_MS);*/
                }
            }
            else
            {
                ESP_LOGE(TAG, "CRC Error for statement:%s", p_Parser->Internal.p_Buffer);
            }

            if(_GNSS_Parser_RunTime.CurrentFormat == GNSS_FORMAT_UNKNOWN)
            {
                /* Send signal to notify that one unknown statement has been met */
                //esp_event_post_to(_GNSS_Parser_RunTime.event_loop_hdl, ESP_NMEA_EVENT, GNSS_UNKNOWN,
                //                  _GNSS_Parser_RunTime.buffer, len, 100 / portTICK_PERIOD_MS);
            }
        }
        // All other characters.
        else
        {
            // We have to add the current character to the checksum if an asterisk was not detected.
            if(_GNSS_Parser_RunTime.isAsterisk == false)
            {
                _GNSS_Parser_RunTime.CRC ^= static_cast<uint8_t>(*c);
            }

            _GNSS_Parser_RunTime.Item[_GNSS_Parser_RunTime.CurrentItemPosition++] = *c;
            _GNSS_Parser_RunTime.Item[_GNSS_Parser_RunTime.CurrentItemPosition] = '\0';
        }

        // Get the next character.
        c++;
    }
}

/** @brief          UART event queue task.
 *  @param p_Arg    Pointer to task argument
 */
static void GNSS_Parser_Event(void* p_Arg)
{
    uart_event_t Event;
    GNSS_Parser_t* Parser = (GNSS_Parser_t*)p_Arg;

    while(true)
    {
        if(xQueueReceive(Parser->Internal.MessageQueue, &Event, 200 / portTICK_PERIOD_MS))
        {
            switch(Event.type)
            {
                case UART_DATA:
                {
                    break;
                }
                case UART_FIFO_OVF:
                {
                    ESP_LOGW(TAG, "HW FIFO Overflow");

                    uart_flush(Parser->UART.Interface);
                    xQueueReset(Parser->Internal.MessageQueue);

                    break;
                }
                case UART_BUFFER_FULL:
                {
                    ESP_LOGW(TAG, "Ring Buffer Full");

                    uart_flush(Parser->UART.Interface);
                    xQueueReset(Parser->Internal.MessageQueue);

                    break;
                }
                case UART_BREAK:
                {
                    ESP_LOGW(TAG, "Rx Break");

                    break;
                }
                case UART_PARITY_ERR:
                {
                    ESP_LOGE(TAG, "Parity Error");

                    break;
                }
                case UART_FRAME_ERR:
                {
                    ESP_LOGE(TAG, "Frame Error");

                    break;
                }
                case UART_PATTERN_DET:
                {
                    int Position;
                    
                    Position = uart_pattern_pop_pos(Parser->UART.Interface);
                    if(Position != -1)
                    {
                        int BytesRead;

                        BytesRead = uart_read_bytes(Parser->UART.Interface, Parser->Internal.p_Buffer, Position, 100 / portTICK_PERIOD_MS);

                        Parser->Internal.p_Buffer[BytesRead] = '\0';

                        ESP_LOGD(TAG, "Buffer: %s", Parser->Internal.p_Buffer);

                        GNSS_Receiver_Decode(Parser);
                    }
                    else
                    {
                        ESP_LOGW(TAG, "Pattern Queue Size too small");

                        uart_flush_input(Parser->UART.Interface);
                    }

                    break;
                }
                default:
                {
                    ESP_LOGW(TAG, "Unknown uart event type: %d", Event.type);

                    break;
                }
            }
        }
    }

    vTaskDelete(NULL);
}

GNSS_Error_t GNSS_Parser_Init(GNSS_Parser_t& p_Parser)
{
    uint8_t Flags;

    esp_log_level_set("uart", ESP_LOG_NONE);

    _GNSS_UART_Config.baud_rate = p_Parser.UART.Baudrate;

    #ifdef CONFIG_GNSS_PARSER_FORMAT_GSA
        _GNSS_Parser_RunTime.AllStatements |= (0x01 << GNSS_FORMAT_GSA);
    #endif

    #ifdef CONFIG_GNSS_PARSER_FORMAT_GSV
        _GNSS_Parser_RunTime.AllStatements |= (0x01 << GNSS_FORMAT_GSV);
    #endif

    #ifdef CONFIG_GNSS_PARSER_FORMAT_GGA
        _GNSS_Parser_RunTime.AllStatements |= (0x01 << GNSS_FORMAT_GGA);
    #endif

    #ifdef CONFIG_GNSS_PARSER_FORMAT_RMC
        _GNSS_Parser_RunTime.AllStatements |= (0x01 << GNSS_FORMAT_RMC);
    #endif

    #ifdef CONFIG_GNSS_PARSER_FORMAT_GLL
        _GNSS_Parser_RunTime.AllStatements |= (0x01 << GNSS_FORMAT_GLL);
    #endif

    #ifdef CONFIG_GNSS_PARSER_FORMAT_VTG
        _GNSS_Parser_RunTime.AllStatements |= (0x01 << GNSS_FORMAT_VTG);
    #endif

    p_Parser.Internal.isInitialized = false;

    p_Parser.Internal.p_Buffer = (uint8_t*)malloc(CONFIG_GNSS_PARSER_UART_BUFFER_SIZE);
    if(p_Parser.Internal.p_Buffer == NULL)
    {
        return GNSS_ERR_FAIL;
    }

    #if CONFIG_GNSS_PARSER_UART_IRAM
        Flags = ESP_INTR_FLAG_IRAM;
    #else
        Flags = 0;
    #endif

    if(uart_driver_install(p_Parser.UART.Interface, CONFIG_GNSS_PARSER_UART_BUFFER_SIZE, 0, CONFIG_GNSS_PARSER_UART_QUEUE_LENGTH, &p_Parser.Internal.MessageQueue, Flags) != ESP_OK)
    {
        goto GNSS_Parser_Init_Install_Failed;
    }

    if((uart_param_config(p_Parser.UART.Interface, &_GNSS_UART_Config) != ESP_OK) ||
       (uart_set_pin(p_Parser.UART.Interface, UART_PIN_NO_CHANGE, p_Parser.UART.Rx, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE) != ESP_OK)
      )
    {
        goto GNSS_Parser_Init_Config_Failed;
    }

    uart_enable_pattern_det_baud_intr(p_Parser.UART.Interface, '\n', 1, 9, 0, 0);
    uart_pattern_queue_reset(p_Parser.UART.Interface, CONFIG_GNSS_PARSER_UART_QUEUE_LENGTH);
    uart_flush(p_Parser.UART.Interface);

    #ifdef CONFIG_RAK3172_TASK_CORE_AFFINITY
        xTaskCreatePinnedToCore(GNSS_Parser_Event, "GNSS-Parser", CONFIG_GNSS_PARSER_TASK_BUFFER_SIZE, p_Device, CONFIG_GNSS_PARSER_TASK_PRIO, &p_Parser.Internal.Handle, CONFIG_GNSS_PARSER_TASK_CORE);
    #else
        xTaskCreate(GNSS_Parser_Event, "GNSS-Parser", CONFIG_GNSS_PARSER_TASK_BUFFER_SIZE, &p_Parser, CONFIG_GNSS_PARSER_TASK_PRIO, &p_Parser.Internal.Handle);
    #endif

    if(p_Parser.Internal.Handle == NULL)
    {
        goto GNSS_Parser_Init_Config_Failed;
    }

    p_Parser.Internal.isInitialized = true;

    ESP_LOGI(TAG, "GNSS Parser initialized...");

    return GNSS_ERR_OK;

GNSS_Parser_Init_Config_Failed:
    uart_driver_delete(p_Parser.UART.Interface);

GNSS_Parser_Init_Install_Failed:
    free(p_Parser.Internal.p_Buffer);

    return GNSS_ERR_FAIL;
}

GNSS_Error_t GNSS_Parser_Deinit(GNSS_Parser_t& p_Parser)
{
    if(p_Parser.Internal.isInitialized)
    {
        return GNSS_ERR_OK;
    }

    vTaskSuspend(p_Parser.Internal.Handle);
    vTaskDelete(p_Parser.Internal.Handle);

    if(uart_is_driver_installed(p_Parser.UART.Interface))
    {
        uart_driver_delete(p_Parser.UART.Interface);
    }

    return GNSS_ERR_OK;
}

/**
 * @brief Add user defined handler for NMEA parser
 *
 * @param nmea_hdl handle of NMEA parser
 * @param event_handler user defined event handler
 * @param handler_args handler specific arguments
 * @return esp_err_t
 *  - ESP_OK: Success
 *  - ESP_ERR_NO_MEM: Cannot allocate memory for the handler
 *  - ESP_ERR_INVALIG_ARG: Invalid combination of event base and event id
 *  - Others: Fail
 */
esp_err_t nmea_parser_add_handler(nmea_parser_handle_t nmea_hdl, esp_event_handler_t event_handler, void *handler_args)
{
    /*
    GNSS_RT_t *esp_GNSS = (GNSS_RT_t *)nmea_hdl;
    return esp_event_handler_register_with(_GNSS_Parser_RunTime.event_loop_hdl, ESP_NMEA_EVENT, ESP_EVENT_ANY_ID,
                                           event_handler, handler_args);*/
                                return ESP_OK;
}

/**
 * @brief Remove user defined handler for NMEA parser
 *
 * @param nmea_hdl handle of NMEA parser
 * @param event_handler user defined event handler
 * @return esp_err_t
 *  - ESP_OK: Success
 *  - ESP_ERR_INVALIG_ARG: Invalid combination of event base and event id
 *  - Others: Fail
 */
esp_err_t nmea_parser_remove_handler(nmea_parser_handle_t nmea_hdl, esp_event_handler_t event_handler)
{/*
    GNSS_RT_t *esp_GNSS = (GNSS_RT_t *)nmea_hdl;
    return esp_event_handler_unregister_with(_GNSS_Parser_RunTime.event_loop_hdl, ESP_NMEA_EVENT, ESP_EVENT_ANY_ID, event_handler);*/
                                return ESP_OK;
}