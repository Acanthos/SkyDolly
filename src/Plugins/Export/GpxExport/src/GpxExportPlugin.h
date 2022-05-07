/**
 * Sky Dolly - The Black Sheep for your Flight Recordings
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
#ifndef GPXEXPORTPLUGIN_H
#define GPXEXPORTPLUGIN_H

#include <memory>

#include <QObject>
#include <QtPlugin>
#include <QWidget>

class QIODevice;
class QString;

#include <Kernel/Settings.h>
#include <PluginManager/ExportIntf.h>
#include <PluginManager/ExportPluginBase.h>

class Flight;
class Aircraft;
struct PositionData;
struct Waypoint;
class GpxExportPluginPrivate;

class GpxExportPlugin : public ExportPluginBase
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID EXPORT_INTERFACE_IID FILE "GpxExportPlugin.json")
    Q_INTERFACES(ExportIntf)
public:
    GpxExportPlugin() noexcept;
    virtual ~GpxExportPlugin() noexcept;

protected:
    virtual ExportPluginBaseSettings &getPluginSettings() const noexcept override;
    virtual QString getFileSuffix() const noexcept override;
    virtual QString getFileFilter() const noexcept override;
    virtual std::unique_ptr<QWidget> createOptionWidget() const noexcept override;
    virtual bool hasMultiAircraftSupport() const noexcept override;
    virtual bool exportFlight(const Flight &flight, QIODevice &io) noexcept override;
    virtual bool exportAircraft(const Flight &flight, const Aircraft &aircraft, QIODevice &io) noexcept override;

private:
    std::unique_ptr<GpxExportPluginPrivate> d;

    bool exportHeader(QIODevice &io) const noexcept;
    bool exportFlightInfo(QIODevice &io) const noexcept;
    bool exportAllAircraft(QIODevice &io) const noexcept;
    bool exportAircraft(const Aircraft &aircraft, QIODevice &io) const noexcept;
    bool exportWaypoints(QIODevice &io) const noexcept;
    bool exportFooter(QIODevice &io) const noexcept;

    QString getFlightDescription() const noexcept;
    QString getAircraftDescription(const Aircraft &aircraft) const noexcept;

    inline bool exportTrackPoint(const PositionData &positionData, QIODevice &io) const noexcept;
    inline bool exportWaypoint(const Waypoint &waypoint, QIODevice &io) const noexcept;
};

#endif // GPXEXPORTPLUGIN_H