#ifndef AUTOMATCHMAKER_H
#define AUTOMATCHMAKER_H

#include <QObject>
#include <QSqlQuery>
#include <3rd_party/oxygine-framework/oxygine/core/ref_counter.h>

#include "game/GameEnums.h"

#include "coreengine/fileserializable.h"
#include "coreengine/scriptvariables.h"

class MainServer;
class AutoMatchMaker;
using spAutoMatchMaker = oxygine::intrusive_ptr<AutoMatchMaker>;

class AutoMatchMaker : public QObject, public FileSerializable, public oxygine::ref_counter
{
    Q_OBJECT
public:
    explicit AutoMatchMaker(const QString & matchId, MainServer * mainServer);
    /**
     * @brief onNewMatchResultData
     * @param objData
     */
    void onNewMatchResultData(const QJsonObject & objData);
    /**
     * @brief onNewPlayerData
     * @param objData
     */
    void onNewPlayerData(const QJsonObject & objData);
    /**
     * @brief serializeObject
     * @param stream
     */
    virtual void serializeObject(QDataStream& stream) const override;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserializeObject(QDataStream& stream) override;
    /**
     * @brief getVersion version of the file
     * @return
     */
    virtual qint32 getVersion() const override
    {
        return 2;
    }
    /**
     * @brief playerJoined
     * @param player
     */
    void playerJoined(const QString & player);
    /**
     * @brief getRunning
     * @return
     */
    bool getRunning() const;
    /**
     * @brief setRunning
     * @param newRunning
     */
    void setRunning(bool newRunning);
    /**
     * @brief getActiveMatch
     * @return
     */
    bool getActiveMatch() const;
    /**
     * @brief setActiveMatch
     * @param newActiveMatch
     */
    void setActiveMatch(bool newActiveMatch);

    qint32 getNotActiveCounter() const;
    void increaseNotActiveCounter();

public slots:
    bool getSignedUp(const QString & playerId);
    void createNewGame(const QString & players);
    QString getMatchId() const;
    void updateMmr(const QString & player1, const QString & player2, qint32 maxEloChange, GameEnums::GameResult gameResultForPlayer1);
    bool setMmr(const QString & player, qint32 mmr);
    qint32 getMmr(const QString & player);
    bool setMatchHistoryData(const QString & player, QString historyData);
    QString getMatchHistoryData(const QString & player);
    bool setMatchMetaData(const QString & player, QString metaData);
    QString getMatchMetaData(const QString & player);
    QStringList getOpponentsForPlayer(const QString & player, qint32 playerCount, qint32 mmrSearchStepRange);
    qint32 getRunningGames(const QString & player);
    bool setRunningGames(const QString & player, qint32 count);
    qint32 getMinGames(const QString & player);
    qint32 getMaxGames(const QString & player);
    /**
     * @brief getVariables
     * @return
     */
    inline ScriptVariables* getVariables()
    {
        return &m_Variables;
    }
    /**
     * @brief doNewPlayerData creates new player data for this match set if no exists else updates minGames, maxGames and metaData to the given values
     * @param player
     * @param minGames
     * @param maxGames
     * @param metaData
     * @param startMmr
     */
    bool doNewPlayerData(const QString & player, qint32 minGames, qint32 maxGames, const QString & metaData, qint32 startMmr = 750);
    /**
     * @brief getName
     * @return
     */
    QString getName();
    /**
     * @brief getDescription
     * @return
     */
    QString getDescription();
    /**
     * @brief getIsSignUpChangeAllowed
     * @return
     */
    bool getIsSignUpChangeAllowed();
    /**
     * @brief getBracketGraphInfo
     * @return
     */
    QJsonObject getBracketGraphInfo();

    static QString getBracketGraphInfoId();
    static QString getBracketGraphPreviousMatchId();
    static QString getBracketGraphPreviousWinnersId();
    static QString getBracketGraphPreviousPlayersId();
private:
    QString m_matchId;
    MainServer & m_mainServer;
    quint64 m_matchCounter{0};
    ScriptVariables m_Variables;
    bool m_running{false};
    bool m_activeMatch{false};
    qint32 m_notActiveCounter{0};
};

Q_DECLARE_INTERFACE(AutoMatchMaker, "AutoMatchMaker");

#endif // AUTOMATCHMAKER_H
