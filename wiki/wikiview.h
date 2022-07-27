#ifndef WIKIVIEW_H
#define WIKIVIEW_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"

#include "objects/base/panel.h"
#include "objects/base/textbox.h"
#include "objects/base/dropdownmenu.h"

#include "wiki/wikidatabase.h"

class WikiView;
using spWikiView = oxygine::intrusive_ptr<WikiView>;

class WikiView : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit WikiView(qint32 viewWidth, qint32 viewHeigth);
    virtual ~WikiView() = default;
signals:
    void sigSearch(bool onlyTag);
    void sigShowWikipage(WikiDatabase::PageData page);
public slots:
    void showWikipage(WikiDatabase::PageData page);
    void searchChanged(QString);
    void search(bool onlyTag);
    void tagChanged(qint32 item);
    void showPage(QString id);
private:
    spTextbox m_SearchString;
    spDropDownmenu m_Tags;
    spPanel m_MainPanel;
};

Q_DECLARE_INTERFACE(WikiView, "WikiView");

#endif // WIKIVIEW_H
