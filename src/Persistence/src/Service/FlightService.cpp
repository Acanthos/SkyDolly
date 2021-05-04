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
#include <QSqlDatabase>

#include <memory>
#include <utility>

#include "../../../Model/src/Flight.h"
#include "../../../Model/src/Aircraft.h"
#include "../Dao/DaoFactory.h"
#include "../Dao/FlightDaoIntf.h"
#include "../Dao/AircraftDaoIntf.h"
#include "FlightService.h"

class FlightServicePrivate
{
public:
    FlightServicePrivate() noexcept
        : daoFactory(std::make_unique<DaoFactory>(DaoFactory::DbType::SQLite)),
          flightDao(daoFactory->createFlightDao())
    {}

    std::unique_ptr<DaoFactory> daoFactory;
    std::unique_ptr<FlightDaoIntf> flightDao;
};

// PUBLIC

FlightService::FlightService() noexcept
    : d(std::make_unique<FlightServicePrivate>())
{}

FlightService::~FlightService() noexcept
{
#ifdef DEBUG
    qDebug("FlightService::~FlightService: DESTROYED.");
#endif
}

bool FlightService::store(Flight &flight) noexcept
{
    bool ok = QSqlDatabase::database().transaction();
    if (ok) {
        d->flightDao->addFlight(flight);
        if (ok) {
            QSqlDatabase::database().commit();
            emit flightStored(flight.getId());
        } else {
            QSqlDatabase::database().rollback();
        }
    }
    return ok;
}

bool FlightService::restore(qint64 id, Flight &flight) noexcept
{
    bool ok = QSqlDatabase::database().transaction();
    if (ok) {
        ok = d->flightDao->getFlightById(id, flight);
        if (ok) {
            emit flightRestored(flight.getId());
        }
    }
    QSqlDatabase::database().rollback();
    return ok;
}

bool FlightService::deleteById(qint64 id)  noexcept
{
    bool ok = QSqlDatabase::database().transaction();
    if (ok) {
        ok = d->flightDao->deleteById(id);
        if (ok) {
            QSqlDatabase::database().commit();
        } else {
            QSqlDatabase::database().rollback();
        }
    }
    return ok;
}

QVector<FlightDescription> FlightService::getFlightDescriptions() const noexcept
{
    QVector<FlightDescription> descriptions;
    if (QSqlDatabase::database().transaction()) {
        descriptions = d->flightDao->getFlightDescriptions();
        QSqlDatabase::database().rollback();
    }
    return descriptions;
}