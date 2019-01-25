#ifndef MENUACTION_H
#define MENUACTION_H

#include <QString>
#include <QUrl>
#include <QIcon>

class MenuAction
{
public:
    MenuAction(QString, QString, bool);
    MenuAction(QString, QString, QString, bool);
    ~MenuAction();

    QString getTitle();
    QUrl getUrl();
    bool hasIcon();
    QIcon getIcon();
    bool hasSeparator();
private:
    QString title;
    QUrl action;
    QString icon;
    bool separator;
};

#endif // MENUACTION_H
