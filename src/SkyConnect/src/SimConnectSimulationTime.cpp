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
#include <windows.h>

#include <SimConnect.h>

#include "../../Kernel/src/Enum.h"
#include "../../Model/src/SimVar.h"
#include "SimConnectType.h"
#include "SimConnectSimulationTime.h"

void SimConnectSimulationTime::addToDataDefinition(HANDLE simConnectHandle) noexcept
{
    ::SimConnect_AddToDataDefinition(simConnectHandle, Enum::toUnderlyingType(SimConnectType::DataDefinition::SimulationTimeDefinition), SimVar::LocalTime, "seconds", SIMCONNECT_DATATYPE_INT32);
    ::SimConnect_AddToDataDefinition(simConnectHandle, Enum::toUnderlyingType(SimConnectType::DataDefinition::SimulationTimeDefinition), SimVar::LocalYear, "number", SIMCONNECT_DATATYPE_INT32);
    ::SimConnect_AddToDataDefinition(simConnectHandle, Enum::toUnderlyingType(SimConnectType::DataDefinition::SimulationTimeDefinition), SimVar::LocalMonthOfYear, "number", SIMCONNECT_DATATYPE_INT32);
    ::SimConnect_AddToDataDefinition(simConnectHandle, Enum::toUnderlyingType(SimConnectType::DataDefinition::SimulationTimeDefinition), SimVar::LocalDayOfMonth, "number", SIMCONNECT_DATATYPE_INT32);
    ::SimConnect_AddToDataDefinition(simConnectHandle, Enum::toUnderlyingType(SimConnectType::DataDefinition::SimulationTimeDefinition), SimVar::ZuluTime, "seconds", SIMCONNECT_DATATYPE_INT32);
    ::SimConnect_AddToDataDefinition(simConnectHandle, Enum::toUnderlyingType(SimConnectType::DataDefinition::SimulationTimeDefinition), SimVar::ZuluYear, "number", SIMCONNECT_DATATYPE_INT32);
    ::SimConnect_AddToDataDefinition(simConnectHandle, Enum::toUnderlyingType(SimConnectType::DataDefinition::SimulationTimeDefinition), SimVar::ZuluMonthOfYear, "number", SIMCONNECT_DATATYPE_INT32);
    ::SimConnect_AddToDataDefinition(simConnectHandle, Enum::toUnderlyingType(SimConnectType::DataDefinition::SimulationTimeDefinition), SimVar::ZuluDayOfMonth, "number", SIMCONNECT_DATATYPE_INT32);
}