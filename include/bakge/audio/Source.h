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
 * @file Source.h
 * @brief Source class declaration.
 */

#ifndef BAKGE_AUDIO_SOURCE_H
#define BAKGE_AUDIO_SOURCE_H

#include <bakge/Bakge.h>

namespace bakge
{

class Stream;

/*! @brief A Node-like object used to play Stream objects.
 *
 * Source is a Node-like object that is used to attach and play Stream
 * objects. It does not inherit Node, but implements similar functionality
 * in that you can move it around in 3D Cartesian space.
 */
class BGE_API Source
{

protected:

    ALuint SourceHandle;

    /*! @brief Default Source constructor.
     *
     * Default Source constructor.
     */
    Source();


public:

    /*! @brief Virtual Source destructor.
     *
     * Virtual Source destructor.
     */
    virtual ~Source();

    /*! @brief Create a new Source object to play an audio Stream from.
     *
     * Create a new Source object to play an audio Stream from.
     *
     * @return Pointer to allocated Source; NULL if any errors occurred.
     */
    BGE_FACTORY Source* Create();

    /*! @brief Play the Source's attached Stream.
     *
     * Play the Source's attached Stream.
     *
     * @return BGE_SUCCESS if the Source is successfully playing; BGE_FAILURE
     * if any errors occurred or no Stream is attached.
     */
    Result Play() const;

    /*! @brief Attach a Stream object to this Source, so it can be played.
     *
     * Attach a Stream object to this Source, so it can be played.
     *
     * @return BGE_SUCCESS if the Stream was successfully attached;
     * BGE_FAILURE if any errors occurred.
     */
    Result Attach(Stream* Str);

}; /* Source */

} /* bakge */

#endif /* BAKGE_AUDIO_SOURCE_H */
