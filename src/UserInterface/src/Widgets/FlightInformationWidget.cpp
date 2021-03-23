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

#include <QDialog>

#include "../../../Model/src/SimVar.h"
#include "../../../Model/src/World.h"
#include "../../../Model/src/Scenario.h"
#include "../../../Model/src/Aircraft.h"
#include "../../../Model/src/AircraftInfo.h"
#include "../../../Model/src/FlightConditions.h"
#include "../../../SkyConnect/src/SkyConnectIntf.h"
#include "../../../SkyConnect/src/Connect.h"
#include "../../../Kernel/src/SkyMath.h"
#include "FlightInformationWidget.h"
#include "ui_FlightInformationWidget.h"

class FlightInformationWidgetPrivate
{
public:
    FlightInformationWidgetPrivate(SkyConnectIntf &theSkyConnect)
        : skyConnect(theSkyConnect)
    {}

    SkyConnectIntf &skyConnect;
    static const QString WindowTitle;
};

const QString FlightInformationWidgetPrivate::WindowTitle = QT_TRANSLATE_NOOP("FlightInformationWidget", "Simulation Variables");

// PUBLIC

FlightInformationWidget::FlightInformationWidget(SkyConnectIntf &skyConnect, QWidget *parent) :
    QDialog(parent),
    d(std::make_unique<FlightInformationWidgetPrivate>(skyConnect)),
    ui(std::make_unique<Ui::FlightInformationWidget>())
{
    ui->setupUi(this);
    Qt::WindowFlags flags = Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    initUi();

}

FlightInformationWidget::~FlightInformationWidget()
{
}

// PROTECTED

void FlightInformationWidget::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)

    updateUi();

    const Scenario &currentScenario = World::getInstance().getCurrentScenario();
    const Aircraft &aircraft = currentScenario.getUserAircraft();
    connect(&currentScenario, &Scenario::flightConditionsChanged,
            this, &FlightInformationWidget::updateInfoUi);
    connect(&aircraft, &Aircraft::infoChanged,
            this, &FlightInformationWidget::updateInfoUi);
}

void FlightInformationWidget::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event)

    const Scenario &currentScenario = World::getInstance().getCurrentScenario();
    const Aircraft &aircraft = currentScenario.getUserAircraft();
    disconnect(&currentScenario, &Scenario::flightConditionsChanged,
            this, &FlightInformationWidget::updateInfoUi);
    disconnect(&aircraft, &Aircraft::infoChanged,
            this, &FlightInformationWidget::updateInfoUi);
}

// PRIVATE

void FlightInformationWidget::initUi()
{
    ui->nameLineEdit->setToolTip(SimVar::Title);
    ui->tailNumberLineEdit->setToolTip(SimVar::ATCFlightNumber);
    ui->airlineLineEdit->setToolTip(SimVar::ATCAirline);
    ui->flightLineEdit->setToolTip(SimVar::ATCId);

    ui->categoryLineEdit->setToolTip(SimVar::Category);
    ui->startOnGroundCheckBox->setToolTip(SimVar::SimOnGround);
    ui->initialAirspeedLineEdit->setToolTip(SimVar::AirspeedTrue);
    ui->surfaceTypeLineEdit->setToolTip(SimVar::SurfaceType);
    ui->wingSpanLineEdit->setToolTip(SimVar::WingSpan);
    ui->engineTypeLineEdit->setToolTip(SimVar::EngineType);
    ui->numberOfEnginesLineEdit->setToolTip(SimVar::NumberOfEngines);
    ui->aircraftAltitudeAboveGroundLineEdit->setToolTip(SimVar::PlaneAltAboveGround);

    ui->groundAltitudeLineEdit->setToolTip(SimVar::GroundAltitude);
    ui->temperatureLineEdit->setToolTip(SimVar::AmbientTemperature);
    ui->totalAirTemperatureLineEdit->setToolTip(SimVar::TotalAirTemperature);
    ui->windVelocityLineEdit->setToolTip(SimVar::AmbientWindVelocity);
    ui->windDirectionLineEdit->setToolTip(SimVar::AmbientWindDirection);
    ui->precipitationStateLineEdit->setToolTip(SimVar::AmbientPrecipState);

    ui->inCloudsCheckBox->setToolTip(SimVar::AmbientInCloud);
    ui->visibilityLineEdit->setToolTip(SimVar::AmbientVisibility);
    ui->seaLevelPressure->setToolTip(SimVar::SeaLevelPressure);
    ui->pitotIcingLineEdit->setToolTip(SimVar::PitotIcePct);
    ui->structuralIcingLineEdit->setToolTip(SimVar::StructuralIcePct);

    // Make the flight information checkboxes checkable, but not for the user
    ui->startOnGroundCheckBox->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    ui->startOnGroundCheckBox->setFocusPolicy(Qt::NoFocus);

    ui->inCloudsCheckBox->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    ui->inCloudsCheckBox->setFocusPolicy(Qt::NoFocus);
}

void FlightInformationWidget::updateUi()
{
    updateInfoUi();
}

// PRIVATE SLOTS

void FlightInformationWidget::updateInfoUi()
{
    const Scenario &currentScenario = World::getInstance().getCurrentScenario();
    const Aircraft &aircraft = currentScenario.getUserAircraftConst();
    const FlightConditions &flightConditions = currentScenario.getFlightConditionsConst();
    const AircraftInfo &aircraftInfo = aircraft.getAircraftInfo();

    ui->nameLineEdit->setText(aircraftInfo.name);
    ui->tailNumberLineEdit->setText(aircraftInfo.atcId);
    ui->airlineLineEdit->setText(aircraftInfo.atcAirline);
    ui->flightLineEdit->setText(aircraftInfo.atcFlightNumber);
    ui->categoryLineEdit->setText(aircraftInfo.category);
    ui->startOnGroundCheckBox->setChecked(aircraftInfo.startOnGround);

    ui->initialAirspeedLineEdit->setText(QString::number(aircraftInfo.initialAirspeed));
    ui->surfaceTypeLineEdit->setText(SimType::surfaceTypeToString(aircraftInfo.surfaceType));
    ui->wingSpanLineEdit->setText(QString::number(aircraftInfo.wingSpan));
    ui->engineTypeLineEdit->setText(SimType::engineTypeToString(aircraftInfo.engineType));
    ui->numberOfEnginesLineEdit->setText(QString::number(aircraftInfo.numberOfEngines));
    ui->aircraftAltitudeAboveGroundLineEdit->setText(QString::number(aircraftInfo.aircraftAltitudeAboveGround));
    ui->startOnGroundCheckBox->setChecked(aircraftInfo.startOnGround);

    ui->groundAltitudeLineEdit->setText(QString::number(flightConditions.groundAltitude));
    ui->temperatureLineEdit->setText(QString::number(flightConditions.ambientTemperature));
    ui->totalAirTemperatureLineEdit->setText(QString::number(flightConditions.totalAirTemperature));
    ui->windVelocityLineEdit->setText(QString::number(flightConditions.windVelocity));
    ui->windDirectionLineEdit->setText(QString::number(flightConditions.windDirection));
    ui->precipitationStateLineEdit->setText(SimType::precipitationStateToString(flightConditions.precipitationState));

    ui->inCloudsCheckBox->setChecked(flightConditions.inClouds);
    ui->visibilityLineEdit->setText(QString::number(flightConditions.visibility));
    ui->seaLevelPressure->setText(QString::number(flightConditions.seaLevelPressure));
    ui->pitotIcingLineEdit->setText(QString::number(SkyMath::toPercent(flightConditions.pitotIcingPercent)));
    ui->structuralIcingLineEdit->setText(QString::number(SkyMath::toPercent(flightConditions.structuralIcingPercent)));
}
