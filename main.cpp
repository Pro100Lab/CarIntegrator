#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <thread>
#include <qDebug>
#include <chrono>
#include <debughelper.h>

void pingDebug()
{
        qDebug() << "ping";
}

int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    DebugHelper debugHelper;
    engine.rootContext()->setContextProperty("debugHelper", &debugHelper);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("CarIntegrator", "Main");

    return app.exec();
}
