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
#include "browser.h"

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

    // key short cuts
    QShortcut* keyF11 = new QShortcut(this); // Initialize the object
    keyF11->setKey(Qt::Key_F11);  // Set the key code
    connect(keyF11, SIGNAL(activated()), this, SLOT(actionFullscreen()));

    QShortcut* keyCtrlQ = new QShortcut(this);         // Initialize the object
    keyCtrlQ->setKey(Qt::CTRL + Qt::Key_Q); // Set the key code
    connect(keyCtrlQ, SIGNAL(activated()), this, SLOT(actionQuit()));

    QShortcut* keyCtrlH = new QShortcut(this);         // Initialize the object
    keyCtrlH->setKey(Qt::CTRL + Qt::Key_H); // Set the key code
    connect(keyCtrlH, SIGNAL(activated()), this, SLOT(actionHome()));

    QShortcut* keyCtrlR = new QShortcut(this);         // Initialize the object
    keyCtrlR->setKey(Qt::CTRL + Qt::Key_R); // Set the key code
    QShortcut* keyF5 = new QShortcut(this);         // Initialize the object
    keyF5->setKey(Qt::Key_F5); // Set the key code
    connect(keyCtrlH, SIGNAL(activated()), this, SLOT(actionReload()));
    connect(keyF5, SIGNAL(activated()), this, SLOT(actionReload()));

    QShortcut* keyAltLeft = new QShortcut(this);         // Initialize the object
    keyAltLeft->setKey(Qt::ALT + Qt::Key_Left); // Set the key code
    connect(keyAltLeft, SIGNAL(activated()), this, SLOT(actionBack()));

    // Window size settings
    QSettings settings;
    restoreState(settings.value("mainWindowState").toByteArray());

    webview->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(webview, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ShowContextMenu(const QPoint &)));
    connect(webview, SIGNAL(urlChanged(QUrl)), this, SLOT(onUrlChanged(QUrl)));
    connect(webview, SIGNAL(Browser::newWindowOpen(QUrl)), this, SLOT(MainWindow::newWindowOpen(QUrl)));
    connect(webview, SIGNAL(iconChanged(QIcon)), this, SLOT(MainWindow::changeIcon(QIcon)));

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

void MainWindow::ShowContextMenu(const QPoint &pos) // this is a slot
{
    QPoint globalPos = webview->mapToGlobal(pos);

    QMenu myMenu;
//    if (this->webview->backingStore()->size() > 0) {
    myMenu.addAction("Back");
//    } else {
//        myMenu.addAction("Back");
//        myMenu.
//    }
    myMenu.addAction("Home");
    myMenu.addAction("Reload");
    myMenu.addSeparator();
    myMenu.addAction("Quit");

    QAction *selectedItem = myMenu.exec(globalPos);
    if (selectedItem == NULL) {
    return;
    } else if (selectedItem->text() == "Reload") {
        actionReload();
    } else if (selectedItem->text() == "Back") {
        actionBack();
    } else if (selectedItem->text() == "Home") {
        actionHome();
    } else if (selectedItem->text() == "Quit") {
        actionQuit();
    }
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
    this->webview->setUrl(this->configHandler->getHome());
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
