#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <qDebug>
#include <debughelper.h>
#include <appcore/applicationcore.h>
#include <servicebus.h>
#include <device-common/device_state.h>
#include <device-common/device_info.h>

const QString DEFAULT_ICON = "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAEAAAABAAgMAAADXB5lNAAAAIGNIUk0AAHomAACAhAAA+gAAAIDoAAB1MAAA6mAAADqYAAAXcJy6UTwAAAAMUExURQAAAAAA//8AAP///0Ol8HgAAAABdFJOUwBA5thmAAAAAWJLR0QDEQxM8gAAAAd0SU1FB+kHFA8mETL2GNkAAADiSURBVDjLxZAxDoJAEEU/GzYhVBTYY7d6im3oMQHPQ8MNOABaiad016jMHwpC5RQk/J3/5s8AskqoKtR/4rWgHakWVsxmN9NuMnVOeNXOQy6emYnvmGmDRDmtx4FzNrDUMZWcyzxSTp6Zm6ccDq6hc0w4eQkxPY500hwY6GIOGOOQdhHMiM4vnhxmiLukAjrXU0FCfg1hW5EDz9yKqFn8zLVYro+zFsfbg0quHz29EJz52L5VBY+hC4awmUREg2SG9XFnBHBmBBghu6EefwhoBJVGGN2wYrrNISvm/iEr5l/qBWq/H6mN9pWRAAAAJXRFWHRkYXRlOmNyZWF0ZQAyMDI1LTA3LTIwVDE1OjM4OjE2KzAwOjAwfHjLigAAACV0RVh0ZGF0ZTptb2RpZnkAMjAyNS0wNy0yMFQxNTozODoxNiswMDowMA0lczYAAAAodEVYdGRhdGU6dGltZXN0YW1wADIwMjUtMDctMjBUMTU6Mzg6MTcrMDA6MDD8R1ldAAAAAElFTkSuQmCC";

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    DebugHelper debugHelper;

    DeviceState::instance().deviceStatus()->setDescription("Утрируем плату");
    DeviceState::instance().deviceStatus()->setProperty("Widgets", QVector<QString>{"Video", "Control"});
    DeviceState::instance().deviceInfo()->setName("Deus vault");
    DeviceState::instance().deviceInfo()->setIcon(DeviceInfo::Base64ToIcon(DEFAULT_ICON));

    ServiceBus service_bus;
    service_bus.startServer(QHostAddress("127.0.0.1"), 21360);

    ApplicationCore appcore;
    appcore.SetApplicationBus(&service_bus);

    QQmlApplicationEngine engine;

    qmlRegisterType<DeviceInfo>("DeviceNS", 1, 0, "DeviceInfo");
    qmlRegisterType<DeviceStatus>("DeviceNS", 1, 0, "DeviceStatus");

    engine.rootContext()->setContextProperty("deviceState", &DeviceState::instance());
    engine.rootContext()->setContextProperty("debugHelper", &debugHelper);
    engine.rootContext()->setContextProperty("appcore", &appcore);
    engine.rootContext()->setContextProperty("serviceBus", &service_bus);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("CarIntegrator", "Main");

    return app.exec();
}
