#include "mainwindow.h"
#include <QApplication>
#include <QLibraryInfo>
#include <QWebEngineSettings>
#include <QWebEngineView>
#include <iostream>
#include <QIcon>
#include "qtws.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    //app.setWindowIcon(QIcon(QLatin1String("netflix-desktop")));

    if (argc < 2) {
        qWarning() << "Specify the configuration file";
        return -1;
    }

    try {
        QtWS *configHandler = new QtWS(argv[1]);

        app.setWindowIcon(QIcon(configHandler->getIconPath()));
        MainWindow* w = new MainWindow(NULL, configHandler);
        w->show();
    } catch (QString str) {
        std::cout << str.toStdString();
//        qWarning("Error: " + str.toLatin1() + ".");
        return -1;
    }

    return app.exec();
}
