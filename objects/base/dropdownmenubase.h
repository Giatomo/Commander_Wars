#ifndef DROPDOWNMENUBASE_H
#define DROPDOWNMENUBASE_H

#include <QObject>
#include <QVector>
#include <QString>

#include "3rd_party/oxygine-framework/oxygine/actor/ClipRectActor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"

#include "objects/base/panel.h"
#include "objects/base/tooltip.h"

class DropDownmenuBase;
using spDropDownmenuBase = oxygine::intrusive_ptr<DropDownmenuBase>;

class DropDownmenuBase : public Tooltip
{
    Q_OBJECT
public:
    explicit DropDownmenuBase(qint32 width, qint32 itemcount);
    virtual ~DropDownmenuBase() = default;
    /**
     * @brief setEnabled
     * @param value
     */
    virtual void setEnabled(bool value) override;
    /**
     * @brief changeItemCount
     * @param itemcount
     */
    void changeItemCount(qint32 itemcount);
    /**
     * @brief setCurrentItem
     * @param index
     */
    Q_INVOKABLE virtual void setCurrentItem(qint32 index) = 0;
    /**
     * @brief getItemCount
     * @return
     */
    Q_INVOKABLE inline qint32 getItemCount()
    {
        return m_Items.size();
    }
    /**
     * @brief getCurrentItem index of the current item
     * @return
     */
    Q_INVOKABLE qint32 getCurrentItem() const;
signals:
    void sigItemChangedInternal(qint32 item);

    void sigShowDropDown();
    void sigHideDropDown();
public slots:
    virtual void itemChanged(qint32 item) = 0;
    void showDropDown();
    void hideDropDown();
    virtual void focusedLost() override;
protected:
    const QSize& addDropDownItem(oxygine::spActor item, qint32 id);
protected:
    oxygine::spClipRectActor m_pClipActor;
    oxygine::spBox9Sprite m_Box;
    oxygine::spButton m_pArrowDown;
    QVector<oxygine::spBox9Sprite> m_Items;
    spPanel m_Panel;
    qint32 m_currentItem{0};
};

Q_DECLARE_INTERFACE(DropDownmenuBase, "DropDownmenuBase");

#endif // DROPDOWNMENU_H
