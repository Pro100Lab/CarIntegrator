#include "applicationcore.h"

ApplicationCore::ApplicationCore(QObject *parent)
    : QObject{parent}
{}

void ApplicationCore::SetApplicationBus(ServiceBus *service_bus)
{
    m_serviceBus = service_bus;
    connect( m_serviceBus, &ServiceBus::newConnection, this, &ApplicationCore::handleNewConnection );
    connect( m_serviceBus, &ServiceBus::messageGotten, this, &ApplicationCore::handleMessage );
    connect( m_serviceBus, &ServiceBus::connectionDeleted, this, &ApplicationCore::handleConnectionDeleted );
}

void ApplicationCore::SendAnswers(quint64 descriptor)
{
    auto& bus_controller = m_busControllers[ descriptor ];
    qDebug() << "ApplicationCore::SendAnswers: " << bus_controller.HasAnswer();

    if( bus_controller.HasAnswer() )
    {
        m_serviceBus->sendMessage( descriptor, bus_controller.GetAnswer().ToString() );
    }
}

void ApplicationCore::SendQuestions(quint64 descriptor)
{
    auto& bus_controller = m_busControllers[ descriptor ];
    qDebug() << "ApplicationCore::SendQuestions: " <<  bus_controller.HasQuestion();;

    if( bus_controller.HasQuestion() )
    {
        m_serviceBus->sendMessage( descriptor, bus_controller.GetQuestion().ToString() );
    }
}

void ApplicationCore::handleConnectionDeleted(quint64 descriptor)
{
    if( auto bus_it = m_busControllers.find( descriptor ); bus_it != m_busControllers.end() )
        m_busControllers.erase( bus_it );
}

void ApplicationCore::handleNewConnection(quint64 descriptor)
{
    qDebug() << "ApplicationCore::handleNewConnection";
    m_busControllers[ descriptor ] = BusController{};
    m_busControllers[ descriptor ].AskHandShake();

    SendQuestions( descriptor );
}

void ApplicationCore::handleMessage(quint64 descriptor, QString message)
{
    auto& bus_controller = m_busControllers[ descriptor ];
    bus_controller.ProcessMessage( message );

    SendAnswers( descriptor );
    SendQuestions( descriptor );
}
