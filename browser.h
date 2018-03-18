#ifndef BROWSER_H
#define BROWSER_H

#include <QWebEngineView>

class Browser : public QWebEngineView
{
public:
    Browser();

    QWebEngineView *createWindow(QWebEnginePage::WebWindowType);

signals:
    void newWindowOpen(QUrl);
};

#endif // BROWSER_H
