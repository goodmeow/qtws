#include "qtwswebpage.h"

#include <QDesktopServices>
#include <QUrl>
#include <QWebEnginePage>

QtWSWebPage::QtWSWebPage() {

}

QWebEnginePage *QtWSWebPage::createWindow(QWebEnginePage::WebWindowType type) {
    QDesktopServices::openUrl(mLastClickedLink);
    mLastClickedLink = nullptr;
}

bool QtWSWebPage::acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame) {
    qWarning() << url.toString();
    switch(type) {
        case QWebEnginePage::NavigationTypeLinkClicked: {
            mLastClickedLink = url; //-- clear it in WebPage::createWindow
            return false;
        }

        default:
            return QWebEnginePage::acceptNavigationRequest(url, type, isMainFrame);
    }
}
