#ifndef QTWSWEBPAGE_H
#define QTWSWEBPAGE_H

#include <QWebEnginePage>
#include <QUrl>

class QtWSWebPage : public QWebEnginePage {
public:
    QtWSWebPage();

    virtual QWebEnginePage *createWindow(WebWindowType);

    bool acceptNavigationRequest(const QUrl&, NavigationType, bool);

private:
    QUrl mLastClickedLink;
};

#endif // QTWSWEBPAGE_H
