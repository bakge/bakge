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
 * @file Clock.h
 * @brief System clock API header.
 */

#ifndef BAKGE_SYSTEM_CLOCK_H
#define BAKGE_SYSTEM_CLOCK_H

#include <bakge/Bakge.h>

namespace bakge
{

/*! @brief Pause the calling thread for a given length of time.
 *
 * Pause the calling thread for a given length of time.
 *
 * @return BGE_SUCCESS if the delay was successful; BGE_FAILURE if any
 * errors occurred.
 */
BGE_FUNC Result Delay(Microseconds Time);

/*! @brief Get the time since bakge::Init was called.
 *
 * @brief Get the time since bakge::Init was called.
 *
 * @return Number of microseconds since bakge::Init was called.
 */
BGE_FUNC Microseconds GetRunningTime();

} /* bakge */

#endif /* BAKGE_SYSTEM_CLOCK_H */
