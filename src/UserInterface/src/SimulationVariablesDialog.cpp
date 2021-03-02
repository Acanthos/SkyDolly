#include <QMetaObject>
#include <QDialog>

#include "../../Kernel/src/Aircraft.h"
#include "../../Kernel/src/AircraftInfo.h"
#include "../../SkyConnect/src/SkyConnect.h"
#include "../../SkyConnect/src/Connect.h"
#include "SimulationVariablesDialog.h"
#include "ui_SimulationVariablesDialog.h"

class SimulationVariablesDialogPrivate
{
public:
    SimulationVariablesDialogPrivate(SkyConnect &theSkyConnect)
        : skyConnect(theSkyConnect)
    {}

    SkyConnect &skyConnect;
    QMetaObject::Connection dataChangedConnection;
    QMetaObject::Connection dataSentConnection;
    QMetaObject::Connection stateChangedConnection;

    static const QString WindowTitle;
};

const QString SimulationVariablesDialogPrivate::WindowTitle = QT_TRANSLATE_NOOP("SimulationVariablesDialog", "Simulation Variables");

// PUBLIC

SimulationVariablesDialog::SimulationVariablesDialog(SkyConnect &skyConnect, QWidget *parent) :
    QDialog(parent),
    d(new SimulationVariablesDialogPrivate(skyConnect)),
    ui(new Ui::SimulationVariablesDialog)
{
    ui->setupUi(this);
    frenchConnection();
}

SimulationVariablesDialog::~SimulationVariablesDialog()
{
    delete ui;
    delete d;
}

// PROTECTED

void SimulationVariablesDialog::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);

    updateAircraftDataUi();
    updateTitle();

    const Aircraft &aircraft = d->skyConnect.getAircraft();
    // Signal sent while recording
    d->dataChangedConnection = connect(&aircraft, &Aircraft::dataChanged,
                                       this, &SimulationVariablesDialog::updateAircraftDataUi);
    // Signal sent while playing
    d->dataSentConnection = connect(&d->skyConnect, &SkyConnect::aircraftDataSent,
                                    this, &SimulationVariablesDialog::updateAircraftDataUi);

    d->stateChangedConnection = connect(&d->skyConnect, &SkyConnect::stateChanged,
                                        this, &SimulationVariablesDialog::updateTitle);
}

void SimulationVariablesDialog::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event);
    this->disconnect(d->dataChangedConnection);
    this->disconnect(d->dataSentConnection);
    this->disconnect(d->stateChangedConnection);
}

// PRIVATE

void SimulationVariablesDialog::frenchConnection()
{
    const Aircraft &aircraft = d->skyConnect.getAircraft();
    connect(&aircraft, &Aircraft::infoChanged,
            this, &SimulationVariablesDialog::updateInfoUi);
}

// PRIVATE SLOTS

void SimulationVariablesDialog::updateInfoUi()
{
    const Aircraft &aircraft = d->skyConnect.getAircraft();
    const AircraftInfo &aircraftInfo = aircraft.getAircraftInfo();

    ui->nameLineEdit->setText(aircraftInfo.name);
    ui->startOnGroundCheckBox->setChecked(aircraftInfo.startOnGround);
    ui->initialAirspeedLineEdit->setText(QString::number(aircraftInfo.initialAirspeed));
}

void SimulationVariablesDialog::updateAircraftDataUi()
{
    const AircraftData &aircraftData = getAircraftData();

    ui->latitudeLineEdit->setText(QString::number(aircraftData.latitude));
    ui->longitudeLineEdit->setText(QString::number(aircraftData.longitude));
    ui->altitudeLineEdit->setText(QString::number(aircraftData.altitude));
    ui->pitchLineEdit->setText(QString::number(aircraftData.pitch));
    ui->bankLineEdit->setText(QString::number(aircraftData.bank));
    ui->headingLineEdit->setText(QString::number(aircraftData.heading));

    ui->yokeXLineEdit->setText(QString::number(aircraftData.yokeXPosition));
    ui->yokeYLineEdit->setText(QString::number(aircraftData.yokeYPosition));
    ui->rudderLineEdit->setText(QString::number(aircraftData.rudderPosition));
    ui->elevatorLineEdit->setText(QString::number(aircraftData.elevatorPosition));
    ui->aileronLineEdit->setText(QString::number(aircraftData.aileronPosition));

    ui->leadingEdgeFlapsLeftLineEdit->setText(QString::number(aircraftData.leadingEdgeFlapsLeftPercent));
    ui->leadingEdgeFlapsRightLineEdit->setText(QString::number(aircraftData.leadingEdgeFlapsRightPercent));
    ui->trailingEdgeFlapsLeftLineEdit->setText(QString::number(aircraftData.trailingEdgeFlapsLeftPercent));
    ui->trailingEdgeFlapsRightLinEdit->setText(QString::number(aircraftData.trailingEdgeFlapsRightPercent));

    ui->throttle1LineEdit->setText(QString::number(aircraftData.throttleLeverPosition1));
    ui->throttle2LineEdit->setText(QString::number(aircraftData.throttleLeverPosition2));
    ui->throttle3LineEdit->setText(QString::number(aircraftData.throttleLeverPosition3));
    ui->throttle4LineEdit->setText(QString::number(aircraftData.throttleLeverPosition4));
    ui->spoilerLineEdit->setText(QString::number(aircraftData.spoilersHandlePosition));
    ui->flapsPositionLineEdit->setText(QString::number(aircraftData.flapsHandleIndex));

    aircraftData.gearHandlePosition ? ui->gearLineEdit->setText(tr("Down")) : ui->gearLineEdit->setText(tr("Up"));
    ui->waterRudderLineEdit->setText(QString::number(aircraftData.waterRudderHandlePosition));
    ui->brakeLeftLineEdit->setText(QString::number(aircraftData.brakeLeftPosition));
    ui->brakeRightLineEdit->setText(QString::number(aircraftData.brakeRightPosition));
}

void SimulationVariablesDialog::updateTitle()
{
    QString windowTitle = SimulationVariablesDialogPrivate::WindowTitle;
    switch (d->skyConnect.getState()) {
    case Connect::State::Recording:
        windowTitle.append(" - " + tr("RECORDING"));
        break;
    case Connect::State::RecordingPaused:
        windowTitle.append(" - " + tr("RECORDING PAUSED"));
        break;
    case Connect::State::Playback:
        windowTitle.append(" - " + tr("PLAYBACK"));
        break;
    case Connect::State::PlaybackPaused:
        windowTitle.append(" - " + tr("PLAYPACK PAUSED"));
        break;
    default:
        break;
    }
    setWindowTitle(windowTitle);
}

// PRIVATE

const AircraftData &SimulationVariablesDialog::getAircraftData() const
{
    const AircraftData aircraftData;
    const Aircraft &aircraft = d->skyConnect.getAircraft();

    if (d->skyConnect.getState() == Connect::State::Recording) {
        return aircraft.getLastAircraftData();
    } else {
        return d->skyConnect.getCurrentAircraftData();
    };
}
