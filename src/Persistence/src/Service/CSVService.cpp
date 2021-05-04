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

#include <QFile>
#include <QFileInfo>
#include <QDateTime>

#include "../../../Model/src/Logbook.h"
#include "../../../Model/src/Flight.h"
#include "../../../Model/src/Aircraft.h"
#include "../../../Model/src/AircraftInfo.h"
#include "../Import/CSVImport.h"
#include "../Export/CSVExport.h"
#include "FlightService.h"
#include "CSVService.h"

class CSVServicePrivate
{
public:
    CSVServicePrivate(FlightService &theFlightService) noexcept
        : flightService(theFlightService)
    {}

    FlightService &flightService;
};

// PUBLIC

CSVService::CSVService(FlightService &flightService) noexcept
    : d(std::make_unique<CSVServicePrivate>(flightService))
{}

CSVService::~CSVService() noexcept
{}

bool CSVService::importAircraft(const QString &filePath) noexcept
{
    QFile file(filePath);
    Flight &flight = Logbook::getInstance().getCurrentFlight();
    flight.clear();
    Aircraft &aircraft = flight.getUserAircraft();
    bool ok = CSVImport::importData(file, aircraft);
    if (ok) {
        AircraftInfo info;
        info.startDate = QFileInfo(filePath).birthTime();
        info.endDate = info.startDate.addMSecs(aircraft.getDurationMSec());
        aircraft.setAircraftInfo(info);
        d->flightService.store(Logbook::getInstance().getCurrentFlight());
    }
    return ok;
}

bool CSVService::exportAircraft(const Aircraft &aircraft, const QString &filePath) noexcept
{
    QFile file(filePath);
    return CSVExport::exportData(aircraft, file);
}