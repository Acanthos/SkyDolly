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

#include "../../../Model/src/Service/ScenarioService.h"
#include "../../../Model/src/ScenarioDescription.h"
#include "ScenarioSelectionDialog.h"
#include "ui_ScenarioSelectionDialog.h"

class ScenarioSelectionDialogPrivate
{
public:
    ScenarioSelectionDialogPrivate(ScenarioService &theScenarioService)
        : scenarioService(theScenarioService)
    {}

    ScenarioService &scenarioService;
};

// PUBLIC

ScenarioSelectionDialog::ScenarioSelectionDialog(ScenarioService &scenarioService, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScenarioSelectionDialog),
    d(std::make_unique<ScenarioSelectionDialogPrivate>(scenarioService))
{
    ui->setupUi(this);
    initUi();
}

ScenarioSelectionDialog::~ScenarioSelectionDialog()
{
    delete ui;
}

// PROTECTED

void ScenarioSelectionDialog::showEvent(QShowEvent *event) noexcept
{
    Q_UNUSED(event)

    updateUi();
}

// PRIVATE

void ScenarioSelectionDialog::initUi()
{
    ui->scenarioTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->scenarioTableWidget->setColumnCount(3);
    QStringList headers {tr("ID"), tr("Description"), tr("Aircraft")};
    ui->scenarioTableWidget->setHorizontalHeaderLabels(headers);
    ui->scenarioTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void ScenarioSelectionDialog::updateUi()
{
    QVector<ScenarioDescription> descriptions = d->scenarioService.getScenarioDescriptions();

    ui->scenarioTableWidget->clearContents();
    ui->scenarioTableWidget->setRowCount(descriptions.count());
    int rowIndex = 0;
    for (const ScenarioDescription &desc : descriptions) {
        QTableWidgetItem *newItem = new QTableWidgetItem(QString::number(desc.id));
        ui->scenarioTableWidget->setItem(rowIndex, 0, newItem);
        newItem = new QTableWidgetItem(desc.description);
        ui->scenarioTableWidget->setItem(rowIndex, 1, newItem);
        newItem = new QTableWidgetItem(desc.aircraftName);
        ui->scenarioTableWidget->setItem(rowIndex, 2, newItem);
        ++rowIndex;
    }
}
