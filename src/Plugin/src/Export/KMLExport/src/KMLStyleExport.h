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
#ifndef KMLSTYLEEXPORT_H
#define KMLSTYLEEXPORT_H

#include <memory>
#include <unordered_map>
#include <utility>

#include <QColor>

class QIODevice;

#include "../../../../../Model/src/SimType.h"

class KMLStyleExportPrivate;

class KMLStyleExport
{
public:
    enum class Icon {
        Airport,
        Flag
    };

    enum class ColorStyle {
        OneColor,
        OneColorPerEngineType,
        ColorRamp,
        ColorRampPerEngineType
    };

    typedef std::unordered_map<SimType::EngineType, std::pair<QRgb, QRgb>> CategoryColor;
    typedef struct {
        CategoryColor categoryColors;
        ColorStyle colorStyle;
        int nofColorsPerRamp;
        float lineWidth;
    } StyleParameter;

    KMLStyleExport() noexcept;
    ~KMLStyleExport() noexcept;

    bool exportStyles(const StyleParameter &styleParameters, QIODevice &io) noexcept;
    QString getNextStyleMapPerEngineType(SimType::EngineType engineType) noexcept;

    static QString getStyleUrl(Icon icon) noexcept;

private:
    std::unique_ptr<KMLStyleExportPrivate> d;

    void initialiseColorRamps() noexcept;
    bool exportHighlightLineStyle(QIODevice &io) const noexcept;
    bool exportNormalLineStyles(QIODevice &io) const noexcept;
    bool exportLineStyleMaps(QIODevice &io) const noexcept;
    bool exportPlacemarkStyles(QIODevice &io) const noexcept;

    static bool exportNormalLineStylesPerEngineType(SimType::EngineType engineType, std::vector<QRgb> &colorRamp, float lineWidth, QIODevice &io) noexcept;
};

#endif // KMLSTYLEEXPORT_H
