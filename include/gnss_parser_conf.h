 /*
 * gnss_parser_conf.h
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

#ifndef GNSS_PARSER_CONF_H_
#define GNSS_PARSER_CONF_H_

/** @brief                  Default initialization macro for a GNSS parser object.
 *  @param UART_Interface   UART interface for the GNSS receiver
 *  @param UART_Baudrate    Baudrate used by the GNSS receiver
 *  @param UART_Rx          Rx input for the GNSS receiver (MCU)
 */
#define GNSS_PARSER_DEFAULT_CONF(UART_Interface, UART_Baudrate, UART_Rx)        {                                           \
                                                                                    .UART                                   \
                                                                                    {                                       \
                                                                                        .Interface = UART_Interface,        \
                                                                                        .Rx = (gpio_num_t)UART_Rx,          \
                                                                                        .Baudrate = UART_Baudrate,          \
                                                                                    },                                      \
                                                                                    .Internal                               \
                                                                                    {                                       \
                                                                                        .MessageFilter = 0,                 \
                                                                                        .Handle = NULL,                     \
                                                                                        .MessageQueue = NULL,               \
                                                                                        .isInitialized = false,             \
                                                                                        .p_Buffer = NULL,                   \
                                                                                        .EventLoop_Handle = NULL,           \
                                                                                        .EventLoop_Instance = NULL          \
                                                                                    }                                       \
                                                                                }

#endif /* GNSS_PARSER_CONF_H_ */