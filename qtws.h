#ifndef QTWS_H
#define QTWS_H

#include <QString>
#include <QList>

class QtWS
{
public:
    QtWS(QString filename);

    QString getName();
    QString getWScope();
    QList<QString> getPlugins();
    QString getHome();
    QString getIconPath();

    QString getConfigName();

    bool isSaveSession();
private:
    QString name;
    QString scope;
    QList<QString> plugins;
    QString home;
    QString iconPath;
    bool saveSession;

    void loadData(QString filename);

};

#endif // QTWS_H
