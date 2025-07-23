#ifndef APPLICATIONCORE_H
#define APPLICATIONCORE_H

#include "servicebus.h"
#include <QObject>
#include <QMap>
#include <QQueue>
#include <QJsonDocument>
#include <QJsonObject>
#include <device-common/device_state.h>

enum BusControllerState {
    bcsUNKNOWN = 0,
    bcsCONNECTED = 1,
    bcsWAITING = 2,
};

enum QuestionType {
    qtHAND_SHAKE = 0,
    qtDATA_LOAD = 1,
};

struct Question {
    QString text;
    QString data;
    QuestionType type;

    QString ToString() {
        return QJsonDocument{
            QJsonObject{
                { "text", text} ,
                { "data", data },
                { "type", static_cast<int>( type ) }
            }
        }.toJson();
    }

    static Question FromJSON(QString json) {
        auto document = QJsonDocument::fromJson(json.toLocal8Bit());
        auto object = document.object();
        return Question{
            object.find("text")->toString(""),
            object.find("data")->toString(""),
            static_cast<QuestionType>( object.find("type")->toInt(1) )
        };
    }
};

struct Answer {
    QString text;
    QString data;
    QuestionType type;

    QString ToString() {
        return QJsonDocument{ QJsonObject{
                                 { "text", text },
                                 { "data", data }
                             } }.toJson();
    }

    static Answer FromJSON(QString json) {
        auto document = QJsonDocument::fromJson(json.toLocal8Bit());
        auto object = document.object();
        return Answer{
            object.find("text")->toString(""),
            object.find("data")->toString("")
        };
    }
};

class BusController{

public:
    void ProcessMessage(QString message) {
        qDebug() << "Process message: " << message;
        switch(m_state)
        {
        case bcsUNKNOWN:
            AskHandShake();
            break;
        case bcsCONNECTED:
            ProcessCommand(message);
            break;
        case bcsWAITING:
            ProcessAnswer(Answer::FromJSON(message));
            break;
        }
    }

    void ProcessAnswer(Answer answer)
    {
        if(answer.text == "ok")
        {
            m_state = bcsCONNECTED;
            m_answers.enqueue(answer);
            qDebug() << "state changed on connected";
        }
    }

    void ProcessCommand(QString message)
    {
        try
        {
            auto question = Question::FromJSON(message);
            if( question.text == "status")
            {
                m_answers.enqueue(Answer{
                    "status",
                    DeviceState::instance().deviceStatus()->ToJSON()
                });
            }

            if( question.text == "info" )
            {
                m_answers.enqueue(Answer{
                    "info",
                    DeviceState::instance().deviceInfo()->toJSON()
                });
            }
        }
        catch(... )
        {
            qDebug() << "process command failed: " << message;
        }

        qDebug() << "process custom result";
        return;
    }

    bool HasQuestion() {
        return !m_questions.empty();
    }

    bool HasAnswer() {
        return !m_answers.empty();
    }

    Question GetQuestion() {
        return m_questions.dequeue();
    }

    Answer GetAnswer() {
        return m_answers.dequeue();
    }

    void AskHandShake() {
        qDebug() << "asking handshake...";
        m_questions.enqueue(Question{
            "ack",
            "",
            QuestionType::qtHAND_SHAKE
        });
        m_state = bcsWAITING;
    }

private:
    BusControllerState m_state = bcsUNKNOWN;
    QQueue<Question> m_questions;
    QQueue<Answer> m_answers;
};

class ApplicationCore : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationCore(QObject *parent = nullptr);
    void SetApplicationBus(ServiceBus* bus);

    void SendAnswers(quint64 descriptor);
    void SendQuestions(quint64 descriptor);

public slots:
    void handleConnectionDeleted(quint64 descriptor);
    void handleNewConnection(quint64 descriptor);
    void handleMessage(quint64 descriptor, QString message);

private:
    QMap<quint64, BusController> m_busControllers;
    ServiceBus *m_serviceBus;
    void SendAswers(quint64);
signals:

};

#endif // APPLICATIONCORE_H
