#include "browser.h"

#include <QDesktopServices>

Browser::Browser() {
}

QWebEngineView *Browser::createWindow(QWebEnginePage::WebWindowType type) {
//    emit newWindowOpen(type.url());
    return NULL;
}

