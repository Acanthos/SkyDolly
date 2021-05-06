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

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVector>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QDateTime>
#include <QTime>
#include <QPushButton>
#include <QMessageBox>

#include "../../../Model/src/Flight.h"
#include "../../../Model/src/FlightSummary.h"
#include "../../../Model/src/Logbook.h"
#include "../../../Persistence/src/Service/FlightService.h"
#include "../Unit.h"

#include "LogbookWidget.h"
#include "ui_LogbookWidget.h"

namespace
{
    constexpr int MinimumTableWidth = 600;
    constexpr int InvalidSelection = -1;
    constexpr int InvalidColumn = -1;
}

class LogbookWidgetPrivate
{
public:
    LogbookWidgetPrivate(FlightService &theFlightService) noexcept
        : descriptionColumnIndex(InvalidColumn),
          flightService(theFlightService),
          selectedRow(InvalidSelection),
          selectedFlightId(Flight::InvalidId)

    {}

    int descriptionColumnIndex;
    FlightService &flightService;
    int selectedRow;
    qint64 selectedFlightId;
    Unit unit;
};

// PUBLIC

LogbookWidget::LogbookWidget(FlightService &flightService, QWidget *parent) noexcept
    : QWidget(parent),
      ui(std::make_unique<Ui::LogbookWidget>()),
      d(std::make_unique<LogbookWidgetPrivate>(flightService))
{
    ui->setupUi(this);
    initUi();
    frenchConnection();
}

LogbookWidget::~LogbookWidget() noexcept
{}

qint64 LogbookWidget::getSelectedFlightId() const noexcept
{
    return d->selectedFlightId;
}

QString LogbookWidget::getTitle() const noexcept
{
    return QString(QT_TRANSLATE_NOOP("LogbookWidget", "Logbook"));
}

// PROTECTED

void LogbookWidget::showEvent(QShowEvent *event) noexcept
{
    Q_UNUSED(event)
    updateUi();

    // Service
    connect(&d->flightService, &FlightService::flightStored,
            this, &LogbookWidget::updateUi);
}

void LogbookWidget::hideEvent(QHideEvent *event) noexcept
{
    Q_UNUSED(event)

    disconnect(&d->flightService, &FlightService::flightStored,
               this, &LogbookWidget::updateUi);
}

// PRIVATE

void LogbookWidget::initUi() noexcept
{
    ui->logTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    const QStringList headers {tr("Flight"), tr("Date"), tr("Aircraft"), tr("Departure Time"), tr("Departure"), tr("Arrival Time"), tr("Arrival"), tr("Total Time of Flight"), tr("Description")};
    ui->logTableWidget->setColumnCount(headers.count());
    ui->logTableWidget->setHorizontalHeaderLabels(headers);
    ui->logTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->logTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->logTableWidget->verticalHeader()->hide();
    ui->logTableWidget->setMinimumWidth(MinimumTableWidth);
}

void LogbookWidget::updateEditUi() noexcept
{
    ui->loadPushButton->setEnabled(d->selectedFlightId != Flight::InvalidId);
    ui->deletePushButton->setEnabled(d->selectedFlightId != Flight::InvalidId);
}

void LogbookWidget::frenchConnection() noexcept
{
    connect(ui->logTableWidget, &QTableWidget::itemSelectionChanged,
            this, &LogbookWidget::handleSelectionChanged);    
    connect(ui->loadPushButton, &QPushButton::clicked,
            this, &LogbookWidget::loadFlight);
    connect(ui->deletePushButton, &QPushButton::clicked,
            this, &LogbookWidget::deleteFlight);
    connect(ui->logTableWidget, &QTableWidget::cellDoubleClicked,
            this, &LogbookWidget::handleCellSelected);
    connect(ui->logTableWidget, &QTableWidget::cellChanged,
            this, &LogbookWidget::handleCellChanged);
}

// PRIVATE SLOTS

void LogbookWidget::updateUi() noexcept
{
    QVector<FlightSummary> summaries = d->flightService.getFlightDescriptions();
    ui->logTableWidget->blockSignals(true);
    ui->logTableWidget->setSortingEnabled(false);
    ui->logTableWidget->clearContents();
    ui->logTableWidget->setRowCount(summaries.count());
    int rowIndex = 0;

    for (const FlightSummary &summary : summaries) {

        int columnIndex = 0;

        QTableWidgetItem *newItem = new QTableWidgetItem();
        newItem->setData(Qt::DisplayRole, summary.id);
        ui->logTableWidget->setItem(rowIndex, columnIndex, newItem);
        ++columnIndex;

        newItem = new QTableWidgetItem(d->unit.formatDate(summary.creationDate));
        ui->logTableWidget->setItem(rowIndex, columnIndex, newItem);
        ++columnIndex;

        newItem = new QTableWidgetItem(summary.aircraftType);
        ui->logTableWidget->setItem(rowIndex, columnIndex, newItem);
        ++columnIndex;

        newItem = new QTableWidgetItem(d->unit.formatTime(summary.startDate));
        ui->logTableWidget->setItem(rowIndex, columnIndex, newItem);
        newItem->setToolTip(tr("Simulation time: %1 (%2Z)").arg(d->unit.formatTime(summary.startSimulationLocalTime), d->unit.formatTime(summary.startSimulationZuluTime)));
        ++columnIndex;

        newItem = new QTableWidgetItem(summary.startLocation);
        ui->logTableWidget->setItem(rowIndex, columnIndex, newItem);
        ++columnIndex;

        newItem = new QTableWidgetItem(d->unit.formatTime(summary.endDate));
        ui->logTableWidget->setItem(rowIndex, columnIndex, newItem);
        newItem->setToolTip(tr("Simulation time: %1 (%2Z)").arg(d->unit.formatTime(summary.endSimulationLocalTime), d->unit.formatTime(summary.endSimulationZuluTime)));
        ++columnIndex;

        newItem = new QTableWidgetItem(summary.endLocation);
        ui->logTableWidget->setItem(rowIndex, columnIndex, newItem);
        ++columnIndex;

        const qint64 durationMSec = summary.startDate.msecsTo(summary.endDate);
        const QTime time = QTime(0, 0).addMSecs(durationMSec);
        newItem = new QTableWidgetItem(d->unit.formatDuration(time));
        ui->logTableWidget->setItem(rowIndex, columnIndex, newItem);
        ++columnIndex;

        newItem = new QTableWidgetItem(summary.description);
        ui->logTableWidget->setItem(rowIndex, columnIndex, newItem);
        d->descriptionColumnIndex = columnIndex;
        ++columnIndex;
        ++rowIndex;

    }
    ui->logTableWidget->resizeColumnsToContents();
    ui->logTableWidget->setSortingEnabled(true);
    ui->logTableWidget->blockSignals(false);

    updateEditUi();
}

void LogbookWidget::handleSelectionChanged() noexcept
{
    QItemSelectionModel *select = ui->logTableWidget->selectionModel();
    QModelIndexList selectedRow = select->selectedRows(0);
    if (selectedRow.count() > 0) {
        QModelIndex modelIndex = selectedRow.at(0);
        d->selectedRow = modelIndex.row();
        d->selectedFlightId = ui->logTableWidget->model()->data(modelIndex).toLongLong();
    } else {
        d->selectedRow = InvalidSelection;
        d->selectedFlightId = Flight::InvalidId;
    }
    updateEditUi();
}

void LogbookWidget::loadFlight() noexcept
{
    qint64 selectedFlightId = d->selectedFlightId;
    if (selectedFlightId != Flight::InvalidId) {
        bool ok = d->flightService.restore(selectedFlightId, Logbook::getInstance().getCurrentFlight());
        if (!ok) {
            QMessageBox::critical(this, tr("Database error"), tr("The flight %1 could not be read from the library.").arg(selectedFlightId));
        }
    }
}

void LogbookWidget::deleteFlight() noexcept
{
    if (d->selectedFlightId != Flight::InvalidId) {
        d->flightService.deleteById(d->selectedFlightId);
        int lastSelectedRow = d->selectedRow;
        updateUi();
        int selectedRow = qMin(lastSelectedRow, ui->logTableWidget->rowCount() - 1);
        ui->logTableWidget->selectRow(selectedRow);
    }
}

void LogbookWidget::handleCellSelected(int row, int column) noexcept
{
    if (column == 8) {
        QTableWidgetItem *item = ui->logTableWidget->item(row, column);
        ui->logTableWidget->editItem(item);
    } else {
        loadFlight();
    }
}

void LogbookWidget::handleCellChanged(int row, int column) noexcept
{
    if (column == d->descriptionColumnIndex) {
        QTableWidgetItem *item = ui->logTableWidget->item(row, column);
        d->flightService.updateDescription(d->selectedFlightId, item->data(Qt::EditRole).toString());
    }
}