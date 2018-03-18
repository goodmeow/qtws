#include "qtws.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QFile>

using namespace std;

QtWS::QtWS(QString filename) {
    this->loadData(filename);
}

QString QtWS::getWScope() {
    return scope;
}

QString QtWS::getHome() {
    return home;
}

QString QtWS::getIconPath() {
    return iconPath;
}

QList<QString> QtWS::getPlugins() {
    return plugins;
}

bool QtWS::isSaveSession() {
    return saveSession;
}

QString QtWS::getName() {
    return name;
}

void QtWS::loadData(QString filename) {
    QFile file;
    file.setFileName(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString content = file.readAll();
    file.close();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(content.toUtf8());
    QJsonObject jsonObject = jsonDocument.object();

    QJsonValue titleInJson = jsonObject.value(QString("name"));
    if (!titleInJson.isString()) {
        throw QString("The title is not a string");
    } else {
        this->name = titleInJson.toString();
    }

    QJsonValue scopeInJson = jsonObject.value(QString("scope"));
    if (!scopeInJson.isArray()) {
        throw QString("The scope is not an array");
        return;
    } else {
        this->scope.clear();
        QJsonArray array = scopeInJson.toArray();
        for (int i = 0; i < array.size(); i++) {
            QJsonValue scopePart = array.at(i);
            if (!scopePart.isString()) {
                throw QString("One of the scopes is not a string");
                return;
            } else {
                this->scope.append(scopePart.toString());
            }
        }
    }

    QJsonValue pluginsInJson = jsonObject.value(QString("plugins"));
    if (!pluginsInJson.isArray()) {
        throw QString("The plugins are not an array");
        return;
    } else {
        this->plugins.clear();
        QJsonArray array = pluginsInJson.toArray();
        for (int i = 0; i < array.size(); i++) {
            QJsonValue plugin = array.at(i);
            if (!plugin.isString()) {
                throw QString("One of the plugins is not a string");
                return;
            } else {
                this->plugins.append(plugin.toString());
            }
        }
    }

    QJsonValue homeInJson = jsonObject.value(QString("home"));
    if (!homeInJson.isString()) {
        throw QString("The home is not a string");
        return;
    } else {
        this->home = homeInJson.toString();
    }

    QJsonValue iconInJson = jsonObject.value(QString("icon"));
    if (!titleInJson.isString()) {
        throw QString("The icon is not a string");
        return;
    } else {
        this->iconPath = iconInJson.toString();
    }

    QJsonValue sessionInJson = jsonObject.value(QString("saveSession"));
    if (!sessionInJson.isBool()) {
        throw QString("Save session is not boolean");
        return;
    } else {
        this->saveSession = sessionInJson.toBool();
    }
}

QString QtWS::getConfigName() {
    return QString("qtws_") + this->name;
}
