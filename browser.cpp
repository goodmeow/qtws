#include "browser.h"

#include <QDesktopServices>

Browser::Browser() {
}

QWebEngineView *Browser::createWindow(QWebEnginePage::WebWindowType type) {
    //emit newWindow(type.url());
    return NULL;
}

