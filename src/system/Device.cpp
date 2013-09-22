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

void GetPrimaryMonitorResolution(int* StoreX, int* StoreY)
{

    GLFWmonitor* PrimaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* MonitorVideoMode = glfwGetVideoMode(PrimaryMonitor);
    *(StoreX) = MonitorVideoMode->width;
    *(StoreY) = MonitorVideoMode->height;
}


void GetPrimaryMonitorPhysicalSize(int* StoreX, int* StoreY)
{

    GLFWmonitor* PrimaryMonitor = glfwGetPrimaryMonitor();
    glfwGetMonitorPhysicalSize(PrimaryMonitor, StoreX, StoreY);
}


int GetNumberMonitors()
{

    int* MonitorCount = NULL;
    glfwGetMonitors(MonitorCount);

    return *MonitorCount;
}

} /* bakge */
