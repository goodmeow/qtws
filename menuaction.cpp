#include "menuaction.h"
#include <QUrl>
#include <QString>

MenuAction::MenuAction(QString title, QString urlStr) {
    this->title = title;
    this->action = QUrl(urlStr);
    this->icon = QString("");
}

MenuAction::~MenuAction() {
}

MenuAction::MenuAction(QString title, QString urlStr, QString iconName) {
    this->title = title;
    this->action = QUrl(urlStr);
    this->icon = iconName;
}

QString MenuAction::getTitle() {
    return this->title;
}

QIcon MenuAction::getIcon() {
    return QIcon::fromTheme(icon);
}

QUrl MenuAction::getUrl() {
    return this->action;
}

bool MenuAction::hasIcon() {
    return this->icon.length() > 0;
}
