#include "resource_management/achievementmanager.h"

#include "coreengine/interpreter.h"

AchievementManager::AchievementManager()
    : RessourceManagement<AchievementManager>("/images/achievements/res.xml",
                                              "/scripts/achievements")
{
}

void AchievementManager::loadAll()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    RessourceManagement<AchievementManager>::loadAll();
    for (auto achievement : m_loadedRessources)
    {
        pInterpreter->doFunction(achievement, "registerAchievements");
    }
}


void AchievementManager::onVictory(qint32 team, bool humanWin)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args;
    args << team;
    args << humanWin;
    for (auto achievement : m_loadedRessources)
    {
        pInterpreter->doFunction(achievement, "onVictory", args);
    }
}
