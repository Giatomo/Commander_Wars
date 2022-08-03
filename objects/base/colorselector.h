#ifndef COLORSELECTOR_H
#define COLORSELECTOR_H

#include <QObject>
#include <QColor>
#include <QVector>

#include "3rd_party/oxygine-framework/oxygine/actor/ColorRectSprite.h"

#include "objects/base/spinbox.h"

class ColorSelector;
using spColorSelector = oxygine::intrusive_ptr<ColorSelector>;

class ColorSelector : public QObject, public oxygine::Actor
{
    Q_OBJECT
    static constexpr qint32 pixelCount = 4;
public:
    explicit ColorSelector(QColor color, qint32 pixelSize);
    virtual ~ColorSelector() = default;
    QColor getCurrentColor() const;

signals:
    void sigSelecetedColorChanged(QColor color);
public slots:
    void selecetedColorChanged(QColor color);
private:
    oxygine::spActor m_ColorDialog;
    spSpinBox m_SpinBoxRed;
    spSpinBox m_SpinBoxGreen;
    spSpinBox m_SpinBoxBlue;
    QColor m_CurrentColor;
    QVector<QVector<oxygine::spColorRectSprite>> m_RedGreenField;
    QVector<oxygine::spColorRectSprite> m_BlueField;
    oxygine::spSprite m_Cursor1;
    oxygine::spSprite m_Cursor2;
    qint32 m_pixelSize;
    bool m_barUpdating{false};
    bool m_boxUpdating{false};
    QElapsedTimer m_trigger;
};

#endif // COLORSELECTOR_H
