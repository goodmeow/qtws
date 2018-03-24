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
#include "browser.h"
#include "menuaction.h"

MainWindow::MainWindow(QWidget *parent, QtWS *configHandler)
    : QMainWindow(parent), ui(new Ui::MainWindow)

{
    this->configHandler = configHandler;

    QWebEngineSettings::globalSettings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    appSettings = new QSettings(configHandler->getConfigName(), "Save State", this);
    ui->setupUi(this);

    readSettings();
    webview = new Browser;
    ui->horizontalLayout->addWidget(webview);
    if (appSettings->value("site").toString() == "") {
        webview->setUrl(QUrl(configHandler->getHome()));
    } else {
        webview->setUrl(QUrl(appSettings->value("site").toString()));
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

    QShortcut* keyAltLeft = new QShortcut(this);         // Initialize the object
    keyAltLeft->setKey(Qt::ALT + Qt::Key_Left); // Set the key code
    connect(keyAltLeft, SIGNAL(activated()), this, SLOT(actionBack()));

    // Window size settings
    QSettings settings;
    restoreState(settings.value("mainWindowState").toByteArray());

    webview->setContextMenuPolicy(Qt::CustomContextMenu);
    if (!configHandler->isMenuDisabled())
        connect(webview, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ShowContextMenu(const QPoint &)));

    connect(webview, SIGNAL(urlChanged(QUrl)), this, SLOT(onUrlChanged(QUrl)));
    connect(webview, SIGNAL(newWindow(QUrl)), this, SLOT(newWindowOpen(QUrl)));
    connect(webview, SIGNAL(iconChanged(QIcon)), this, SLOT(changeIcon(QIcon)));

    connect(webview->page(), &QWebEnginePage::fullScreenRequested, this, &MainWindow::fullScreenRequested);

    this->window()->setWindowTitle(this->configHandler->getName());
    this->setWindowIcon(QIcon(this->configHandler->getIconPath()));

}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::closeEvent(QCloseEvent *) {
    // This will be called whenever this window is closed.
    writeSettings();
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

  // fullscreen on video players

  if (request.toggleOn()) {
    this->showFullScreen();
  } else {
    this->showNormal();
  }
  request.accept();
}

void MainWindow::ShowContextMenu(const QPoint &pos) {

    QPoint globalPos = webview->mapToGlobal(pos);

    QMenu myMenu;

    myMenu.addAction(QString("Back"), this, &MainWindow::actionBack);

    if (!webview->page()->history()->canGoBack()) {
        myMenu.actions().at(0)->setEnabled(false);
    }

    myMenu.addAction(QString("Reload"), this, &MainWindow::actionReload);
    myMenu.addAction(QString("Home"), this, &MainWindow::actionHome);

    if (configHandler->getMenu().size() > 0) {
        myMenu.addSeparator();

        for (int i = 0; i < configHandler->getMenu().size(); i++) {
            MenuAction action = configHandler->getMenu().at(i);
            QAction* menuAction = myMenu.addAction(action.getTitle());
            menuAction->setData(action.getUrl());
        }
    }

    connect(&myMenu, SIGNAL(triggered(QAction*)), this, SLOT(actionMenuTrigger(QAction*)));

    //TODO insert all the custom actions here
    myMenu.addSeparator();
    myMenu.addAction(QString("Quit"), this, &MainWindow::actionQuit);

    myMenu.exec(globalPos);
}


// Slot handler of F11
void MainWindow::actionFullscreen() {
    /* This handler will make switching applications in full screen mode
    * and back to normal window mode
    * */
    if (this->isFullScreen()) {
        this->showNormal();
    } else {
        this->showFullScreen();
    }
}

void MainWindow::onUrlChanged(QUrl url) {
    //QString fileChosen = QFileDialog::getSaveFileName(this);

    qWarning() << url.toString().toLatin1();
    for (int i = 0; i < this->configHandler->getWScope().size(); i++) {
        QString scopeUrl = this->configHandler->getWScope().at(i);
        QUrl allowedScope = QUrl(scopeUrl);

        if (url.matches(allowedScope,
                        QUrl::RemovePassword |
                        QUrl::RemoveFilename |
                        QUrl::RemovePath |
                        QUrl::RemoveQuery |
                        QUrl::RemoveUserInfo |
                        QUrl::RemoveFragment |
                        QUrl::RemoveAuthority |
                        QUrl::RemoveScheme))
            return;
    }

    QDesktopServices::openUrl(url);
    this->webview->stop();
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

void MainWindow::actionMenuTrigger(QAction* action) {
    if (!action->data().isNull()) {
        this->webview->setUrl(action->data().toUrl());
    }
}

void MainWindow::newWindowOpen(QUrl url) {
    qWarning() << url.toString().toLatin1();
    for (int i = 0; i < this->configHandler->getWScope().size(); i++) {
        QString scopeUrl = this->configHandler->getWScope().at(i);
        QUrl allowedScope = QUrl(scopeUrl);

        if (url.matches(allowedScope,
                        QUrl::RemovePassword |
                        QUrl::RemoveFilename |
                        QUrl::RemovePath |
                        QUrl::RemoveQuery |
                        QUrl::RemoveUserInfo |
                        QUrl::RemoveFragment |
                        QUrl::RemoveAuthority |
                        QUrl::RemoveScheme)) {
            this->webview->setUrl(url);
            return;
        }
    }

    QDesktopServices::openUrl(url);
    this->webview->stop();
}

void MainWindow::changeIcon(QIcon icon) {
//    this->setWindowIcon(icon);
}
