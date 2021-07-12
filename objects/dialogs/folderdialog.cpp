#include "objects/dialogs/folderdialog.h"

#include "coreengine/mainapp.h"
#include "coreengine/globalutils.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"
#include "objects/dialogs/dialogmessagebox.h"

const char* const ROOT = "::::";

FolderDialog::FolderDialog(QString startFolder)
{
    setObjectName("FolderDialog");
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("filedialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    // current folder
    m_CurrentFolder = spTextbox::create(Settings::getWidth() - 60);
    m_CurrentFolder->setPosition(30, 30);
    pSpriteBox->addChild(m_CurrentFolder);
    m_CurrentFolder->setCurrentText(startFolder);
    connect(m_CurrentFolder.get(), &Textbox::sigTextChanged, this, &FolderDialog::showFolder, Qt::QueuedConnection);
    // folder file selection
    m_MainPanel = spPanel::create(true, QSize(Settings::getWidth() - 60, Settings::getHeight() - 150), QSize(Settings::getWidth() - 60, Settings::getHeight() - 300));
    m_MainPanel->setPosition(30, 30 + m_CurrentFolder->getHeight() + 10);
    pSpriteBox->addChild(m_MainPanel);

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(Settings::getWidth() - 30 - m_OkButton->getWidth(),
                            Settings::getHeight() - 30 - m_OkButton->getHeight());
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        QDir folder(m_CurrentFolder->getCurrentText());
        QDir virtFolder(oxygine::Resources::RCC_PREFIX_PATH + m_CurrentFolder->getCurrentText());
        if (folder.exists() || virtFolder.exists())
        {
            emit sigFolderSelected(m_CurrentFolder->getCurrentText());
        }
        detach();
    });
    // cancel button
    m_CancelButton = pObjectManager->createButton(tr("Cancel"), 150);
    m_CancelButton->setPosition(30,
                                Settings::getHeight() - 30 - m_CancelButton->getHeight());
    pSpriteBox->addChild(m_CancelButton);
    m_CancelButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigCancel();
        detach();
    });

    // go folder up
    pAnim = pObjectManager->getResAnim("filedialogitems");
    oxygine::spBox9Sprite pBox = oxygine::spBox9Sprite::create();
    pBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pBox->setResAnim(pAnim);
    oxygine::spTextField textField = oxygine::spTextField::create();
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = true;
    textField->setStyle(style);
    textField->setHtmlText("..");
    textField->attachTo(pBox);
    pBox->setSize(m_MainPanel->getWidth() - 50, 40);
    textField->setHeight(40);
    textField->setWidth(pBox->getWidth() - 18);
    textField->setX(8);
    pBox->setPosition(0, 0);
    pBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    m_MainPanel->addItem(pBox);
    // add some event handling :)
    pBox->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        pBox->addTween(oxygine::Sprite::TweenAddColor(QColor(32, 200, 32, 0)), oxygine::timeMS(300));
    });
    pBox->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        pBox->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    pBox->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        QDir dir(m_CurrentFolder->getCurrentText());
        if (m_CurrentFolder->getCurrentText() != "")
        {
            emit sigShowFolder(dir.absolutePath() + "/..");
        }
        else
        {
            emit sigShowFolder(ROOT);
        }
    });
    connect(this, &FolderDialog::sigShowFolder, this, &FolderDialog::showFolder, Qt::QueuedConnection);
    showFolder(startFolder);
    connect(pApp, &Mainapp::sigKeyDown, this, &FolderDialog::KeyInput, Qt::QueuedConnection);
    pApp->continueRendering();
}

FolderDialog::~FolderDialog()
{
}

void FolderDialog::showFolder(QString folder)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    // clean up current panel items
    for (qint32 i = 0; i < m_Items.size(); i++)
    {
        m_MainPanel->removeItem(m_Items[i]);
    }

    folder = folder.replace("\\", "/");
    folder = QDir(folder).absolutePath();
    folder = GlobalUtils::makePathRelative(folder);
    m_Items.clear();
    QDir dir(folder);
    QDir virtDir(oxygine::Resource::RCC_PREFIX_PATH + folder);
    if (!dir.exists() && !virtDir.exists())
    {
        folder = ROOT;
    }

    // this is the we have to do all the work function of the file dialog...
    // we want the root folder
    QFileInfoList infoList;
    if (folder == ROOT)
    {
        infoList = QDir::drives();
    }
    else
    {
        infoList = GlobalUtils::getInfoList(folder);
    }

    qint32 itemCount = 0;
    for (qint32 i = 1; i < infoList.size(); i++)
    {
        QString myPath;
        if (folder == ROOT)
        {
            myPath = infoList[i].absoluteFilePath();
        }
        else if (infoList[i].absoluteFilePath() != QCoreApplication::applicationDirPath() &&
                 infoList[i].absoluteFilePath() != QCoreApplication::applicationDirPath() + "/")
        {
           myPath = GlobalUtils::makePathRelative(infoList[i].absoluteFilePath());
        }
        else
        {
            myPath = infoList[i].absoluteFilePath();
        }
        if (myPath == folder)
        {
            // skip ourself
            continue;
        }
        ObjectManager* pObjectManager = ObjectManager::getInstance();
        oxygine::ResAnim* pAnim = pObjectManager->getResAnim("filedialogitems");
        oxygine::spBox9Sprite pBox = oxygine::spBox9Sprite::create();
        pBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
        pBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
        pBox->setResAnim(pAnim);
        oxygine::spTextField textField = oxygine::spTextField::create();
        oxygine::TextStyle style = FontManager::getMainFont24();
        style.color = FontManager::getFontColor();
        style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
        style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
        style.multiline = false;
        textField->setStyle(style);

        textField->attachTo(pBox);
        pBox->setSize(m_MainPanel->getWidth() - 70, 40);
        textField->setHeight(40);
        textField->setWidth(pBox->getWidth() - 18);
        textField->setX(13);
        textField->setY(5);
        pBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
        m_MainPanel->addItem(pBox);
        // add some event handling :)
        pBox->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
        {
            pBox->addTween(oxygine::Sprite::TweenAddColor(QColor(32, 200, 32, 0)), oxygine::timeMS(300));
        });
        pBox->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
        {
            pBox->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
        });
        pBox->setPosition(0, 40 + itemCount * 40);

        // loop through all entries :)
        if (infoList[i].isDir())
        {
            if (folder == ROOT)
            {
                textField->setHtmlText(infoList[i].absoluteFilePath());
            }
            else
            {
                textField->setHtmlText(infoList[i].baseName());
            }
            pBox->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
            {
                emit sigShowFolder(myPath);
            });
        }
        else
        {
            // not possible i hope
        }
        m_Items.append(pBox);
        itemCount++;
    }
    m_MainPanel->setContentHeigth(itemCount * 40 + 50);
    if (folder == ROOT)
    {
        m_CurrentFolder->setCurrentText("");
    }
    else
    {
        m_CurrentFolder->setCurrentText(folder);
    }
    pApp->continueRendering();
}

void FolderDialog::deleteItem()
{
    QString folder = m_CurrentFolder->getCurrentText();
    QDir dir(folder);
    dir.removeRecursively();
    dir.cdUp();
    showFolder(dir.path());
    m_CurrentFolder->setCurrentText(dir.path());
    m_focused = true;
}

void FolderDialog::KeyInput(oxygine::KeyEvent event)
{
    // for debugging
    Qt::Key cur = event.getKey();
    if (!m_CurrentFolder->getFocused() &&
        m_focused)
    {
        switch(cur)
        {
            case Qt::Key_Delete:
            {
                if (QFile::exists(m_CurrentFolder->getCurrentText()))
                {
                    m_focused = false;
                    spDialogMessageBox pSurrender = spDialogMessageBox::create(tr("Do you want to delete the folder ") + m_CurrentFolder->getCurrentText() + "?", true);
                    connect(pSurrender.get(), &DialogMessageBox::sigOk, this, &FolderDialog::deleteItem, Qt::QueuedConnection);
                    connect(pSurrender.get(), &DialogMessageBox::sigCancel, [=]()
                    {
                        m_focused = true;
                    });
                    addChild(pSurrender);
                }
                break;
            }
            default:
            {
                break;
            }
        }
        
    }
}
