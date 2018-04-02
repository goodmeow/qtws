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
private:
    QtWS *configHandler;
};

#endif // QTWSWEBPAGE_H
