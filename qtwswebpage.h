#ifndef QTWSWEBPAGE_H
#define QTWSWEBPAGE_H

#include "qtws.h"
#include <QWebEnginePage>
#include <QUrl>

class QtWSWebPage : public QWebEnginePage {
public:
    QtWSWebPage(QtWS*);

    virtual QWebEnginePage *createWindow(WebWindowType);

    bool acceptNavigationRequest(const QUrl&, NavigationType, bool);

private:
    QUrl mLastClickedLink;
    QtWS *configHandler;

private Q_SLOTS:
    void onLinkHovered(const QString &);
};

#endif // QTWSWEBPAGE_H
