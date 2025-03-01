#ifndef EDITORSELECTION_H
#define EDITORSELECTION_H


#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include <QObject>
#include <QVector>
#include <QVector3D>
#include "game/terrain.h"
#include "game/building.h"
#include "game/gamemap.h"
#include "game/player.h"
#include "game/unit.h"

#include "coreengine/LUPDATE_MACROS.h"

class EditorSelection;
using spEditorSelection = oxygine::intrusive_ptr<EditorSelection>;

class EditorSelection final : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    /**
     * @brief The EditorMode enum describes what we want to place at the moment
     */
    ENUM_CLASS EditorMode
    {
        All = -1,
                Terrain = 0,
                Building,
                Unit,
    };
    /**
     * @brief The PlacementSize enum describes how terrain is placed
     */
    ENUM_CLASS PlacementSize
    {
        None = 0,
                Small,
                Medium,
                BigSquare,
                Big,
                Fill
    };

    explicit EditorSelection(qint32 width, bool smallScreen, GameMap* pMap);
   virtual ~EditorSelection() = default;
    inline EditorMode getCurrentMode() const
    {
        return m_Mode;
    }
    inline QString getCurrentTerrainID()
    {
        return m_Terrains.at(m_selectedItem)->getTerrainID();
    }
    inline QString getCurrentBuildingID()
    {
        return m_Buildings.at(m_selectedItem)->getBuildingID();
    }
    /**
     * @brief getCurrentSpBuilding the current selected unit
     * @return
     */
    spBuilding getCurrentSpBuilding();
    /**
     * @brief getCurrentSpUnit the current selected unit
     * @return
     */
    spUnit getCurrentSpUnit();
    /**
     * @brief getSizeMode
     * @return
     */
    PlacementSize getSizeMode() const;
    /**
     * @brief createPlayerSelection
     */
    void createPlayerSelection();
    /**
     * @brief KeyInput
     * @param event
     */
    void KeyInput(Qt::Key cur);
    /**
    * @brief getActivePalette
    * @return
    */
    Q_INVOKABLE qint32 getActivePalette() const;
    Q_INVOKABLE void selectTerrain(QString terrainID);
    Q_INVOKABLE void selectBuilding(QString buildingID);
    Q_INVOKABLE void selectUnit(QString unitID);

signals:
    void sigUpdateSelectedPlayer();
    void sigSelectionChanged();
    void sigChangeSelectedPlayer(qint32 player);
    void sigUpdateTerrainView();
    void sigUpdateBuildingView();
    void sigUpdateUnitView();
    void sigChangeScrollValue(qint32 dir);
    void sigPaletteChanged(qint32 newPalette);
public slots:
    void updateSelectedPlayer();
    void changeSelectedPlayer(qint32 player);
    void updateTerrainView();
    void updateBuildingView();
    void updateUnitView();
    void changeScrollValue(qint32 dir);
    void onPaletteChanged(qint32 newPalette);
private:
    /**
     * @brief selectBuilding
     * @param building
     */
    void selectBuilding(qint32 building);
    /**
     * @brief selectUnit
     * @param unit
     */
    void selectUnit(qint32 unit);
    /**
     * @brief selectTerrain
     * @param terrain
     */
    void selectTerrain(qint32 terrain);
    /**
     * @brief getPosX
     * @param xCounter
     * @return
     */
    qint32 getPosX(qint32 xCounter);
    /**
     * @brief initTerrainSection
     */
    void initTerrainSection();
    /**
     * @brief createTerrainSectionLabel
     * @param currentIdentifier
     * @param xCounter
     * @param posY
     */
    void createTerrainSectionLabel(qint32 item, qint32 & currentIdentifier, qint32 & xCounter, qint32 & posY);
    /**
     * @brief initBuildingSection
     */
    void initBuildingSection();
    /**
     * @brief createBuildingSectionLabel
     * @param item
     * @param currentIdentifier
     * @param xCounter
     * @param posY
     */
    void createBuildingSectionLabel(qint32 item, qint32 & currentIdentifier, qint32 & xCounter, qint32 & posY);
    /**
     * @brief initUnitSelection
     */
    void initUnitSelection();
    /**
     * @brief createUnitSectionLabel
     * @param item
     * @param currentIdentifier
     * @param xCounter
     * @param posY
     */
    void createUnitSectionLabel(qint32 item, qint32 & currentIdentifier, qint32 & xCounter, qint32 & posY);
    /**
     * @brief hideSelection
     */
    void hideSelection();
    /**
     * @brief changeSelection
     * @param item
     */
    void changeSelection(qint32 item);
private:
    void createBoxPlacementSize();
    void createBoxSelectionMode();
    void initSelection();

    /**
     * @brief calcMaxPlayerSelection the amount of player hq's shown in the select player part
     * @return
     */
    qint32 calcMaxPlayerSelection();
    oxygine::spSprite createV9Box(qint32 x, qint32 y, qint32 width, qint32 heigth);

private:
    // small hints for the ui
    static const qint32 m_frameSize = 10;
    static const qint32 m_selectionHeight = 60;
    static const qint32 m_startHPlacementSize = 0;
    static const qint32 m_startHSelectionType = m_startHPlacementSize + m_selectionHeight;
    static const qint32 m_startHSelectedPlayer = m_startHSelectionType + m_startHPlacementSize + m_selectionHeight;
    static const qint32 m_startHTerrain = m_startHSelectedPlayer + m_selectionHeight + GameMap::defaultImageSize;
    const qint32 m_startH = GameMap::getImageSize() * 2;
    static const float m_xFactor;
    static const float m_yFactor;
    EditorMode m_Mode{EditorMode::Terrain};
    PlacementSize m_SizeMode{PlacementSize::Small};
    qint32 m_StartIndex{0};
    qint32 m_xCount{0};
    qint32 m_labelWidth{0};

    oxygine::spButton m_pButtonTop;
    oxygine::spSlidingActor m_PlacementSelectionClip;
    oxygine::spActor m_PlacementActor;
    QVector<spTerrain> m_Terrains;
    QVector<oxygine::spActor> m_terrainActors;
    QVector<spBuilding> m_Buildings;
    QVector<oxygine::spActor> m_buildingActors;
    QVector<spUnit> m_Units;
    QVector<oxygine::spActor> m_unitActors;

    oxygine::spSprite m_BoxPlacementSize;
    oxygine::spSprite m_BoxSelectionType;
    oxygine::spSprite m_BoxSelectedPlayer;
    oxygine::spSprite m_BoxPlacementSelection;

    oxygine::spSprite m_CurrentSelector;
    oxygine::spSprite m_CurrentSelectorSize;
    oxygine::spSprite m_CurrentSelectorMode;

    oxygine::spSprite m_pSpriteTerrainMode;
    oxygine::spSprite m_pSpriteBuildingMode;
    oxygine::spSprite m_pSpriteUnitMode;

    qint32 m_selectedItem{0};

    qint32 m_activePalette{0};
    /**
     * @brief current selected player
     */
    spPlayer m_currentPlayer{nullptr};
    qint32 m_playerStartIndex{0};
    QVector<spBuilding> m_Players;
    GameMap* m_pMap{nullptr};
};

#endif // EDITORSELECTION_H


