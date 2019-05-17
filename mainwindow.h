#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QByteArray>
#include <QCommandLineParser>
#include <QMainWindow>
#include <QMessageBox>
#include <QSettings>
#include <QShortcut>
#include <QWebEngineFullScreenRequest>
#include <QWebEngineView>
#include <QProgressBar>
#include "qtws.h"
#include "mpris.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, QtWS *config = NULL, QApplication *app = NULL);
    ~MainWindow();

    void gotoUrl(QUrl);

private slots:
    // slots for handlers of hotkeys
    void actionFullscreen();
    void actionQuit();
    void actionHome();
    void actionBack();
    void actionReload();

    void actionToggleMute();
    void actionTogglePlay();

    void actionMenuTrigger(QAction*);
    void ShowContextMenu(const QPoint &pos);

    void downloadRequested(QWebEngineDownloadItem*);

#ifdef DBUS
    void dServicePlay();
    void dServicePause();
    void dServiceStop();
#endif

protected:
    // save window geometry
    void closeEvent(QCloseEvent* event);

private:
    Ui::MainWindow *ui;
    QWebEngineView *webview;
    bool maximized;
    Mpris *mpris;
//  QShortcut *keyF11;   // Entity of F11 hotkey
//  QShortcut *keyCtrlQ; // Entity of Ctrl + Q hotkey
//  QShortcut *keyAltLeft; // Entity of Back
//  QShortcut *keyCtrlH; // Entity of Ctrl + H hotkey
//  QShortcut *keyF5; // Entity of F5 hotkey
//  QShortcut *keyCtrlR; // Entity of Ctrl + R hotkey
    QSettings *appSettings;
    QtWS *configHandler;

    QProgressBar *progressBar;

    void initializeUI();
    void initializeWebView();
    void initializeMPRIS();
    void initializeShortcuts();

    void fullScreenRequested(QWebEngineFullScreenRequest request);

    void writeSettings();
    void readSettings();

    QApplication *app;
};

#endif // MAINWINDOW_H
