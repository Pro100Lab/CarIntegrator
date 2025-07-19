#ifndef DEBUGHELPER_H
#define DEBUGHELPER_H

#include <QObject>
#include <qDebug>

class DebugHelper : public QObject
{
    Q_OBJECT
public:
    explicit DebugHelper(QObject *parent = nullptr);

    Q_INVOKABLE void pingDebug(QString str) {
        qDebug() << "ping: " << str;
    }

signals:
};


#endif // DEBUGHELPER_H
