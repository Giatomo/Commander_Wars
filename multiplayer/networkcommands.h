#pragma once

#include <QObject>

namespace NetworkCommands
{
    static const Qt::ConnectionType UNIQUE_DATA_CONNECTION = static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection);

    enum class PublicKeyActions
    {
        SendInitialMapUpdate,
        CreateAccount,
        LoginAccount,
        ResetPassword,
        ChangePassword,
        RequestLoginData,
        DeleteAccount,
    };

    enum DisconnectReason
    {
        InvalidPassword,
        NoMoreObservers,
        NoValidPlayerAvailable,
        InvalidConnection,
        InvalidUsername,
        PasswordOutdated,
        DatabaseAccessError,
        UsernameAlreadyInGame,
        Max,
    };
    /**
     * @brief RESYNCINFO
     */
    const char* const RESYNCINFO = "RESYNCINFO";
    /**
     * @brief MAPINFO map info of from the host
     */
    const char* const MAPINFO = "MAPINFO";
    /**
     * @brief REQUESTRULE request the game rules from the host
     */
    const char* const REQUESTRULE = "REQUESTRULE";
    /**
     * @brief RANDOMMAPIDENTIFIER
     */
    const char* const RANDOMMAPIDENTIFIER = "??;";
    /**
     * @brief HOSTMAPIDENTIFIER
     */
    const char* const SERVERMAPIDENTIFIER = ";||;";
    /**
     * @brief SENDINITUPDATE initial update from the host containing the game rules and all player data
     */
    const char* const SENDINITUPDATE = "SENDINITUPDATE";
    /**
     * @brief REQUESTMAP the client daoesn't own the map request it from the host
     */
    const char* const REQUESTMAP = "REQUESTMAP";
    /**
     * @brief MAPDATA the map from the host either as savegame or as map file
     */
    const char* const MAPDATA = "MAPDATA";
    /**
     * @brief INITGAME initializes the game
     */
    const char* const INITGAME = "INITGAME";
    /**
     * @brief CLIENTREADY the client is ready to start the game
     */
    const char* const CLIENTREADY = "CLIENTREADY";
    /**
     * @brief PLAYERDATA contains the complete player as stream data
     */
    const char* const PLAYERDATA = "PLAYERDATA";
    /**
     * @brief PLAYERARMY
     */
    const char* const PLAYERARMY = "PLAYERARMY";
    /**
     * @brief COLORDATA the color of the selected player
     */
    const char* const COLORDATA = "COLORDATA";
    /**
     * @brief CODATA the selected co's of a player
     */
    const char* const CODATA = "CODATA";
    /**
     * @brief PLAYERCHANGED the ai owner of a player has changed
     */
    const char* const PLAYERCHANGED = "PLAYERCHANGED";
    /**
     * @brief REQUESTPLAYER request a player from the host
     */
    const char* const REQUESTPLAYER = "REQUESTPLAYER";
    /**
     * @brief SERVERREADY the host is ready to start the game
     */
    const char* const SERVERREADY = "SERVERREADY";
    /**
     * @brief CLIENTINITGAME the client has initialized the game
     */
    const char* const CLIENTINITGAME = "CLIENTINITGAME";
    /**
     * @brief STARTGAME the host has started the game. the client is allowed to start the game as well.
     */
    const char* const STARTGAME = "STARTGAME";    
    /**
     * @brief JOINASOBSERVER
     */
    const char* const JOINASOBSERVER = "JOINASOBSERVER";
    /**
     * @brief PLAYERACCESSDENIED
     */
    const char* const PLAYERACCESSDENIED = "PLAYERACCESSDENIED";
    /**
     * @brief REQUESTJOINREASON
     */
    const char* const REQUESTJOINREASON = "REQUESTJOINREASON";
    /**
     * @brief VERIFYGAMEDATA
     */
    const char* const VERIFYGAMEDATA = "VERIFYGAMEDATA";
    /**
     * @brief GAMEDATAVERIFIED
     */
    const char* const GAMEDATAVERIFIED = "GAMEDATAVERIFIED";
    /**
     * @brief JOINASPLAYER
     */
    const char* const JOINASPLAYER = "JOINASPLAYER";
    /**
     * @brief WAITFORPLAYERJOINSYNCFINISHED
     */
    const char* const WAITFORPLAYERJOINSYNCFINISHED = "WAITFORPLAYERJOINSYNCFINISHED";
    /**
     * @brief WAITINGFORSYNCFINISHED
     */
    const char* const WAITINGFORPLAYERJOINSYNCFINISHED = "WAITINGFORPLAYERJOINSYNCFINISHED";
    /**
     * @brief SENDCURRENTGAMESTATE
     */
    const char* const SENDCURRENTGAMESTATE = "SENDCURRENTGAMESTATE";
    /**
     * @brief RECEIVEDCURRENTGAMESTATE
     */
    const char* const RECEIVEDCURRENTGAMESTATE = "RECEIVEDCURRENTGAMESTATE";
    /**
     * @brief PLAYERJOINEDFINISHED
     */
    const char* const PLAYERJOINEDFINISHED = "PLAYERJOINEDFINISHED";
    /**
     * @brief REQUESTPLAYERCONTROLLEDINFO
     */
    const char* const REQUESTPLAYERCONTROLLEDINFO = "REQUESTPLAYERCONTROLLEDINFO";
    /**
     * @brief RECEIVEPLAYERCONTROLLEDINFO
     */
    const char* const RECEIVEPLAYERCONTROLLEDINFO = "RECEIVEPLAYERCONTROLLEDINFO";
    /**
     * @brief REQUESTPUBLICKEY
     */
    const char* const REQUESTPUBLICKEY = "REQUESTPUBLICKEY";
    /**
     * @brief SENDPUBLICKEY
     */
    const char* const SENDPUBLICKEY = "SENDPUBLICKEY";
    /**
     * @brief CRYPTEDMESSAGE
     */
    const char* const CRYPTEDMESSAGE = "CRYPTEDMESSAGE";
    /**
     * @brief CRYPTEDMESSAGE
     */
    const char* const VERIFYLOGINDATA = "VERIFYLOGINDATA";
    /**
     * @brief DISCONNECTINFOFROMSERVER
     */
    const char* const DISCONNECTINGFROMSERVER = "DISCONNECTINGFROMSERVER";
    /**
     * @brief REQUESTUSERNAME
     */
    const char* const REQUESTUSERNAME = "REQUESTUSERNAME";
    /**
     * @brief SENDUSERNAME
     */
    const char* const SENDUSERNAME = "SENDUSERNAME";

    /*****************************************************************************************************************************/
    // dedicated server commands
    /*****************************************************************************************************************************/
    /**
     * @brief LAUNCHGAMEONSERVER
     */
    const char* const LAUNCHGAMEONSERVER = "LAUNCHGAMEONSERVER";
    /**
      * @brief LAUNCHGAMEONSERVER
      */
    const char* const GAMERUNNINGONSERVER = "GAMERUNNINGONSERVER";
    /**
     * @brief SERVERREQUESTOPENPLAYERCOUNT
     */
    const char* const SERVERREQUESTOPENPLAYERCOUNT = "SERVERREQUESTOPENPLAYERCOUNT";
    /**
     * @brief SERVEROPENPLAYERCOUNT
     */
    const char* const SERVEROPENPLAYERCOUNT = "SERVEROPENPLAYERCOUNT";
    /**
     * @brief SERVERGAMEDATA
     */
    const char* const SERVERGAMEDATA = "SERVERGAMEDATA";
    /**
     * @brief SERVERGAMESTARTED
     */
    const char* const SLAVEGAMESTARTED = "SLAVEGAMESTARTED";
    /**
     * @brief SERVERJOINGAME
     */
    const char* const SERVERJOINGAME = "SERVERJOINGAME";
    /**
     * @brief SLAVEREADY
     */
    const char* const SLAVEREADY = "SLAVEREADY";
    /**
     * @brief SLAVERELAUNCHED
     */
    const char* const SLAVERELAUNCHED = "SLAVERELAUNCHED";
    /**
     * @brief SLAVEADDRESSINFO
     */
    const char* const SLAVEADDRESSINFO = "SLAVEADDRESSINFO";
    /**
     * @brief STARTSERVERGAME
     */
    const char* const STARTSERVERGAME = "STARTSERVERGAME";
    /**
     * @brief GAMENOLONGERAVAILABLE
     */
    const char* const SERVERGAMENOLONGERAVAILABLE = "SERVERGAMENOLONGERAVAILABLE";
    /**
     * @brief SERVERINVALIDMODCONFIG
     */
    const char* const SERVERINVALIDMODCONFIG = "SERVERINVALIDMODCONFIG";
    /**
     * @brief SERVERNOGAMESLOTSAVAILABLE
     */
    const char* const SERVERNOGAMESLOTSAVAILABLE = "SERVERNOGAMESLOTSAVAILABLE";
    /**
     * @brief SERVERACCOUNTMESSAGE
     */
    const char* const SERVERACCOUNTMESSAGE = "SERVERACCOUNTMESSAGE";
    /**
     * @brief SERVERREQUESTUSERGAMES
     */
    const char* const SERVERREQUESTUSERGAMES = "SERVERREQUESTUSERGAMES";
    /**
     * @brief SERVERREQUESTOBSERVEGAMES
     */
    const char* const SERVERREQUESTOBSERVEGAMES = "SERVERREQUESTOBSERVEGAMES";
    /**
     * @brief SERVERUSERGAMEDATA
     */
    const char* const SERVERUSERGAMEDATA = "SERVERUSERGAMEDATA";
    /**
     * @brief SERVERREQUESTGAMES
     */
    const char* const SERVERREQUESTGAMES = "SERVERREQUESTGAMES";
    /**
     * @brief SERVERREQUESTVERSION
     */
    const char* const SERVERREQUESTVERSION = "SERVERREQUESTVERSION";
    /**
     * @brief SERVERVERSION
     */
    const char* const SERVERVERSION = "SERVERVERSION";
    /**
     * @brief SERVERRELAUNCHSLAVE
     */
    const char* const SERVERRELAUNCHSLAVE = "SERVERRELAUNCHSLAVE";
    /**
     * @brief SLAVEINFODESPAWNING
     */
    const char* const SLAVEINFODESPAWNING = "SLAVEINFODESPAWNING";
    /**
     * @brief SLAVEINFODESPAWNING
     */
    const char* const SLAVEMULTIPLAYERGAMERESULT = "SLAVEMULTIPLAYERGAMERESULT";
    /**
     * @brief DESPAWNSLAVE
     */
    const char* const DESPAWNSLAVE = "DESPAWNSLAVE";
    /**
     * @brief SLAVEFORCEDESPAWN
     */
    const char* const SLAVEFORCEDESPAWN = "SLAVEFORCEDESPAWN";
    /**
     * @brief ONLINEINFO
     */
    const char* const ONLINEINFO = "ONLINEINFO";
    /**
     * @brief SERVERREQUESTAUTOMATCHINFO
     */
    const char* const SERVERREQUESTAUTOMATCHINFO = "SERVERREQUESTAUTOMATCHINFO";
    /**
     * @brief SERVERSENDAUTOMATCHINFO
     */
    const char* const SERVERSENDAUTOMATCHINFO = "SERVERSENDAUTOMATCHINFO";
}
