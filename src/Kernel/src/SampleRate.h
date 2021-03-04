/**
 * Sky Dolly - The black sheep for your flight recordings
 *
 * Copyright (c) Oliver Knoll
 * All rights reserved.
 *
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
 * to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED *AS IS*, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#ifndef SAMPLERATE_H
#define SAMPLERATE_H

namespace SampleRate
{
    enum SampleRate {
        Hz1,
        Hz2,
        Hz5,
        Hz10,
        Hz15,
        Hz20,
        Hz24,
        Hz25,
        Hz30,
        Hz45,
        Hz50,
        Hz60
    };

    inline constexpr double toValue(SampleRate SampleRate) {
        switch (SampleRate) {
        case Hz1:
            return 1.0;
        case Hz2:
            return 2.0;
        case Hz5:
            return 5.0;
        case Hz10:
            return 10.0;
        case Hz15:
            return 15.0;
        case Hz20:
            return 20.0;
        case Hz24:
            return 24.0;
        case Hz25:
            return 25.0;
        case Hz30:
            return 30.0;
        case Hz45:
            return 45.0;
        case Hz50:
            return 50.0;
        case Hz60:
            return 60.0;
        default:
            return 30.0;
        }
    }

    inline constexpr SampleRate fromValue(double SampleRate) {
        if (SampleRate <= 1.0)
            return Hz1;
        else if (SampleRate <= 1.0)
            return Hz2;
        else if (SampleRate <= 5.0)
            return Hz5;
        else if (SampleRate <= 10.0)
            return Hz10;
        else if (SampleRate <= 15.0)
            return Hz15;
        else if (SampleRate <= 20.0)
            return Hz20;
        else if (SampleRate <= 24.0)
            return Hz24;
        else if (SampleRate <= 25.0)
            return Hz25;
        else if (SampleRate <= 30.0)
            return Hz30;
        else if (SampleRate <= 45.0)
            return Hz45;
        else if (SampleRate <= 50.0)
            return Hz50;
        else if (SampleRate <= 60.0)
            return Hz60;
        else
            return Hz30;
    }
}

#endif // SAMPLERATE_H