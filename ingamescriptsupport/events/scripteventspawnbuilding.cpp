#include "ingamescriptsupport/events/scripteventspawnbuilding.h"

#include "resource_management/buildingspritemanager.h"

ScriptEventSpawnBuilding::ScriptEventSpawnBuilding()
    : ScriptEventGeneric(EventType::SpawnBuilding, ScriptEvent::EventSpawnBuilding)
{
    m_Items.append(ScriptEventGeneric::Data());
    m_Items.append(ScriptEventGeneric::Data());
    m_Items.append(ScriptEventGeneric::Data());

    m_Items[0].min = 1;
    m_Items[0].infinite = -1;
    m_Items[0].tooltip = tr("Building that gets spawned");
    m_Items[0].labelText = tr("Owner: ");
    m_Items[0].types = ScriptEventGeneric::EditTypes::Selection;
    m_Items[0].item = "TOWN";
    m_Items[0].beforeCode = "map.replaceBuilding(\"";
    m_Items[0].afterCode = "\", ";
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
    for (qint32 i = 0; i < pBuildingSpriteManager->getCount(); i++)
    {
        m_Items[0].items.append(pBuildingSpriteManager->getName(i));
        m_Items[0].data.append(pBuildingSpriteManager->getID(i));
    }

    m_Items[1].min = 0;
    m_Items[1].infinite = -1;
    m_Items[1].tooltip = tr("X Position of the Building");
    m_Items[1].types = ScriptEventGeneric::EditTypes::Integer;
    m_Items[1].beforeCode = "";
    m_Items[1].labelText = tr("X: ");
    m_Items[1].item = "0";
    m_Items[1].afterCode = ", ";

    m_Items[2].min = 0;
    m_Items[2].infinite = -1;
    m_Items[2].tooltip = tr("Y Position of the Building");
    m_Items[2].types = ScriptEventGeneric::EditTypes::Integer;
    m_Items[2].beforeCode = "";
    m_Items[2].labelText = tr("Y: ");
    m_Items[2].item = "0";
    m_Items[2].afterCode = ");";
}
