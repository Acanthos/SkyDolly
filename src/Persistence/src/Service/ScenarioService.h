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
#ifndef SCENARIOSERVICE_H
#define SCENARIOSERVICE_H

#include <memory>

#include <QObject>
#include <QVector>

#include "../../../Model/src/Scenario.h"
#include "../../../Model/src/ScenarioDescription.h"
#include "../PersistenceLib.h"

class ScenarioServicePrivate;

class PERSISTENCE_API ScenarioService : public QObject
{
    Q_OBJECT
public:
    ScenarioService() noexcept;
    ~ScenarioService() noexcept;

    bool store(Scenario &scenario) noexcept;
    bool restore(qint64 id, Scenario &scenario) noexcept;
    bool deleteById(qint64 id) noexcept;

    QVector<ScenarioDescription> getScenarioDescriptions() const noexcept;

signals:
    void scenarioStored(qint64 scenarioId);
    void scenarioRestored(qint64 scenarioId);

private:
    std::unique_ptr<ScenarioServicePrivate> d;
};

#endif // SCENARIOSERVICE_H
