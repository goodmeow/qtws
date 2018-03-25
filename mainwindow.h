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
#include "qtws.h"
#include "browser.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0, QtWS *config = NULL);
  ~MainWindow();
  // QAction amazon();

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

  void onUrlChanged(QUrl url);
  void newWindowOpen(QUrl url);
  void changeIcon(QIcon icon);

protected:
  // save window geometry
  void closeEvent(QCloseEvent *);

private:
  Ui::MainWindow *ui;
  Browser *webview;
  bool maximized;
//  QShortcut *keyF11;   // Entity of F11 hotkey
//  QShortcut *keyCtrlQ; // Entity of Ctrl + Q hotkey
//  QShortcut *keyAltLeft; // Entity of Back
//  QShortcut *keyCtrlH; // Entity of Ctrl + H hotkey
//  QShortcut *keyF5; // Entity of F5 hotkey
//  QShortcut *keyCtrlR; // Entity of Ctrl + R hotkey
  QSettings *appSettings;

  QtWS *configHandler;

  void fullScreenRequested(QWebEngineFullScreenRequest request);
  void writeSettings();
  void readSettings();
  void restore();
};

#endif // MAINWINDOW_H
