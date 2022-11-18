/**
 * Sky Dolly - The Black Sheep for Your Flight Recordings
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
#include <QWidget>
#include <QComboBox>

#include <Kernel/Enum.h>
#include "CsvLocationImportOptionWidget.h"
#include "CsvLocationImportSettings.h"
#include "ui_CsvLocationImportOptionWidget.h"

struct CsvLocationImportOptionWidgetPrivate
{
    CsvLocationImportOptionWidgetPrivate(CsvLocationImportSettings &theImportSettings) noexcept
        : settings(theImportSettings)
    {}

     CsvLocationImportSettings &settings;
};

// PUBLIC

CsvLocationImportOptionWidget::CsvLocationImportOptionWidget(CsvLocationImportSettings &settings, QWidget *parent) noexcept
    : QWidget(parent),
      ui(std::make_unique<Ui::CsvLocationImportOptionWidget>()),
      d(std::make_unique<CsvLocationImportOptionWidgetPrivate>(settings))
{
    ui->setupUi(this);
    initUi();
    updateUi();
    frenchConnection();
}

CsvLocationImportOptionWidget::~CsvLocationImportOptionWidget() = default;

// PRIVATE

void CsvLocationImportOptionWidget::frenchConnection() noexcept
{
    connect(ui->formatComboBox, &QComboBox::currentIndexChanged,
            this, &CsvLocationImportOptionWidget::onFormatChanged);
    connect(&d->settings, &CsvLocationImportSettings::extendedSettingsChanged,
            this, &CsvLocationImportOptionWidget::updateUi);
}

void CsvLocationImportOptionWidget::initUi() noexcept
{
    ui->formatComboBox->addItem("Sky Dolly", Enum::toUnderlyingType(CsvLocationImportSettings::Format::SkyDolly));
    ui->formatComboBox->addItem("Little Navmap", Enum::toUnderlyingType(CsvLocationImportSettings::Format::LittleNavmap));
}

// PRIVATE SLOTS

void CsvLocationImportOptionWidget::updateUi() noexcept
{
    const CsvLocationImportSettings::Format format = d->settings.getFormat();
    int currentIndex = 0;
    while (currentIndex < ui->formatComboBox->count() &&
           static_cast<CsvLocationImportSettings::Format>(ui->formatComboBox->itemData(currentIndex).toInt()) != format) {
        ++currentIndex;
    }
    ui->formatComboBox->setCurrentIndex(currentIndex);
}

void CsvLocationImportOptionWidget::onFormatChanged([[maybe_unused]]int index) noexcept
{
    const CsvLocationImportSettings::Format format = static_cast<CsvLocationImportSettings::Format>(ui->formatComboBox->currentData().toInt());
    d->settings.setFormat(format);
}
