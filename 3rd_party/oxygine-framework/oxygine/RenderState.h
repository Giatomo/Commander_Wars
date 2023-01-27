#pragma once
#include <QTransform>

#include "3rd_party/oxygine-framework/oxygine/math/Rect.h"

namespace oxygine
{

    class RenderState final
    {
    public:
        explicit RenderState() = default;
        ~RenderState() = default;

        QColor getFinalColor(const QColor& clr) const
        {
            QColor color = clr;
            color.setAlpha((color.alpha() * static_cast<qint32>(alpha)) / 255);
            return color;
        }

        QTransform transform;
        unsigned char alpha{255};
        const RectF* clip{nullptr};
    };
}
