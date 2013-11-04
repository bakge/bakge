/* *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013 Paul Holden et al. (See AUTHORS)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * */

/*!
 * @file Utility.h
 * @brief Internal utility code that is not exposed as end-user API.
 */

#ifndef BAKGE_INTERNAL_UTILITY_H
#define BAKGE_INTERNAL_UTILITY_H

#include <bakge/Bakge.h>

namespace bakge
{

/*! @brief Check for required OpenGL extensions.
 *
 * Ensures the extensions required to run Bakge are supported by the current
 * hardware. Bakge requires a certain set of extensions for its various
 * wrappers and classes to work properly. If any of these are unavailable
 * on the current hardware or driver configuration, Bakge will not work.
 *
 * @return true if all required extensions are supported; false if one or
 * more are unavailable.
 */
bool _CheckRequiredExtensions();

/*! @brief Platform-specific initialization.
 *
 * Platform-specific initialization.
 *
 * @return BGE_SUCCESS if initialization was successful; BGE_FAILURE if
 * any errors occurred.
 */
Result _PlatformInit(int, char*[]);

/*! @brief Platform-specific deinitialization.
 *
 * Platform-specific deinitialization.
 *
 * @return BGE_SUCCESS if deinitialization was successful; BGE_FAILURE if
 * any errors occurred.
 */
Result _PlatformDeinit();

} // bakge

#endif // BAKGE_INTERNAL_UTILITY_H
