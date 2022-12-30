 /*
 * gnss_parser_defs.h
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

#ifndef GNSS_PARSER_ERRORS_H_
#define GNSS_PARSER_ERRORS_H_

#include <sdkconfig.h>

typedef int GNSS_Error_t;

/** @brief      Generic error check macro.
 *  @param Func Function that should be checked
 */
#define GNSS_ERROR_CHECK(Func)                                   do                                                                                                              \
                                                                {                                                                                                               \
                                                                    GNSS_Error_t Error = Func;                                                                              \
                                                                    if(Error != GNSS_ERR_OK)                                                                                \
                                                                    {                                                                                                           \
                                                                        ESP_LOGE("GNSS", "Error check failed in (%s) at line (%u): 0x%X!", __FUNCTION__, __LINE__, Error);  \
                                                                        return Error;                                                                                           \
                                                                    }                                                                                                           \
                                                                } while(0);

/** @brief GNSS parser error base.
 */
#define GNSS_BASE                                                CONFIG_GNSS_PARSER_MISC_ERROR_BASE

/** @brief Everything okay.
 */
#define GNSS_ERR_OK                                              (GNSS_BASE + 0)

/** @brief Generic error.
 */
#define GNSS_ERR_FAIL                                            (GNSS_BASE + 1)

/** @brief Invalid function parameter.
 */
#define GNSS_ERR_INVALID_ARG                                     (GNSS_BASE + 2)

/** @brief Parser not initialized.
 */
#define GNSS_ERR_NOT_INITIALIZED                                 (GNSS_BASE + 3)

#endif /* GNSS_PARSER_ERRORS_H_ */