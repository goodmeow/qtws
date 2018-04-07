#ifndef MENUACTION_H
#define MENUACTION_H

#include <QString>
#include <QUrl>
#include <QIcon>

class MenuAction
{
public:
    MenuAction(QString, QString);
    MenuAction(QString, QString, QString);
    ~MenuAction();

    QString getTitle();
    QUrl getUrl();
    bool hasIcon();
    QIcon getIcon();
private:
    QString title;
    QUrl action;
    QString icon;
};

#endif // MENUACTION_H
