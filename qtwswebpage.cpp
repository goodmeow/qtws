#include "qtwswebpage.h"
#include "qtws.h"

#include <QDesktopServices>
#include <QUrl>
#include <QWebEnginePage>

QtWSWebPage::QtWSWebPage(QtWS *config) {
    configHandler = config;

    connect(this, &QWebEnginePage::linkHovered, this, &QtWSWebPage::onLinkHovered);
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

bool QtWSWebPage::acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame) {
//    switch(type) {
//        case QWebEnginePage::NavigationTypeLinkClicked: {
//            if (this->configHandler->isInScope(url)) {
//                return true;
//            } else {
//                if (isMainFrame) {
//                    qWarning() << "Opened from acceptNavigationRequest";
//                    QDesktopServices::openUrl(mLastClickedLink);
//                    return false;
//                } else {
//                    mLastClickedLink = url;
//                    return true;
//                }
//            }
//        }

//        default:
//            mLastClickedLink = url;
//            return true;
//    }
    return QWebEnginePage::acceptNavigationRequest(url, type, isMainFrame);
}

void QtWSWebPage::onLinkHovered(const QString &link) {
    mLastClickedLink = QUrl(link);
}
