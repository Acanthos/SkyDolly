#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <QObject>
#include <QByteArray>
#include <QVector>

#include "KernelLib.h"
#include "AircraftInfo.h"
#include "Position.h"

class AircraftPrivate;

class KERNEL_API Aircraft : public QObject
{
    Q_OBJECT
public:    
    Aircraft(QObject *parent = nullptr);
    virtual ~Aircraft();

    void setAircraftInfo(AircraftInfo aircraftInfo);
    const AircraftInfo &getAircraftInfo() const;

    void upsertPosition(Position position);
    const Position &getLastPosition() const;
    const QVector<Position> getPositions() const;    
    const Position &getPosition(qint64 timestamp) const;

    void clear();

signals:
    void infoChanged();
    void positionChanged();

private:
    AircraftPrivate *d;

    bool updateCurrentIndex(qint64 timestamp) const;
    bool getSupportPositions(qint64 timestamp, const Position **p0, const Position **p1, const Position **p2, const Position **p3) const;
    static double normaliseTimestamp(const Position &p1, const Position &p2, quint64 timestamp);
};

#endif // AIRCRAFT_H
