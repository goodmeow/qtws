#include "menuaction.h"
#include <QUrl>
#include <QString>

MenuAction::MenuAction(QString title, QString urlStr)
{
    this->title = title;
    this->action = QUrl(urlStr);
}

QString MenuAction::getTitle()
{
    return this->title;
}

QUrl MenuAction::getUrl()
{
    return this->action;
}
