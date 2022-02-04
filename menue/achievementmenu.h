#ifndef ACHIEVEMENTMENU_H
#define ACHIEVEMENTMENU_H


#include "3rd_party/oxygine-framework/oxygine-framework.h"
#include "menue/basemenu.h"

#include "objects/base/panel.h"
#include "objects/base/textbox.h"
#include "objects/base/dropdownmenu.h"

class Achievementmenu;
using spAchievementmenu = oxygine::intrusive_ptr<Achievementmenu>;

class Achievementmenu : public Basemenu
{
    Q_OBJECT
    enum class SortDirection
    {
        None,
        Ascending,
        Descending
    };
public:

    explicit Achievementmenu();
    virtual ~Achievementmenu() = default;
signals:
    void sigExitMenue();
    void sigSearch();
public slots:
    void exitMenue();
    void search();
    virtual void onEnter() override;
private:
    void searchChanged(QString searchText, QString group, SortDirection sortDirection);

private:
    spPanel m_MainPanel;
    spTextbox m_SearchString;
    spDropDownmenu m_group;
    spDropDownmenu m_sort;
};

#endif // ACHIEVEMENTMENU_H
