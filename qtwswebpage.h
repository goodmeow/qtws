#ifndef QTWSWEBPAGE_H
#define QTWSWEBPAGE_H

#include "qtws.h"
#include <QWebEnginePage>
#include <QUrl>

class QtWSWebPage :
        public QWebEnginePage {
public:
    QtWSWebPage(QtWS*);

    virtual QWebEnginePage *createWindow(WebWindowType);
    virtual bool acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame);

private slots:
    void checkPermission(QUrl&, QWebEnginePage::Feature);

private:
    QtWS *configHandler;
};

#endif // QTWSWEBPAGE_H
