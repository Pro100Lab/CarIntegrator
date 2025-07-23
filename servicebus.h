#ifndef SERVICEBUS_H
#define SERVICEBUS_H

#include <QObject>
#include <QHostAddress>
#include <QTcpServer>
#include <QException>
#include <QTcpSocket>
#include <QMap>

struct ServerAlreadyRunningException : public QException {};


class ServiceBus : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList connectors READ connectors NOTIFY connectorsChanged FINAL)


public:
    explicit ServiceBus(QObject *parent = nullptr);

    void startServer(QHostAddress address, qint64 port);
    void stopServer();

    QStringList connectors();

public slots:
    void acceptNewConnection();
    void acceptNewMessage();
    void handleDisconnection();

    void sendMessage(qintptr descriptor, QString message);

signals:
    void messageGotten(qintptr descriptor, QString message);
    void newConnection(qintptr descriptor);
    void connectionDeleted(qintptr descriptor);
    void connectorsChanged();

private:
    QTcpServer* m_server = nullptr;
    QMap<qintptr, QTcpSocket*> m_connectionPool;
    qint64 m_limitConnections;
};

#endif // SERVICEBUS_H
