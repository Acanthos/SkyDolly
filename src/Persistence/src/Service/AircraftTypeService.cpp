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
#include <utility>
#include <vector>

#include <QSqlDatabase>

#include "../../../Model/src/AircraftType.h"
#include "../Dao/DaoFactory.h"
#include "../Dao/AircraftTypeDaoIntf.h"
#include "AircraftTypeService.h"

class AircraftTypeServicePrivate
{
public:
    AircraftTypeServicePrivate() noexcept
        : daoFactory(std::make_unique<DaoFactory>(DaoFactory::DbType::SQLite)),
          aircraftTypeDao(daoFactory->createAircraftTypeDao())
    {}

    std::unique_ptr<DaoFactory> daoFactory;
    std::unique_ptr<AircraftTypeDaoIntf> aircraftTypeDao;
};

// PUBLIC

AircraftTypeService::AircraftTypeService() noexcept
    : d(std::make_unique<AircraftTypeServicePrivate>())
{}

AircraftTypeService::~AircraftTypeService() noexcept
{}

bool AircraftTypeService::getByType(const QString &type, AircraftType &aircraftType) const noexcept
{
    bool ok = QSqlDatabase::database().transaction();
    if (ok) {
        ok = d->aircraftTypeDao->getByType(type, aircraftType);
        QSqlDatabase::database().rollback();
    }
    return ok;
}

bool AircraftTypeService::getAll(std::insert_iterator<std::vector<AircraftType>> insertIterator) const noexcept
{
    bool ok = QSqlDatabase::database().transaction();
    if (ok) {
        ok = d->aircraftTypeDao->getAll(insertIterator);
        QSqlDatabase::database().rollback();
    }
    return ok;
}