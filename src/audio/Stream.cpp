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

#include <bakge/Bakge.h>

namespace bakge
{

Stream::Stream()
{
    StreamBuffer = 0;
    Vorbis = NULL;
}


Stream::~Stream()
{
    if(StreamBuffer != 0)
        alDeleteBuffers(1, &StreamBuffer);

    if(Vorbis != NULL)
        stb_vorbis_close(Vorbis);
}


Stream* Stream::Create(int Len, Byte* Data)
{
    Stream* S = (Stream*)calloc(1, sizeof(Stream));
    if(S == NULL) {
        Log("ERROR: Stream - Couldn't allocate memory\n");
        return NULL;
    }

    new(S) Stream;

    alGetError(); // Clear error state
    alGenBuffers(1, &S->StreamBuffer);
    if(alGetError() != AL_NO_ERROR) {
        Log("ERROR: Stream - Error creating buffer\n");
        delete S;
        return NULL;
    }

    int Channels;
    short* StreamData;
    int NumPackets = stb_vorbis_decode_memory(Data, Len, &Channels,
                                                        &StreamData);
    if(NumPackets < 0) {
        Log("ERROR: Stream - Error decoding data\n");
        delete S;
        return NULL;
    }

    // Use Channels to check error
    S->Vorbis = stb_vorbis_open_memory(Data, Len, &Channels, NULL);
    if(S->Vorbis == NULL) {
        Log("ERROR: Stream - Error opening vorbis stream\n");
        delete S;
        return NULL;
    }

    S->Info = stb_vorbis_get_info(S->Vorbis);

    alBufferData(S->StreamBuffer, Channels == 1 ? AL_FORMAT_MONO16 :
                                    AL_FORMAT_STEREO16, StreamData,
                                    NumPackets / 2, S->GetFrequency());
    if(alGetError() != AL_NO_ERROR) {
        Log("ERROR: Stream - Error setting buffer data\n");
        delete S;
        return NULL;
    }

    return S;
}

} /* bakge */
