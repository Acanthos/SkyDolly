/**
 * Sky Dolly - The Black Sheep for Your Flight Recordings
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
#include <QSettings>
#include <QByteArray>

#include <Version.h>
#include "SettingsConverter.h"

namespace
{
    void convertToV13([[maybe_unused]] const Version &settingsVersion, QSettings &settings) noexcept
    {
        settings.beginGroup("Window");
        {
            settings.setValue("LocationTableState", QVariant());
        }
        settings.endGroup();
    }

    void convertToV16([[maybe_unused]] const Version &settingsVersion, QSettings &settings) noexcept
    {
        static constexpr const char *AddToFlightEnabledKey = "AddToFlightEnabled";
        static constexpr const char *ImportDirectoryKey = "ImportDirectoryEnabled";
        convertToV13(settingsVersion, settings);

        // CSV import
        settings.beginGroup("Plugins/077448de-4909-4c5e-8957-2347afee6708");
        {
            const bool addToFlight = settings.value(AddToFlightEnabledKey).toBool();
            const bool importDirectory = settings.value(ImportDirectoryKey).toBool();
            if (addToFlight) {
                // Add to current flight
                settings.setValue("AircraftImportMode", 0);
            } else {
                // Add to separate flights / new flight
                settings.setValue("AircraftImportMode", importDirectory ? 2 : 1);
            }
        }
        settings.endGroup();
        // GPX import
        settings.beginGroup("Plugins/13f44df3-1df6-4458-ad29-71f7b185bf3e");
        {
            const bool addToFlight = settings.value(AddToFlightEnabledKey).toBool();
            const bool importDirectory = settings.value(ImportDirectoryKey).toBool();
            if (addToFlight) {
                // Add to current flight
                settings.setValue("AircraftImportMode", 0);
            } else {
                // Add to separate flights / new flight
                settings.setValue("AircraftImportMode", importDirectory ? 2 : 1);
            }
        }
        settings.endGroup();
        // IGC import
        settings.beginGroup("Plugins/a1902cf0-82a8-47ed-86ee-5a15152697c7");
        {
            const bool addToFlight = settings.value(AddToFlightEnabledKey).toBool();
            const bool importDirectory = settings.value(ImportDirectoryKey).toBool();
            if (addToFlight) {
                // Add to current flight
                settings.setValue("AircraftImportMode", 0);
            } else {
                // Add to separate flights / new flight
                settings.setValue("AircraftImportMode", importDirectory ? 2 : 1);
            }
        }
        settings.endGroup();
        // KML import
        settings.beginGroup("Plugins/5a72c866-310d-4d84-8bd6-1baa720bc64e");
        {
            const bool addToFlight = settings.value(AddToFlightEnabledKey).toBool();
            const bool importDirectory = settings.value(ImportDirectoryKey).toBool();
            if (addToFlight) {
                // Add to current flight
                settings.setValue("AircraftImportMode", 0);
            } else {
                // Add to separate flights / new flight
                settings.setValue("AircraftImportMode", importDirectory ? 2 : 1);
            }
        }
        settings.endGroup();
    }
}

// PUBLIC

void SettingsConverter::convertToCurrent(const Version &settingsVersion, QSettings &settings) noexcept
{
    const Version currentVersion;
    if (settingsVersion < currentVersion) {
        ::convertToV16(settingsVersion, settings);
    }
}
