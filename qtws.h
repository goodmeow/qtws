#ifndef QTWS_H
#define QTWS_H

#include <QString>
#include <QList>
#include "menuaction.h"

class QtWS {
public:
    QtWS(QString filename);

    QString getName();

    QList<QString> getWScope();
    bool isInScope(QUrl);

    QList<QString> getPlugins();
    QString getHome();
    QString getIconPath();

    QList<MenuAction> getMenu();

    QString getConfigName();

    bool isSaveSession();
    bool isAlwaysOnTop();
    bool isMenuDisabled();
    bool hasMultimedia();
    bool canDownload();

private:
    QString name;
    QList<QString> scope;
    QList<QString> plugins;
    QString home;
    QString iconPath;
    bool saveSession;
    bool alwaysOnTop;
    bool menuDisabled;
    bool multimedia;
    bool download;

    QList<MenuAction> menu;

    void loadData(QString filename);
};

#endif // QTWS_H
