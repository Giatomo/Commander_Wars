#ifndef FOCUSABLEOBJECT_H
#define FOCUSABLEOBJECT_H

#include <QObject>
#include <QVariant>

#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"

class FocusableObject;
using spFocusableObject = oxygine::intrusive_ptr<FocusableObject>;

class FocusableObject : public QObject, public oxygine::Sprite
{
    Q_OBJECT
public:
    explicit FocusableObject();
    virtual ~FocusableObject();

    static void looseFocus();
    static FocusableObject* getFocusedObject()
    {
        return m_focusedObject;
    }
    static bool handleEvent(QEvent *event);
    static void handleInputMethodQuery(Qt::InputMethodQuery query, QVariant arg);
    Q_INVOKABLE bool getSubComponent() const;
    Q_INVOKABLE void setSubComponent(bool subComponent);
    Q_INVOKABLE bool getFocused() const;
signals:
    void sigFocusedLost();
    void sigFocused();
    void sigLooseFocusInternal();
protected:
    virtual void focused(){}
    virtual void inputMethodQuery(Qt::InputMethodQuery query, QVariant arg){}
    /**
     * @brief keyInputMethodQueryEvent called in case a focused object shows a virtual key board
     * @param event
     */
    virtual bool doHandleEvent(QEvent *event)
    {
        return false;
    }
protected slots:
    virtual void looseFocusInternal();
public slots:
    virtual void focusedLost(){}
private slots:
    void focusedInternal();
protected:
    bool m_focused{false};
private:
    static FocusableObject* m_focusedObject;
    static bool m_registeredAtStage;
    bool m_subComponent{false};
};

Q_DECLARE_INTERFACE(FocusableObject, "FocusableObject");

#endif // FOCUSABLEOBJECT_H
