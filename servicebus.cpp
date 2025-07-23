#include "servicebus.h"

ServiceBus::ServiceBus(QObject *parent)
    : QObject{parent}
{}

void ServiceBus::startServer(QHostAddress address, qint64 port)
{
    if(m_server)
        throw new ServerAlreadyRunningException;

    m_server = new QTcpServer();

    connect(m_server, &QTcpServer::newConnection, this, &ServiceBus::acceptNewConnection);
    connect(m_server, &QTcpServer::acceptError, [](QAbstractSocket::SocketError error) {
        qDebug() << "Server error:" << error;
    });

    m_server->listen(address, port);

    if(m_server->isListening())
        qDebug() << "Server is listening on:" << m_server->serverAddress().toString()
                 << "port:" << m_server->serverPort();

}

void ServiceBus::stopServer()
{
    m_server->close();
    m_server->deleteLater();

    m_server = nullptr;
}

QStringList ServiceBus::connectors()
{
    QStringList result;

    for( const auto& [_, bus ] : m_connectionPool.asKeyValueRange() )
    {
        result.push_back( bus->localAddress().toString() );
    }

    qDebug() << "connectors: " << result;
    return result;
}

void ServiceBus::acceptNewConnection()
{
    qDebug() << "ServiceBus::acceptNewConnection";
    QTcpSocket* socket = m_server->nextPendingConnection();
    if (!socket) {
        qDebug() << "Error: socket is null!";
        return;
    }

    m_connectionPool[ socket->socketDescriptor() ] = socket;

    emit connectorsChanged();
    emit newConnection( socket->socketDescriptor() );

    connect(socket, &QTcpSocket::readyRead, this, &ServiceBus::acceptNewMessage);
    connect(socket, &QTcpSocket::disconnected, this, &ServiceBus::handleDisconnection);
}

void ServiceBus::acceptNewMessage()
{
    qDebug() << "ServiceBus::acceptNewMessage";
    auto socket = qobject_cast<QTcpSocket*>(this->sender());
    auto bytes = socket->readAll();
    QString message = QString::fromLocal8Bit(bytes);

    emit messageGotten(socket->socketDescriptor(), message);
    qDebug() << "Прочтено" << message;
}

void ServiceBus::handleDisconnection()
{
    QTcpSocket* soc = qobject_cast<QTcpSocket*>( sender() );

    qDebug() << "socket disconnected: " << soc->socketDescriptor();
    if( m_connectionPool.contains( soc->socketDescriptor() ) )
    {
        m_connectionPool.remove( soc->socketDescriptor() );

        emit connectionDeleted( soc->socketDescriptor() );
        emit connectorsChanged();
    }
}

void ServiceBus::sendMessage(qintptr descriptor, QString message)
{
    qDebug() << "ServiceBus::sendMessage, descriptor: " << descriptor << " message: " << message;
    if( m_connectionPool.contains( descriptor ) )
    {
        qDebug() << "Отправлено сообщение: " << message;
        m_connectionPool[ descriptor ]->write( message.toLocal8Bit() );
    }

}
