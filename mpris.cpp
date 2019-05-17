#include "mpris.h"
#include <QDBusConnection>
#include <QDBusMessage>

static inline qlonglong convertTime(int t) {
    return t*1000000;
}


Mpris::Mpris(QObject *p, QString name)
    : QObject(p), pos(-1) {

    this->serviceName = name;

    QDBusConnection::sessionBus().registerService("org.mpris.MediaPlayer2." + name.toLatin1());

    QDBusConnection::sessionBus().unregisterService("org.mpris.MediaPlayer2." + name.toLatin1());


//    QDBusConnection::connectToBus()
//    QDBusConnection::sessionBus().registerObject("/org/mpris/MediaPlayer2", this, QDBusConnection::ExportAdaptors);
}

Mpris::~Mpris() {
    QDBusConnection::sessionBus().unregisterService("org.mpris.MediaPlayer2." + this->serviceName.toLatin1());
}

void Mpris::Pause() {
    qDebug()<<"pause";

}

void Mpris::Play() {
    qDebug()<<"play";
}

QString Mpris::PlaybackStatus() const {
    qDebug()<<"PlaybackStatus";
    return "PlaybackStatus";
}

qlonglong Mpris::Position() const {
    // Cant use MPDStatus, as we dont poll for track position, but use a timer instead!
    //return MPDStatus::self()->timeElapsed();
    return 1000000000;
}

void Mpris::updateStatus() {
    QVariantMap map;
    qDebug()<<"updateStatus";


}

void Mpris::updateCurrentCover(const QString &fileName) {
    if (fileName!=currentCover) {
        currentCover=fileName;
        signalUpdate("Metadata", Metadata());
    }
}

void Mpris::updateCurrentSong() {
    qDebug()<<"updateCurrentSong";

}

QVariantMap Mpris::Metadata() const {
    QVariantMap metadataMap;

    return metadataMap;
}

void Mpris::Raise() {
    emit showMainWindow();
}

void Mpris::signalUpdate(const QString &property, const QVariant &value) {
    QVariantMap map;
    map.insert(property, value);
    signalUpdate(map);
}

void Mpris::signalUpdate(const QVariantMap &map) {
    if (map.isEmpty()) {
        return;
    }
    QDBusMessage signal = QDBusMessage::createSignal("/org/mpris/MediaPlayer2",
                                                     "org.freedesktop.DBus.Properties",
                                                     "PropertiesChanged");
    QVariantList args = QVariantList()
            << "org.mpris.MediaPlayer2.Player"
            << map
            << QStringList();
    signal.setArguments(args);
    QDBusConnection::sessionBus().send(signal);
}
