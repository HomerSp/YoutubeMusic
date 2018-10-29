#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtWebEngine>
#include <QSystemTrayIcon>
#include "webengineviewfix.h"
#include "thememanager.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setApplicationName("YoutubeMusic");
    QCoreApplication::setOrganizationName("aqba");
    QApplication app(argc, argv);

    // Make sure the tray icon shows properly
    QIcon::setThemeSearchPaths(QIcon::themeSearchPaths() << ":/icons");

    QtWebEngine::initialize();

    qmlRegisterType<WebEngineViewFix>("se.aqba.qt.youtubemusic", 1, 0, "WebEngineViewFix");
    qmlRegisterType<ThemeManager>("se.aqba.qt.youtubemusic", 1, 0, "ThemeManager");
    qmlRegisterType<ThemeProperties>("se.aqba.qt.youtubemusic", 1, 0, "ThemeProperties");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    MainWindow mainWindow(engine.rootObjects().first());

    return app.exec();
}
