#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QContextMenuEvent>
#include <QMenu>
#include <QSettings>
#include <QWebEngineFullScreenRequest>
#include <QWebEngineSettings>
#include <QWebEngineView>
#include <QWidget>
#include <QIcon>
#include <QBackingStore>
#include <QDesktopServices>
#include <QWebEngineHistory>
#include <QFileDialog>
#include <QWebEngineProfile>
#include <QProgressBar>
#include <QCloseEvent>
#include <QIcon>
#include "menuaction.h"
#include "qtwswebpage.h"

#ifdef DBUS
#include <QtDBus/QDBusConnection>
#include <QtDBus/QtDBus>
#endif

MainWindow::MainWindow(QWidget *parent, QtWS *configHandler)
    : QMainWindow(parent), ui(new Ui::MainWindow)

{    
    this->configHandler = configHandler;

    this->mpris = new Mpris(this, configHandler->getName());

    QWebEngineSettings::globalSettings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    appSettings = new QSettings(configHandler->getConfigName(), "Save State", this);
    ui->setupUi(this);

    readSettings();
    webview = new QWebEngineView;
    webview->setPage(new QtWSWebPage(configHandler));
    ui->horizontalLayout->addWidget(webview);
    if (appSettings->value("site").toString() == "") {
        webview->setUrl(QUrl(configHandler->getHome()));
    } else {
        webview->setUrl(QUrl(appSettings->value("site").toString()));
    }

    if (this->configHandler->canDownload()) {
        progressBar = new QProgressBar(this);
        ui->horizontalLayout->addWidget(progressBar);
        progressBar->hide();
    }

    webview->settings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true);
    webview->settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);

    QShortcut* keyF11 = new QShortcut(this);
    keyF11->setKey(Qt::Key_F11);
    connect(keyF11, SIGNAL(activated()), this, SLOT(actionFullscreen()));

    QShortcut* keyCtrlQ = new QShortcut(this);
    keyCtrlQ->setKey(Qt::CTRL + Qt::Key_Q);
    connect(keyCtrlQ, SIGNAL(activated()), this, SLOT(actionQuit()));

    QShortcut* keyCtrlH = new QShortcut(this);
    keyCtrlH->setKey(Qt::CTRL + Qt::Key_H);
    connect(keyCtrlH, SIGNAL(activated()), this, SLOT(actionHome()));

    QShortcut* keyCtrlR = new QShortcut(this);
    keyCtrlR->setKey(Qt::CTRL + Qt::Key_R);
    connect(keyCtrlR, SIGNAL(activated()), this, SLOT(actionReload()));

    QShortcut* keyF5 = new QShortcut(this);
    keyF5->setKey(Qt::Key_F5);
    connect(keyF5, SIGNAL(activated()), this, SLOT(actionReload()));

    if (configHandler->hasMultimedia()) {
        QShortcut* keyCtrlM = new QShortcut(this);
        keyCtrlM->setKey(Qt::CTRL + Qt::Key_M);
        connect(keyCtrlM, SIGNAL(activated()), this, SLOT(actionToggleMute()));

        QShortcut* keyCtrlP = new QShortcut(this);
        keyCtrlP->setKey(Qt::CTRL + Qt::Key_M);
        connect(keyCtrlP, SIGNAL(activated()), this, SLOT(actionTogglePlay()));
    }

    QShortcut* keyAltLeft = new QShortcut(this);         // Initialize the object
    keyAltLeft->setKey(Qt::ALT + Qt::Key_Left); // Set the key code
    connect(keyAltLeft, SIGNAL(activated()), this, SLOT(actionBack()));

    // Window size settings
    QSettings settings;
    restoreState(settings.value("mainWindowState").toByteArray());

    if (this->configHandler->isMenuDisabled()) {
        webview->setContextMenuPolicy(Qt::NoContextMenu);
    } else {
        webview->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(webview, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ShowContextMenu(const QPoint &)));
    }

    if (this->configHandler->canDownload()) {
        connect(QWebEngineProfile::defaultProfile(), SIGNAL(downloadRequested(QWebEngineDownloadItem*)), this, SLOT(downloadRequested(QWebEngineDownloadItem*)));
    }

    connect(webview->page(), &QWebEnginePage::fullScreenRequested, this, &MainWindow::fullScreenRequested);

    this->window()->setWindowTitle(this->configHandler->getName());
    this->setWindowIcon(QIcon(this->configHandler->getIconPath()));

#ifdef DBUS
    if (configHandler->hasMultimedia()) {
        bool ret;

        const QString serviceName = tr("it.qtws.services.") + configHandler->getName();
        const QString interfaceName = tr("it.qtws.") + configHandler->getName();

        QDBusConnection::sessionBus().registerService(serviceName);
        QDBusConnection::sessionBus().registerObject(tr("/"), this);

        ret = QDBusConnection::sessionBus().connect(
                    QString(),
                    QString(),
                    interfaceName,
                    tr("Play"),
                    tr(""),
                    this, //receiver
                    SLOT(dServicePlay()));
        if (!ret)
            qWarning() << tr("Unregistered Play");

        ret = QDBusConnection::sessionBus().connect(
                    QString(),
                    QString(),
                    interfaceName,
                    tr("Pause"),
                    tr(""),
                    this, //receiver
                    SLOT(dServicePause()));
        if (!ret)
            qWarning() << tr("Unregistered Play");

        ret = QDBusConnection::sessionBus().connect(
                    QString(),
                    QString(),
                    interfaceName,
                    tr("Stop"),
                    tr(""),
                    this, //receiver
                    SLOT(dServiceStop()));
        if (!ret)
            qWarning() << tr("Unregistered Play");
    }
#endif
}

MainWindow::~MainWindow() {
    delete ui;
#ifdef DBUS
    const QString interfaceName = tr("it.qtws.") + configHandler->getName();

    QDBusConnection::sessionBus().unregisterService(interfaceName);
    QDBusConnection::sessionBus().unregisterObject(tr("/"));
#endif
}

void MainWindow::gotoUrl(QUrl url) {
    if (this->configHandler->isInScope(url))
        this->webview->setUrl(url);
    else
        QMessageBox::warning(this, tr("Invalid URL"), tr("The requested URL is not in the scope of this web application."), QMessageBox::Ok);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    // This will be called whenever this window is closed.
    writeSettings();
    
    delete this->webview->page();
    delete this->webview;
    
    event->accept();
    delete ui;
}

void MainWindow::writeSettings() {
    if (!this->configHandler->isSaveSession())
        return;

    // Write the values to disk in categories.
    appSettings->setValue("state/mainWindowState", saveState());
    appSettings->setValue("geometry/mainWindowGeometry", saveGeometry());
    QString site = webview->url().toString();
    appSettings->setValue("site", site);
    qDebug() << " write settings:" << site;
}

void MainWindow::restore() {

  QByteArray stateData =
      appSettings->value("state/mainWindowState").toByteArray();

  QByteArray geometryData =
      appSettings->value("geometry/mainWindowGeometry").toByteArray();

  restoreState(stateData);
  restoreGeometry(geometryData);
}

void MainWindow::readSettings() {
    if (this->configHandler->isSaveSession())
        restore();
}

void MainWindow::fullScreenRequested(QWebEngineFullScreenRequest request) {
    if (request.toggleOn()) {
        maximized = this->isMaximized();

        this->showFullScreen();
    } else {
        this->webview->triggerPageAction(QWebEnginePage::ExitFullScreen);
//        this->showNormal();

//        if (maximized)
//            this->showMaximized();
    }
    request.accept();
}

void MainWindow::ShowContextMenu(const QPoint &pos) {

    QPoint globalPos = webview->mapToGlobal(pos);

    QMenu *myMenu = new QMenu();

    myMenu->addAction(QIcon::fromTheme(QString("back")), QString("Back"), this, &MainWindow::actionBack);

    if (!webview->page()->history()->canGoBack()) {
        myMenu->actions().at(0)->setEnabled(false);
    }

    myMenu->addAction(QIcon::fromTheme(QString("reload")), QString("Reload"), this, &MainWindow::actionReload);
    myMenu->addAction(QIcon::fromTheme(QString("go-home")), QString("Home"), this, &MainWindow::actionHome);

    if (configHandler->hasMultimedia()) {
        myMenu->addSeparator();
        if (webview->page()->isAudioMuted())
            myMenu->addAction(QIcon::fromTheme(QString("audio-volume-muted")), QString("Unmute"), this, &MainWindow::actionToggleMute);
        else
            myMenu->addAction(QIcon::fromTheme(QString("audio-volume-high")), QString("Mute"), this, &MainWindow::actionToggleMute);

        myMenu->addAction(QIcon::fromTheme(QString("media-playback-start")), QString("Play/Pause"), this, &MainWindow::actionTogglePlay);
    }

    if (configHandler->getMenu().size() > 0) {
        myMenu->addSeparator();

        for (int i = 0; i < configHandler->getMenu().size(); i++) {
            MenuAction action = configHandler->getMenu().at(i);

            QAction* menuAction;
            if (action.hasIcon())
                menuAction = myMenu->addAction(action.getIcon(), action.getTitle());
            else
                menuAction = myMenu->addAction(action.getTitle());

            menuAction->setData(action.getUrl());
        }
    }

    connect(myMenu, SIGNAL(triggered(QAction*)), this, SLOT(actionMenuTrigger(QAction*)));

    //TODO insert all the custom actions here
    myMenu->addSeparator();
    myMenu->addAction(QIcon::fromTheme(QString("application-exit")), QString("Quit"), this, &MainWindow::actionQuit);

    myMenu->popup(globalPos);
}


// Slot handler of F11
void MainWindow::actionFullscreen() {
    /* This handler will make switching applications in full screen mode
    * and back to normal window mode
    * */
    if (!this->isFullScreen()) {
        maximized = this->isMaximized();

        this->showFullScreen();
    } else {
//        this->webview->triggerPageAction(QWebEnginePage::ExitFullScreen);
        this->showNormal();

        if (maximized)
            this->showMaximized();
    }
}

// Slot handler for Ctrl + Q
void MainWindow::actionQuit() {
    writeSettings();
    QApplication::quit();
}


void MainWindow::actionBack() {
    this->webview->back();
}

void MainWindow::actionHome() {
    this->webview->setUrl(this->configHandler->getHome());
}

void MainWindow::actionReload() {
    this->webview->reload();
}

void MainWindow::actionToggleMute() {
    webview->page()->setAudioMuted(!webview->page()->isAudioMuted());
}

void MainWindow::actionTogglePlay() {
    webview->page()->runJavaScript(QString("x=document.getElementsByTagName(\"video\"); for(i = 0; i < x.length; i++) {if (x[i].paused) {x[i].play()} else {x[i].pause()}};"));
}

void MainWindow::actionMenuTrigger(QAction* action) {
    if (!action->data().isNull()) {
        this->webview->setUrl(action->data().toUrl());
    }
}

void MainWindow::downloadRequested(QWebEngineDownloadItem* download) {
    if (this->configHandler->canDownload()) {
        qDebug() << "Format: " <<  download->savePageFormat();
        qDebug() << "Path: " << download->path();
        // If you want to modify something like the default path or the format
        QString filename = QFileDialog::getSaveFileName(this);
        download->setPath(filename);
        // Check your url to accept/reject the download
        download->accept();

        progressBar->setRange(0, 100);
        progressBar->setValue(0);
        progressBar->show();
        connect(download, &QWebEngineDownloadItem::downloadProgress, this, [this](qint64 received, qint64 total) {
            QString unit;
            qint64 copyReceived = received;
            if (copyReceived < 1024) {
                unit = tr("bytes");
            } else if (copyReceived < 1024*1024) {
                copyReceived /= 1024;
                unit = tr("KB");
            } else {
                copyReceived /= 1024*1024;
                unit = tr("MB");
            }

            this->progressBar->setToolTip(QString::number(copyReceived) + QString(" ") + unit);
            this->progressBar->setStatusTip(QString::number(copyReceived) + QString(" ") + unit);

            if (total > 0) {
                this->progressBar->setValue((int)(received * 100 / total));
            } else {
                this->progressBar->setRange(0, __INT32_MAX__);
                this->progressBar->setValue(copyReceived);
                this->progressBar->setFormat(tr("%v ") + unit);
            }
        });

        connect(download, &QWebEngineDownloadItem::finished, this, [this, download]() {
            this->progressBar->hide();
        });
    }
}

#ifdef DBUS
void MainWindow::dServicePlay() {
    qWarning() << tr("Play");
}

void MainWindow::dServicePause() {
    qWarning() << tr("Pause");
}

void MainWindow::dServiceStop() {
    qWarning() << tr("Stop");
}
#endif
