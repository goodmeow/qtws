#ifndef BROWSER_H
#define BROWSER_H

#include <QWebEngineView>

class Browser : public QWebEngineView
{
public:
    Browser();

    virtual QWebEngineView *createWindow(QWebEnginePage::WebWindowType);

signals:
    void newWindow(QUrl);
};

#endif // BROWSER_H
