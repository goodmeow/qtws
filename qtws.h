#ifndef QTWS_H
#define QTWS_H

#include <QString>
#include <QList>
#include "menuaction.h"

class QtWS
{
public:
    QtWS(QString filename);

    QString getName();
    QString getWScope();
    QList<QString> getPlugins();
    QString getHome();
    QString getIconPath();

    QList<MenuAction> getMenu();

    QString getConfigName();

    bool isSaveSession();
    bool isMenuDisabled();

private:
    QString name;
    QString scope;
    QList<QString> plugins;
    QString home;
    QString iconPath;
    bool saveSession;
    bool menuDisabled;

    QList<MenuAction> menu;

    void loadData(QString filename);
};

#endif // QTWS_H
