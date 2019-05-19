#include "multiplayermenu.h"

#include "qcryptographichash.h"

#include "qdir.h"
#include "qdiriterator.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"
#include "coreengine/settings.h"

#include "multiplayer/lobbymenu.h"

#include "network/tcpclient.h"
#include "network/tcpserver.h"

#include "game/gamemap.h"
#include "game/player.h"
#include "game/co.h"

#include "resource_management/backgroundmanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

Multiplayermenu::Multiplayermenu(QString adress, bool host)
    : MapSelectionMapsMenue(),
      m_Host(host)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Console::print("Entering Multiplayer Menue", Console::eDEBUG);

    if (!host)
    {
        m_NetworkInterface = new TCPClient();
        m_pPlayerSelection->attachNetworkInterface(m_NetworkInterface);
        emit m_NetworkInterface->sig_connect(adress, Settings::getGamePort());
        hideRuleSelection();
        hideMapSelection();
        m_MapSelectionStep = MapSelectionStep::selectPlayer;
        // quit on host connection lost
        connect(m_NetworkInterface.get(), &NetworkInterface::sigDisconnected, this, &Multiplayermenu::disconnected, Qt::QueuedConnection);
        connect(m_NetworkInterface.get(), &NetworkInterface::recieveData, this, &Multiplayermenu::recieveData, Qt::QueuedConnection);
        connect(m_pPlayerSelection.get(), &PlayerSelection::sigDisconnect, this, &Multiplayermenu::slotButtonBack, Qt::QueuedConnection);
    }
    else
    {
        oxygine::TextStyle style = FontManager::getMainFont();
        style.color = oxygine::Color(255, 255, 255, 255);
        style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
        style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
        style.multiline = true;

        m_pHostAdresse = new oxygine::TextField();
        addChild(m_pHostAdresse);
        m_pHostAdresse->setStyle(style);
        m_pHostAdresse->setText((tr("Host Adress: ") + NetworkInterface::getIPAdresse()).toStdString().c_str());
        m_pHostAdresse->setPosition(pApp->getSettings()->getWidth() / 2 - m_pHostAdresse->getTextRect().getWidth() / 2,
                                 pApp->getSettings()->getHeight() - m_pHostAdresse->getTextRect().getHeight() - 10);

        m_pHostAdresse->setVisible(false);
    }

}

void Multiplayermenu::playerJoined(quint64 socketID)
{
    if (m_NetworkInterface->getIsServer())
    {
        if (m_pPlayerSelection->hasOpenPlayer())
        {
            QCryptographicHash myHash(QCryptographicHash::Sha3_512);
            QString file = m_currentMapFile.filePath();
            QString fileName = m_currentMapFile.fileName();
            QFile mapFile(file);
            mapFile.open(QIODevice::ReadOnly);
            myHash.addData(&mapFile);
            mapFile.close();
            QByteArray hash = myHash.result();
            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream << QString("MAPINFO");
            stream << fileName;
            stream << hash;
            // send map data to client
            m_NetworkInterface->sendData(socketID, data, NetworkInterface::NetworkSerives::Multiplayer, false);
        }
        else
        {
            // reject connection by disconnecting
            emit dynamic_cast<TCPServer*>(m_NetworkInterface.get())->sigDisconnectClient(socketID);
        }
    }
}

void Multiplayermenu::recieveData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service)
{
    // data for us?
    if (service == NetworkInterface::NetworkSerives::Multiplayer)
    {
        QDataStream stream(&data, QIODevice::ReadOnly);
        QString messageType;
        stream >> messageType;
        if (messageType == "MAPINFO")
        {
            if (!m_NetworkInterface->getIsServer())
            {
                QString fileName;
                stream >> fileName;
                QByteArray hash;
                stream >> hash;
                if (existsMap(fileName, hash))
                {
                    QByteArray data;
                    QDataStream stream(&data, QIODevice::WriteOnly);
                    stream << QString("REQUESTRULE");
                    m_NetworkInterface->sendData(socketID, data, NetworkInterface::NetworkSerives::Multiplayer, false);
                }
                else
                {
                    QByteArray sendData;
                    QDataStream sendStream(&sendData, QIODevice::WriteOnly);
                    sendStream << QString("REQUESTMAP");
                    m_NetworkInterface->sendData(socketID, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
                }
            }
        }
        else if (messageType == "REQUESTRULE")
        {
            if (m_NetworkInterface->getIsServer())
            {
                QByteArray sendData;
                QDataStream sendStream(&sendData, QIODevice::WriteOnly);
                sendStream << QString("SENDINITUPDATE");
                m_pCurrentMap->getGameRules()->serializeObject(sendStream);
                for (qint32 i = 0; i < m_pCurrentMap->getPlayerCount(); i++)
                {
                    BaseGameInputIF::AiTypes aiType = m_pPlayerSelection->getPlayerAiType(i);
                    if (aiType == BaseGameInputIF::AiTypes::Human)
                    {
                        sendStream << Settings::getUsername();
                        sendStream << static_cast<qint32>(BaseGameInputIF::AiTypes::ProxyAi);
                    }
                    else
                    {
                        sendStream << m_pPlayerSelection->getPlayerAiName(i);
                        if (m_pPlayerSelection->isOpenPlayer(i))
                        {
                            sendStream << static_cast<qint32>(BaseGameInputIF::AiTypes::Open);
                        }
                        else
                        {
                            sendStream << static_cast<qint32>(BaseGameInputIF::AiTypes::ProxyAi);
                        }
                    }
                    m_pCurrentMap->getPlayer(i)->serializeObject(sendStream);
                }
                m_NetworkInterface->sendData(socketID, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
            }
        }
        else if (messageType == "SENDINITUPDATE")
        {
            if (!m_NetworkInterface->getIsServer())
            {
                m_pCurrentMap->getGameRules()->deserializeObject(stream);
                for (qint32 i = 0; i < m_pCurrentMap->getPlayerCount(); i++)
                {
                    QString name;
                    qint32 aiType;
                    stream >> name;
                    stream >> aiType;
                    m_pPlayerSelection->setPlayerAiName(i, name);
                    m_pCurrentMap->getPlayer(i)->deserializeObject(stream);
                    m_pCurrentMap->getPlayer(i)->setBaseGameInput(BaseGameInputIF::createAi(static_cast<BaseGameInputIF::AiTypes>(aiType)));
                    m_pPlayerSelection->updatePlayerData(i);
                }
                m_pPlayerSelection->sendPlayerRequest(socketID, -1, BaseGameInputIF::AiTypes::Human);
            }
        }
        else if (messageType == "REQUESTMAP")
        {

        }
    }
}

bool Multiplayermenu::existsMap(QString& fileName, QByteArray& hash)
{
    QString path = QCoreApplication::applicationDirPath() + "/maps";
    QStringList filter;
    filter << "*" + fileName;
    QDirIterator dirIter(path, filter, QDir::Files, QDirIterator::Subdirectories);
    bool found = false;
    while (dirIter.hasNext() && !found)
    {
        dirIter.next();
        QString file = dirIter.fileInfo().absoluteFilePath();
        QFile mapFile(file);
        mapFile.open(QIODevice::ReadOnly);
        QCryptographicHash myHash(QCryptographicHash::Sha3_512);
        myHash.addData(&mapFile);
        mapFile.close();
        QByteArray myHashArray = myHash.result();
        if (hash == myHashArray)
        {
            m_pCurrentMap = new GameMap(file, true);
            m_pCurrentMap->getGameScript()->init();
            m_MapName->setText(m_pCurrentMap->getMapName().toStdString().c_str());
            m_MapAuthor->setText(m_pCurrentMap->getMapAuthor().toStdString().c_str());
            m_MapDescription->setText(m_pCurrentMap->getMapDescription().toStdString().c_str());
            showPlayerSelection();
            found = true;
        }
    }
    return found;

}

void Multiplayermenu::disconnected(quint64 socketID)
{
    if (m_Host)
    {

    }
    else
    {
        if (m_NetworkInterface.get() != nullptr)
        {
            m_NetworkInterface->quit();
            m_NetworkInterface->wait();
            m_NetworkInterface = nullptr;
        }
        Console::print("Leaving Map Selection Menue", Console::eDEBUG);
        oxygine::getStage()->addChild(new LobbyMenu());
        oxygine::Actor::detach();
    }
}

void Multiplayermenu::slotButtonBack()
{
    if (!m_Host || m_MapSelectionStep == MapSelectionStep::selectMap)
    {
        if (m_NetworkInterface.get() != nullptr)
        {
            m_NetworkInterface->quit();
            m_NetworkInterface->wait();
            m_NetworkInterface = nullptr;
        }
        Console::print("Leaving Map Selection Menue", Console::eDEBUG);
        oxygine::getStage()->addChild(new LobbyMenu());
        oxygine::Actor::detach();
    }
    else if (m_Host)
    {
        if (m_NetworkInterface.get() != nullptr)
        {
            m_NetworkInterface->quit();
            m_NetworkInterface->wait();
            m_NetworkInterface = nullptr;
        }
        m_pHostAdresse->setVisible(false);
        MapSelectionMapsMenue::slotButtonBack();
    }
}

void Multiplayermenu::slotButtonNext()
{
    if (m_Host && m_MapSelectionStep == MapSelectionStep::selectRules)
    {
        m_pHostAdresse->setVisible(true);
        m_NetworkInterface = new TCPServer();
        m_pPlayerSelection->attachNetworkInterface(m_NetworkInterface);
        emit m_NetworkInterface->sig_connect("", Settings::getGamePort());
        connect(m_NetworkInterface.get(), &NetworkInterface::sigConnected, this, &Multiplayermenu::playerJoined, Qt::QueuedConnection);
        connect(m_NetworkInterface.get(), &NetworkInterface::recieveData, this, &Multiplayermenu::recieveData, Qt::QueuedConnection);
        connect(m_NetworkInterface.get(), &NetworkInterface::sigDisconnected, this, &Multiplayermenu::disconnected, Qt::QueuedConnection);
    }
    MapSelectionMapsMenue::slotButtonNext();
}

void Multiplayermenu::startGame()
{

}
