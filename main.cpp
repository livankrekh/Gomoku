#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "gomokugame.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

//    qmlRegisterType<GomokuGame>("gomoku.process", 1, 0, "GameProcess");

    GomokuGame game;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("game", &game);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
