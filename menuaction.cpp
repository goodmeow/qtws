#include "menuaction.h"
#include <QUrl>
#include <QString>

MenuAction::MenuAction(QString title, QString urlStr, bool separator) {
    this->title = title;
    this->action = QUrl(urlStr);
    this->icon = QString("");
    this->separator = separator;
}

MenuAction::~MenuAction() {
}

MenuAction::MenuAction(QString title, QString urlStr, QString iconName, bool separator) {
    this->title = title;
    this->action = QUrl(urlStr);
    this->icon = iconName;
    this->separator = separator;
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

bool MenuAction::hasSeparator() {
    return this->separator;
}
