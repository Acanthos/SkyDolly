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

#include <QObject>
#include <QString>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QSqlRecord>
#include <QDateTime>
#include <QTimeZone>

#include "../../../../Model/src/FlightPlan.h"
#include "../../../../Model/src/FlightPlanData.h"
#include "../../ConnectionManager.h"
#include "SQLiteFlightPlanDao.h"

class SQLiteFlightPlanDaoPrivate
{
public:
    SQLiteFlightPlanDaoPrivate() noexcept
    {}

    std::unique_ptr<QSqlQuery> insertQuery;
    std::unique_ptr<QSqlQuery> selectByAircraftIdQuery;
    std::unique_ptr<QSqlQuery> deleteByFlightIdQuery;

    void initQueries()
    {
        if (insertQuery == nullptr) {
            insertQuery = std::make_unique<QSqlQuery>();
            insertQuery->prepare(
"insert into flight_plan ("
"  aircraft_id,"
"  timestamp,"
"  ident,"
"  latitude,"
"  longitude,"
"  altitude,"
"  local_sim_time,"
"  zulu_sim_time"
") values ("
" :aircraft_id,"
" :timestamp,"
" :ident,"
" :latitude,"
" :longitude,"
" :altitude,"
" :local_sim_time,"
" :zulu_sim_time"
");");
        }
        if (selectByAircraftIdQuery == nullptr) {
            selectByAircraftIdQuery = std::make_unique<QSqlQuery>();
            selectByAircraftIdQuery->prepare(
"select * "
"from   flight_plan fp "
"where  fp.aircraft_id = :aircraft_id "
"order by fp.timestamp asc;");
        }
        if (deleteByFlightIdQuery == nullptr) {
            deleteByFlightIdQuery = std::make_unique<QSqlQuery>();
            deleteByFlightIdQuery->prepare(
"delete "
"from   flight_plan "
"where  aircraft_id in (select a.id "
"                       from aircraft a"
"                       where a.flight_id = :flight_id"
"                      );");
        }
    }

    void resetQueries() noexcept
    {
        insertQuery = nullptr;
        selectByAircraftIdQuery = nullptr;
        deleteByFlightIdQuery = nullptr;
    }
};

// PUBLIC

SQLiteFlightPlanDao::SQLiteFlightPlanDao(QObject *parent) noexcept
    : QObject(parent),
      d(std::make_unique<SQLiteFlightPlanDaoPrivate>())
{
    frenchConnection();
}

SQLiteFlightPlanDao::~SQLiteFlightPlanDao() noexcept
{}

bool SQLiteFlightPlanDao::add(qint64 aircraftId, const QVector<FlightPlanData> &flightPlanData)  noexcept
{
    d->initQueries();
    d->insertQuery->bindValue(":aircraft_id", aircraftId);
    bool ok = true;
    for (const FlightPlanData &data : flightPlanData) {
        d->insertQuery->bindValue(":timestamp", data.timestamp);
        d->insertQuery->bindValue(":ident", data.waypointIdentifier);
        d->insertQuery->bindValue(":latitude", data.latitude);
        d->insertQuery->bindValue(":longitude", data.longitude);
        d->insertQuery->bindValue(":altitude", data.altitude);
        // No conversion to UTC
        d->insertQuery->bindValue(":local_sim_time", data.localTime);
        // Zulu time equals to UTC time
        d->insertQuery->bindValue(":zulu_sim_time", data.zuluTime);

        ok = d->insertQuery->exec();
        if (!ok) {
#ifdef DEBUG
            qDebug("SQLiteFlightPlanDao::add: SQL error: %s", qPrintable(d->insertQuery->lastError().databaseText() + " - error code: " + d->insertQuery->lastError().nativeErrorCode()));
#endif
            break;
        }
    }
    return ok;
}

bool SQLiteFlightPlanDao::getByAircraftId(qint64 aircraftId, FlightPlan &flightPlan) const noexcept
{
    d->initQueries();
    d->selectByAircraftIdQuery->bindValue(":aircraft_id", aircraftId);
    bool ok = d->selectByAircraftIdQuery->exec();
    if (ok) {
        flightPlan.clear();
        const int identifierIdx = d->selectByAircraftIdQuery->record().indexOf("ident");
        const int latitudeIdx = d->selectByAircraftIdQuery->record().indexOf("latitude");
        const int longitudeIdx = d->selectByAircraftIdQuery->record().indexOf("longitude");
        const int altitudeIdx = d->selectByAircraftIdQuery->record().indexOf("altitude");
        const int localSimulationTimeIdx = d->selectByAircraftIdQuery->record().indexOf("local_sim_time");
        const int zuluSimulationTimeIdx = d->selectByAircraftIdQuery->record().indexOf("zulu_sim_time");
        while (d->selectByAircraftIdQuery->next()) {
            FlightPlanData data;
            data.waypointIdentifier = d->selectByAircraftIdQuery->value(identifierIdx).toString();
            data.latitude = d->selectByAircraftIdQuery->value(latitudeIdx).toFloat();
            data.longitude = d->selectByAircraftIdQuery->value(longitudeIdx).toFloat();
            data.altitude = d->selectByAircraftIdQuery->value(altitudeIdx).toFloat();
            // Persisted time is already local simulation time
            data.localTime = d->selectByAircraftIdQuery->value(localSimulationTimeIdx).toDateTime();
            // UTC equals zulu time, so no conversion necessary
            data.zuluTime = d->selectByAircraftIdQuery->value(zuluSimulationTimeIdx).toDateTime();
            flightPlan.add(data);
        }
#ifdef DEBUG
    } else {
        qDebug("SQLiteFlightPlanDao::getByAircraftId: SQL error: %s", qPrintable(d->selectByAircraftIdQuery->lastError().databaseText() + " - error code: " + d->selectByAircraftIdQuery->lastError().nativeErrorCode()));
#endif
    }

    return ok;
}

bool SQLiteFlightPlanDao::deleteByFlightId(qint64 flightId) noexcept
{
    d->initQueries();
    d->deleteByFlightIdQuery->bindValue(":flight_id", flightId);
    bool ok = d->deleteByFlightIdQuery->exec();
#ifdef DEBUG
    if (!ok) {
        qDebug("SQLiteFlightPlanDao::deleteByFlightId: SQL error: %s", qPrintable(d->deleteByFlightIdQuery->lastError().databaseText() + " - error code: " + d->deleteByFlightIdQuery->lastError().nativeErrorCode()));
    }
#endif
    return ok;
}

// PRIVATE

void SQLiteFlightPlanDao::frenchConnection() noexcept
{
    connect(&ConnectionManager::getInstance(), &ConnectionManager::connectionChanged,
            this, &SQLiteFlightPlanDao::handleConnectionChanged);
}

// PRIVATE SLOTS

void SQLiteFlightPlanDao::handleConnectionChanged() noexcept
{
    d->resetQueries();
}
