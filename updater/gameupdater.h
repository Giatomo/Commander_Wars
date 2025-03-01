#pragma once

#include <QObject>
#include "3rd_party/oxygine-framework/oxygine/core/intrusive_ptr.h"
#include "updater/filedownloader.h"
#include "updater/zipSupport/qzipreader.h"

class GameUpdater;
using spGameUpdater = oxygine::intrusive_ptr<GameUpdater>;

class GameUpdater final : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    static const char* const MODE_FORCE;
    static const char* const MODE_INSTALL;

    explicit GameUpdater();
    ~GameUpdater() = default;
    static void launchPatcher();
    static void launchApplication();
    static void cleanUpOldArtifacts();
public slots:
    void onNewState(FileDownloader::State state);
    void onNewProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onExtractProgress(qint32 progress);
private:
    void continueBooting();
    void finishDownload();
    void install();
private:
    FileDownloader m_filedownloader;
    QFile m_downloadFile;
    QZipReader m_zipReader;
    bool m_continued{false};
};

