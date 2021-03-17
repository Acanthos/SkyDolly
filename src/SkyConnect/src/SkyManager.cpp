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
#include <memory>

#ifdef WIN32
#include "SkyConnectImpl.h"
#else
#include "SkyConnectDummy.h"
#endif
#include "SkyManager.h"

class SkyManagerPrivate
{
public:

    // TODO Plugin system
#ifdef WIN32
    SkyManagerPrivate()
        : currentSkyConnect(std::make_unique<SkyConnectImpl>())
    {
    }
#else
    SkyManagerPrivate()
        : currentSkyConnect(std::make_unique<SkyConnectDummy>())
    {
    }
#endif

    ~SkyManagerPrivate()
    {
    }

    std::unique_ptr<SkyConnectIntf> currentSkyConnect;

    static SkyManager *instance;
};

SkyManager *SkyManagerPrivate::instance = nullptr;

// PUBLIC

SkyManager &SkyManager::getInstance() noexcept
{
    if (SkyManagerPrivate::instance == nullptr) {
        SkyManagerPrivate::instance = new SkyManager();
    }
    return *SkyManagerPrivate::instance;
}

void SkyManager::destroyInstance() noexcept
{
    if (SkyManagerPrivate::instance != nullptr) {
        delete SkyManagerPrivate::instance;
        SkyManagerPrivate::instance = nullptr;
    }
}

SkyConnectIntf &SkyManager::currentSkyConnect() const noexcept
{
    return *d->currentSkyConnect;
}

// PROTECTED

SkyManager::~SkyManager() noexcept
{
}

// PRIVATE

SkyManager::SkyManager() noexcept
    : d(std::make_unique<SkyManagerPrivate>())
{
}
