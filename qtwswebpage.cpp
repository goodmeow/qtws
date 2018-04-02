#include "qtwswebpage.h"
#include "qtws.h"

#include <QDesktopServices>
#include <QUrl>
#include <QWebEnginePage>

QtWSWebPage::QtWSWebPage(QtWS *config) {
    configHandler = config;
}

QWebEnginePage *QtWSWebPage::createWindow(QWebEnginePage::WebWindowType type) {
    QWebEnginePage *fakePage = new QWebEnginePage(this);

    connect(fakePage, &QWebEnginePage::urlChanged, this, [this, fakePage](QUrl url) {
        if (!this->configHandler->isInScope(url)) {
            QDesktopServices::openUrl(url);
        } else {
            this->setUrl(url);
        }

        fakePage->triggerAction(QWebEnginePage::RequestClose);
    });

    connect(fakePage, &QWebEnginePage::windowCloseRequested, this, []() {
        qWarning() << tr("CLOSED!");
    });

    return fakePage;
}
