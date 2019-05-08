#include "humanplayerinputmenu.h"

#include "game/gamemap.h"

#include "menue/gamemenue.h"
#include "objects/cursor.h"

#include "coreengine/mainapp.h"

#include "resource_management/gamemanager.h"

#include "resource_management/fontmanager.h"

HumanPlayerInputMenu::HumanPlayerInputMenu(QStringList texts, QStringList actionIDs, QVector<oxygine::spActor> icons,
                                           QVector<qint32> costList, QVector<bool> enabledList)
    : m_ActionIDs(actionIDs),
      m_CostList(costList)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    connect(pApp, &Mainapp::sigKeyDown, this, &HumanPlayerInputMenu::keyInput, Qt::QueuedConnection);
    qint32 width = 0;
    oxygine::TextStyle style = FontManager::getMainFont();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_DEFAULT;
    style.fontSize = 20;
    oxygine::spTextField testText = new oxygine::TextField();
    for (qint32 i = 0; i < texts.size(); i++)
    {

        testText->setStyle(style);
        testText->setText(texts[i].toStdString().c_str());
        qint32 newWidth = testText->getTextRect().getWidth();
        if (newWidth > width )
        {
            width = newWidth;
        }
    }
    width += GameMap::Imagesize + GameMap::Imagesize * 3 / 4 ;

    GameManager* pGameManager = GameManager::getInstance();
    oxygine::spBox9Sprite pTopBox = new oxygine::Box9Sprite();
    oxygine::ResAnim* pAnim = pGameManager->getResAnim("menu+top");
    pTopBox->setResAnim(pAnim);
    pTopBox->setSize(pAnim->getSize());
    pTopBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pTopBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pTopBox->setWidth(width);
    this->addChild(pTopBox);

    qint32 y = static_cast<qint32>(pAnim->getHeight());
    startY = y;
    m_Cursor = new oxygine::Sprite();
    pAnim = pGameManager->getResAnim("cursor+menu");
    if (pAnim->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime, -1);
        m_Cursor->addTween(tween);
    }
    else
    {
        m_Cursor->setResAnim(pAnim);
    }
    m_Cursor->setPosition(width - m_Cursor->getScaledWidth() / 2, y + GameMap::Imagesize / 2 - m_Cursor->getScaledHeight() / 2);
    m_Cursor->setScale(GameMap::Imagesize / pAnim->getWidth());
    for (qint32 i = 0; i < actionIDs.size(); i++)
    {
        bool enabled = true;
        if (enabledList.size() > 0)
        {
            enabled = enabledList[i];
        }
        qint32 costs = 0;
        if (costList.size() > 0)
        {
            costs = costList[i];
        }

        oxygine::spBox9Sprite pItemBox = new oxygine::Box9Sprite();
        pAnim = pGameManager->getResAnim("menu+middle");
        pItemBox->setResAnim(pAnim);
        pItemBox->setSize(pAnim->getSize());
        pItemBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
        pItemBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);        
        pItemBox->addChild(icons[i]);
        icons[i]->setPosition(3, 0);
        pItemBox->setHeight(GameMap::Imagesize);
        pItemBox->setY(y);
        pItemBox->setWidth(width);

        // text for the item
        oxygine::spTextField textField = new oxygine::TextField();

        // set color font based
        if (!enabled)
        {
            style.color = oxygine::Color(255, 0, 0, 255);
        }
        else
        {
            style.color = oxygine::Color(255, 255, 255, 255);
        }
        textField->setStyle(style);
        textField->setText(texts[i].toStdString().c_str());
        textField->attachTo(pItemBox);
        textField->setPosition(3 + GameMap::Imagesize, 0);
        textField->setSize(width - textField->getX(), GameMap::Imagesize - 4);

        this->addChild(pItemBox);
        pItemBox->addEventListener(oxygine::TouchEvent::OVER, [=](oxygine::Event *pEvent)->void
        {
            pEvent->stopPropagation();
            m_Cursor->setY(y + GameMap::Imagesize / 2 - m_Cursor->getScaledHeight() / 2);
            currentAction = i;
        });
        QString action = actionIDs[i];
        if (enabled)
        {
            pItemBox->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event *pEvent)->void
            {
                oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
                if (pTouchEvent != nullptr)
                {
                    if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Left)
                    {
                        pEvent->stopPropagation();
                        emit sigItemSelected(action, costs);
                    }
                }
            });
        }
        y += pItemBox->getHeight();
        itemHeigth = pItemBox->getHeight();
    }
    oxygine::spBox9Sprite pBottomBox = new oxygine::Box9Sprite();
    pAnim = pGameManager->getResAnim("menu+bottom");
    pBottomBox->setResAnim(pAnim);
    pBottomBox->setSize(pAnim->getSize());
    pBottomBox->setWidth(width);
    pBottomBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pBottomBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pBottomBox->setY(y);
    this->addChild(pBottomBox);
    this->addChild(m_Cursor);
    this->setPriority(static_cast<qint16>(Mainapp::ZOrder::Objects));
    this->setHeight(y + pBottomBox->getHeight());
    this->setWidth(width);
    GameMenue* pGameMenue = GameMenue::getInstance();
    connect(pGameMenue, &GameMenue::sigMouseMove, this, &HumanPlayerInputMenu::mouseMove, Qt::QueuedConnection);
}

void HumanPlayerInputMenu::leftClick(qint32, qint32)
{
    emit sigItemSelected(m_ActionIDs[currentAction], m_CostList[currentAction]);
}

void HumanPlayerInputMenu::setMenuPosition(qint32 x, qint32 y)
{
    GameMap* pMap = GameMap::getInstance();

    if (x + getWidth() + GameMap::Imagesize / 2 > pMap->getMapWidth() * GameMap::Imagesize)
    {
        x = pMap->getMapWidth() * GameMap::Imagesize - getWidth() - GameMap::Imagesize / 2;
    }
    if (y + getHeight() + GameMap::Imagesize / 2 > pMap->getMapHeight() * GameMap::Imagesize)
    {
        y = pMap->getMapHeight() * GameMap::Imagesize - getHeight() - GameMap::Imagesize / 2;
    }
    this->setPosition(x, y);
}

void HumanPlayerInputMenu::keyInput(SDL_Event event)
{
    // for debugging
    SDL_Keycode cur = event.key.keysym.sym;
    if (cur == Settings::getKey_up())
    {
        if (currentAction > 0)
        {
            currentAction--;
        }
    }
    else if (cur == Settings::getKey_down())
    {
        if (currentAction < m_ActionIDs.size() - 1)
        {
            currentAction++;
        }
    }
    else if (cur == Settings::getKey_left())
    {
        currentAction = 0;
    }
    else if (cur == Settings::getKey_right())
    {
        currentAction = m_ActionIDs.size() - 1;
    }
    else if (cur == Settings::getKey_confirm())
    {
        if (m_ActionIDs.size() > 0)
        {
            if (m_CostList.size() == m_ActionIDs.size())
            {
                emit sigItemSelected(m_ActionIDs[currentAction], m_CostList[currentAction]);
            }
            else
            {
                emit sigItemSelected(m_ActionIDs[currentAction], 0);
            }
        }
    }
    else if (cur == Settings::getKey_cancel())
    {
        emit sigCanceled(0, 0);
    }
    m_Cursor->setY(startY + itemHeigth * currentAction + GameMap::Imagesize / 2 - m_Cursor->getScaledHeight() / 2);
}

void HumanPlayerInputMenu::mouseMove(qint32 x, qint32 y)
{
    qint32 newX = -1;
    qint32 newY = -1;
    GameMap* pMap = GameMap::getInstance();
    if (x < this->getX() * pMap->getZoom() || x > (this->getX() + this->getWidth()) * pMap->getZoom())
    {
        newX = (this->getX() + this->getWidth() / 2) * pMap->getZoom();
    }
    if (y < (this->getY() + startY) * pMap->getZoom())
    {
        newY = (this->getY() + startY + itemHeigth / 2) * pMap->getZoom();
        currentAction = 0;
    }
    if (y > (this->getY() + startY + itemHeigth * m_ActionIDs.size()) * pMap->getZoom())
    {
        newY = (this->getY() + startY + itemHeigth * m_ActionIDs.size() - itemHeigth / 2) * pMap->getZoom();
        currentAction = m_ActionIDs.size() - 1;
    }
    if (newX < 0 && newY >= 0)
    {
        newX = x;
    }
    if (newY < 0 && newX >= 0)
    {
        newY = y;
    }
    if (newX >= 0 && newY >= 0)
    {
        // warp cursor to sweet nice position
        SDL_WarpMouseInWindow(oxygine::core::getWindow(), pMap->getX() + newX, pMap->getY() + newY);
    }
}
