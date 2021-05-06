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
#include <QVector>

#include "FlightPlanData.h"
#include "FlightPlan.h"

class FlightPlanPrivate
{
public:
    FlightPlanPrivate() noexcept
    {}

    QVector<FlightPlanData> flightPlanData;
};

// PUBLIC

FlightPlan::FlightPlan(QObject *parent) noexcept
    : QObject(parent),
      d(std::make_unique<FlightPlanPrivate>())
{
}

FlightPlan::~FlightPlan() noexcept
{
}

void FlightPlan::add(const FlightPlanData &flightPlanData) noexcept
{
    d->flightPlanData.append(flightPlanData);
    emit waypointAdded(flightPlanData);
}

void FlightPlan::update(int index, const FlightPlanData &flightPlanData) noexcept
{
    FlightPlanData data = d->flightPlanData.at(index);
    bool changed = false;
    if (index >= 0 && index < d->flightPlanData.count()) {
        if (data.timestamp != flightPlanData.timestamp) {
            data.timestamp = flightPlanData.timestamp;
            changed = true;
        }
        if (data.latitude != flightPlanData.latitude) {
            data.latitude = flightPlanData.latitude;
            changed = true;
        }
        if (data.longitude != flightPlanData.longitude) {
            data.longitude = flightPlanData.longitude;
            changed = true;
        }
        if (data.altitude != flightPlanData.altitude) {
            data.altitude = flightPlanData.altitude;
            changed = true;
        }
        if (data.localTime != flightPlanData.localTime) {
            data.localTime = flightPlanData.localTime;
            changed = true;
        }
        if (data.zuluTime != flightPlanData.zuluTime) {
            data.zuluTime = flightPlanData.zuluTime;
            changed = true;
        }
    }
    if (changed) {
        d->flightPlanData[index] = data;
        emit waypointUpdated(index, data);
    }
}

const QVector<FlightPlanData> &FlightPlan::getAllConst() const noexcept
{
    return d->flightPlanData;
}

void FlightPlan::clear() noexcept
{
    d->flightPlanData.clear();
    emit waypointsCleared();
}
