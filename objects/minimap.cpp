#include "3rd_party/oxygine-framework/oxygine/actor/ColorRectSprite.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"
#include "3rd_party/oxygine-framework/oxygine/TouchEvent.h"
#include "3rd_party/oxygine-framework/oxygine/tween/tweentogglevisibility.h"

#include "objects/minimap.h"

#include "game/gamemap.h"
#include "game/player.h"
#include "game/unit.h"
#include "game/building.h"

#include "resource_management/gamemanager.h"

#include "menue/gamemenue.h"

Minimap::Minimap()
{
#ifdef GRAPHICSUPPORT
    setObjectName("Minimap");
#endif
    Interpreter::setCppOwnerShip(this);
    addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event* pEvent)
    {
        oxygine::TouchEvent* pTouchEvent = oxygine::safeCast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            qint32 x = pTouchEvent->localPosition.x() / IMAGE_SIZE;
            qint32 y = pTouchEvent->localPosition.y() / IMAGE_SIZE;
            emit clicked(x, y, false);
        }
    });
}

void Minimap::clear()
{
    // clear minimap
    removeChildren();
    m_Items.clear();
}

void Minimap::setMenu(GameMenue* pMenu)
{
    m_pGamemenu = pMenu;
}

void Minimap::updateMinimap(const GameMap* pMap, bool useVision)
{
    Mainapp::getInstance()->pauseRendering();
     // load minimap
    if (pMap != nullptr)
    {
        CONSOLE_PRINT("Minimap::updateMinimap", GameConsole::eDEBUG);
        qint32 width = static_cast<qint32>(pMap->getMapWidth());
        qint32 heigth = static_cast<qint32>(pMap->getMapHeight());
        qint32 size = width * heigth;
        setSize(IMAGE_SIZE * width, IMAGE_SIZE * heigth);
        if (size != m_Items.length())
        {
            clear();
            m_Items.reserve(size);
        }

        Player* pPlayer = pMap->getCurrentPlayer();
        if (m_pGamemenu != nullptr)
        {
            pPlayer = m_pGamemenu->getCurrentViewPlayer();
        }
        if (pPlayer != nullptr)
        {
            GameManager* pGameManager = GameManager::getInstance();
            for (qint32 x = 0; x < width; x++)
            {
                for (qint32 y = 0; y < heigth; y++)
                {
                    qint32 item = x * heigth + y;
                    if (item >= m_Items.size())
                    {
                        m_Items.append(MinimapFieldInfo());
                    }
                    if (pPlayer->getFieldVisibleType(x, y) == GameEnums::VisionType_Shrouded &&
                        pMap->getGameRules()->getFogMode() == GameEnums::Fog_OfShroud)
                    {
                        if (m_Items[item].terrainId != "||SHROUDED||")
                        {
                            oxygine::spColorRectSprite pSprite = oxygine::spColorRectSprite::create();
                            pSprite->setColor(Qt::black);
                            pSprite->setPosition(x * IMAGE_SIZE, y * IMAGE_SIZE);
                            pSprite->setSize(IMAGE_SIZE, IMAGE_SIZE);
                            addChild(pSprite);
                            if (m_Items[item].background.get())
                            {
                                m_Items[item].background->detach();
                            }
                            m_Items[item].background = pSprite;
                            m_Items[item].terrainId = "||SHROUDED||";
                            if (m_Items[item].unit.get())
                            {
                                m_Items[item].unit->detach();
                                m_Items[item].unitId = "";
                                m_Items[item].unitPlayer = -1;
                            }
                        }
                    }
                    else
                    {
                        Terrain* pTerrain = pMap->getTerrain(x, y);
                        Building* pBuilding = pTerrain->getBuilding();
                        QString id = pTerrain->getID();
                        if (pBuilding != nullptr)
                        {
                            qint32 owner = -1;
                            Player* pPlayer = pBuilding->getOwner();
                            if (pPlayer != nullptr && !pBuilding->getNeutralLoaded())
                            {
                                owner = pPlayer->getPlayerID();
                            }
                            if (id != m_Items[item].terrainId || owner != m_Items[item].terrainOwner)
                            {
                                if (m_Items[item].background.get())
                                {
                                    m_Items[item].background->detach();
                                }
                                m_Items[item].terrainId = id;
                                m_Items[item].terrainOwner = owner;
                                oxygine::ResAnim* pAnim = pGameManager->getResAnim(pBuilding->getMinimapIcon());
                                if (pAnim != nullptr)
                                {
                                    oxygine::spSprite pSprite = oxygine::spSprite::create();
                                    if (pAnim->getTotalFrames() > 1)
                                    {
                                        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(pAnim->getTotalFrames() * GameMap::frameTime), -1);
                                        pSprite->addTween(tween);
                                    }
                                    else
                                    {
                                        pSprite->setResAnim(pAnim);
                                    }
                                    pSprite->setPosition(x * IMAGE_SIZE, y * IMAGE_SIZE);
                                    if (pAnim->getWidth() > 0)
                                    {
                                        pSprite->setScale(static_cast<float>(IMAGE_SIZE) / static_cast<float>(pAnim->getWidth()));
                                    }
                                    if (pPlayer != nullptr && !pBuilding->getNeutralLoaded())
                                    {
                                        QColor color = pPlayer->getColor();
                                        pSprite->setColor(color.red(), color.green(), color.blue(), 255);
                                    }
                                    addChild(pSprite);
                                    m_Items[item].background = pSprite;
                                }
                            }
                        }
                        else if (id != m_Items[item].terrainId)
                        {
                            if (m_Items[item].background.get())
                            {
                                m_Items[item].background->detach();
                            }
                            m_Items[item].terrainId = id;
                            m_Items[item].terrainOwner = -1;
                            oxygine::ResAnim* pAnim = pGameManager->getResAnim(pTerrain->getMinimapIcon());
                            if (pAnim != nullptr)
                            {
                                oxygine::spSprite pSprite = oxygine::spSprite::create();
                                if (pAnim->getTotalFrames() > 1)
                                {
                                    oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(pAnim->getTotalFrames() * GameMap::frameTime), -1);
                                    pSprite->addTween(tween);
                                }
                                else
                                {
                                    pSprite->setResAnim(pAnim);
                                }
                                pSprite->setPosition(x * IMAGE_SIZE, y * IMAGE_SIZE);
                                if (pAnim->getWidth() > 0)
                                {
                                    pSprite->setScale(static_cast<float>(IMAGE_SIZE) / static_cast<float>(pAnim->getWidth()));
                                }
                                addChild(pSprite);
                                m_Items[item].background = pSprite;
                            }
                        }
                        Unit* pUnit = pTerrain->getUnit();
                        bool removeUnit = true;
                        if (pUnit != nullptr)
                        {
                            if (!useVision || !pUnit->isStealthed(pPlayer))
                            {
                                removeUnit = false;
                                if (pUnit->getUnitID() != m_Items[item].unitId ||
                                    pUnit->getOwner()->getPlayerID() != m_Items[item].unitPlayer)
                                {
                                    if (m_Items[item].unit.get())
                                    {
                                        m_Items[item].unit->detach();
                                    }
                                    m_Items[item].unitId = pUnit->getUnitID();
                                    m_Items[item].unitPlayer = pUnit->getOwner()->getPlayerID();
                                    oxygine::ResAnim* pAnim = pGameManager->getResAnim("minimap_unit");
                                    if (pAnim != nullptr)
                                    {
                                        oxygine::spSprite pSprite = oxygine::spSprite::create();
                                        if (pAnim->getTotalFrames() > 1)
                                        {
                                            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(pAnim->getTotalFrames() * GameMap::frameTime), -1);
                                            pSprite->addTween(tween);
                                        }
                                        else
                                        {
                                            pSprite->setResAnim(pAnim);
                                        }
                                        oxygine::spTween tween2 = oxygine::createTween(TweenToggleVisibility(0, 0.5f), oxygine::timeMS(1000), -1);
                                        pSprite->addTween(tween2);
                                        pSprite->setPosition(x * IMAGE_SIZE, y * IMAGE_SIZE);
                                        if (pAnim->getWidth() > 0)
                                        {
                                            pSprite->setScale(static_cast<float>(IMAGE_SIZE) / static_cast<float>(pAnim->getWidth()));
                                        }
                                        Player* pPlayer = pUnit->getOwner();
                                        QColor color = pPlayer->getColor();
                                        pSprite->setColor(color.red(), color.green(), color.blue(), 255);
                                        pSprite->setPriority(1);
                                        addChild(pSprite);
                                        m_Items[item].unit = pSprite;
                                    }
                                }
                                else
                                {
#ifdef GRAPHICSUPPORT
                                    if (m_Items[item].unit.get() != nullptr)
                                    {
                                        auto & tweens = m_Items[item].unit->getTweens();
                                        for (auto & tween : tweens)
                                        {
                                            tween->reset();
                                            tween->start(*m_Items[item].unit);
                                        }
                                    }
#endif
                                }
                            }
                        }
                        if (removeUnit)
                        {
                            if (m_Items[item].unit.get())
                            {
                                m_Items[item].unit->detach();
                                m_Items[item].unitId = "";
                                m_Items[item].unitPlayer = -1;
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        clear();
    }
    Mainapp::getInstance()->continueRendering();
}
