#include "qtws.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QFile>
#include <QDebug>
#include <QRegExp>
#include <QStringList>

using namespace std;

QtWS::QtWS(QString filename) {
    this->multimedia    = false;
    this->menuDisabled  = false;
    this->download      = false;
    this->alwaysOnTop   = false;
    this->cacheMB       = 50;
    this->permissions.clear();

    this->loadData(filename);
}

QList<QString> QtWS::getWScope() {
    return scope;
}

bool QtWS::isInScope(QUrl url) {
    for (int i = 0; i < scope.size(); i++) {
        QString scopeUrl = this->getWScope().at(i);

        QRegExp regex(scopeUrl);

        QString urlBase = url.toString();

        if (regex.indexIn(urlBase) != -1)
            return true;
    }

    return false;
}

QString QtWS::getHome() {
    return home;
}

QString QtWS::getIconPath() {
    return iconPath;
}

QList<MenuAction> QtWS::getMenu() {
      return this->menu;
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

bool QtWS::isAlwaysOnTop() {
    return this->alwaysOnTop;
}

bool QtWS::isMenuDisabled() {
    return this->menuDisabled;
}

bool QtWS::hasMultimedia() {
    return this->multimedia;
}

bool QtWS::canDownload() {
    return this->download;
}

int QtWS::getCacheMB() {
    return this->cacheMB;
}

bool QtWS::hasPermission(QWebEnginePage::Feature permissionId) {
    for (int i = 0; i < this->permissions.size(); i++) {
        if (this->permissions.at(i) == (int)permissionId) {
            return true;
        }
    }

    return false;
}

QString QtWS::getUserReadablePermissions() {
    return this->userReadablePermissions.join(QString(", "));
}


void QtWS::loadData(QString filename) {
    QFile file;
    file.setFileName(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!file.exists()) {
        throw QString("Configuration file ") + filename + QString(" not found");
    }
    QString content = file.readAll();
    file.close();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(content.toUtf8());
    QJsonObject jsonObject = jsonDocument.object();

    QJsonValue titleInJson = jsonObject.value(QString("name"));
    if (!titleInJson.isString()) {
        throw QString("The title is not a string");
    } else {
        if (this->name.contains(QString("/")) || this->name.contains("\\"))
            throw QString("Illegal name " + this->name + ": it cannot contain slashes or backslashes.");
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
    if (!iconInJson.isString()) {
        throw QString("The icon is not a string");
        return;
    } else {
        this->iconPath = iconInJson.toString();
    }

    QJsonValue sessionInJson = jsonObject.value(QString("saveSession"));
    if (!sessionInJson.isBool()) {
        throw QString("Save session is not boolean");
    } else {
        this->saveSession = sessionInJson.toBool();
    }

    QJsonValue cacheInJson = jsonObject.value(QString("cacheMB"));
    if (!cacheInJson.isUndefined()) {
        if (!cacheInJson.isDouble()) {
            throw QString("The cache is not a number");
            return;
        } else {
            this->cacheMB = iconInJson.toInt(50);
        }
    }

    QJsonValue alwaysOnTopVal = jsonObject.value(QString("alwaysOnTop"));
    if (!alwaysOnTopVal.isBool()) {
        this->alwaysOnTop = false;
    } else {
        this->alwaysOnTop = alwaysOnTopVal.toBool();
    }

    QJsonValue menuDisabledJson = jsonObject.value(QString("menuDisabled"));
    if (!menuDisabledJson.isUndefined()) {
        if (!menuDisabledJson.isBool()) {
            throw QString("Menu disabling is not boolean");
        } else {
            this->menuDisabled = menuDisabledJson.toBool();
        }
    }

    QJsonValue multimediaJson = jsonObject.value(QString("multimedia"));
    if (!multimediaJson.isUndefined()) {
        if (!multimediaJson.isBool()) {
            throw QString("Multimedia is not boolean");
        } else {
            this->multimedia = multimediaJson.toBool();
        }
    }

    QJsonValue downloadJson = jsonObject.value(QString("download"));
    if (!downloadJson.isUndefined()) {
        if (!downloadJson.isBool()) {
            throw QString("Download is not boolean");
        } else {
            this->download = downloadJson.toBool();
        }
    }

    QJsonValue permissionsInJson = jsonObject.value(QString("permissions"));
    if (!permissionsInJson.isUndefined()) {
        this->permissions.clear();
        if (!permissionsInJson.isArray()) {
            throw QString("Permissions is not an array");
        } else {
            QJsonArray permissionsArray = permissionsInJson.toArray();
            for (int i = 0; i < permissionsArray.size(); i++) {
                QJsonValue permissionItem = permissionsArray.at(i);
                if (!permissionItem.isString()) {
                    throw QString("All the permissions have to be strings");
                }

                QString permissionRequested = permissionItem.toString();

                if      (permissionRequested == QString("Notifications")) {
                    this->permissions.append(0);
                    this->userReadablePermissions.append(QString("Notifications"));
                } else if (permissionRequested == QString("Geolocation")) {
                    this->permissions.append(1);
                    this->userReadablePermissions.append(QString("Geolocation"));
                } else if (permissionRequested == QString("MediaAudioCapture")) {
                    this->permissions.append(2);
                    this->userReadablePermissions.append(QString("Microphone"));
                } else if (permissionRequested == QString("MediaVideoCapture")) {
                    this->permissions.append(3);
                    this->userReadablePermissions.append(QString("Camera"));
                } else if (permissionRequested == QString("MediaAudioVideoCapture")) {
                    this->permissions.append(4);
                    this->userReadablePermissions.append(QString("Microphone+Camera"));
                } else if (permissionRequested == QString("MouseLock")) {
                    this->permissions.append(5);
                    this->userReadablePermissions.append(QString("Mouse lock"));
                } else if (permissionRequested == QString("DesktopVideoCapture")) {
                    this->permissions.append(6);
                    this->userReadablePermissions.append(QString("Desktop video capture"));
                } else if (permissionRequested == QString("DesktopAudioVideoCapture")) {
                    this->permissions.append(7);
                    this->userReadablePermissions.append(QString("Desktop video+audio capture"));
                } else
                    throw QString("Invalid permission \"") + permissionRequested + QString("\"");
            }
        }
    }

    QJsonValue menuInJson = jsonObject.value(QString("menu"));
    if (!menuInJson.isUndefined()) {
        this->menu.clear();
        if (!menuInJson.isArray()) {
            throw QString("Menu is not an array");
        } else {
            QJsonArray menuArray = menuInJson.toArray();
            for (int i = 0; i < menuArray.size(); i++) {
                QJsonValue element = menuArray.at(i);
                if (!element.isObject()) {
                    throw QString("Menu item is not an object");
                } else {
                    QJsonObject menuItem = element.toObject();
                    QJsonValue menuItemName     = menuItem.value(QString("title"));
                    QJsonValue menuItemAction   = menuItem.value(QString("action"));
                    QJsonValue menuItemIcon     = menuItem.value(QString("icon"));

                    if (!menuItemName.isString()) {
                        throw QString("Menu item does not have a title");
                    } else if (!menuItemAction.isString()) {
                        throw QString("Menu item does not have an action");
                    } else if (!menuItemIcon.isString() && !menuItemIcon.isUndefined()) {
                        throw QString("Menu item does not have a string icon name");
                    } else {
                        if (menuItemIcon.isUndefined()) {
                            MenuAction action(menuItemName.toString(), menuItemAction.toString());
                            this->menu.append(action);
                        } else {
                            MenuAction action(menuItemName.toString(), menuItemAction.toString(), menuItemIcon.toString());
                            this->menu.append(action);
                        }
                    }
                }
            }
        }
    }
}

QString QtWS::getConfigName() {
    return QString("qtws_") + this->name;
}
