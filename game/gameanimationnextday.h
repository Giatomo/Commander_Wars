#ifndef GAMEANIMATIONNEXTDAY_H
#define GAMEANIMATIONNEXTDAY_H

#include <QTimer>

#include "game/gameanimation.h"
#include "game/player.h"

class GameAnimationNextDay : public GameAnimation
{
    Q_OBJECT
public:
    GameAnimationNextDay(Player* pPlayer, quint32 frameTime, bool permanent);

    virtual void restart() override;
    virtual void stop() override;

signals:
    void sigShowSaveAndExit();
    void sigRightClick();
public slots:
    void rightClick();
    virtual bool onFinished(bool skipping) override;
private:
    QTimer endTimer;
    bool m_permanent;
};

#endif // GAMEANIMATIONNEXTDAY_H
