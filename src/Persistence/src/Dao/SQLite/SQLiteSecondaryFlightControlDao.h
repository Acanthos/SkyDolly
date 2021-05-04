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
#ifndef SQLITESECONDARYFLIGHTCONTROLDAO_H
#define SQLITESECONDARYFLIGHTCONTROLDAO_H

#include <memory>

#include <QObject>
#include <QVector>

#include "../../../../Model/src/SecondaryFlightControlData.h"
#include "../SecondaryFlightControlDaoIntf.h"

class SQLiteSecondaryFlightControlDaoPrivate;

class SQLiteSecondaryFlightControlDao : public QObject, public SecondaryFlightControlDaoIntf
{
public:
    explicit SQLiteSecondaryFlightControlDao(QObject *parent = nullptr) noexcept;
    virtual ~SQLiteSecondaryFlightControlDao() noexcept;

    virtual bool add(qint64 aircraftId, const SecondaryFlightControlData &data) noexcept override;
    virtual bool getByAircraftId(qint64 aircraftId, QVector<SecondaryFlightControlData> &data) const noexcept override;
    virtual bool deleteByFlightId(qint64 flightId) noexcept override;

private:
    std::unique_ptr<SQLiteSecondaryFlightControlDaoPrivate> d;

    void frenchConnection() noexcept;

private slots:
    void handleConnectionChanged() noexcept;
};

#endif // SQLITESECONDARYFLIGHTCONTROLDAO_H