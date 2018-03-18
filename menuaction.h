#ifndef MENUACTION_H
#define MENUACTION_H

#include <QString>
#include <QUrl>

class MenuAction
{
public:
    MenuAction(QString, QString);

    QString getTitle();
    QUrl getUrl();
private:
    QString title;
    QUrl action;
};

#endif // MENUACTION_H
