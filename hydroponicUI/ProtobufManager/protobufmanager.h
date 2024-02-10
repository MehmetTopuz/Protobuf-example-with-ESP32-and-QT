#ifndef PROTOBUFMANAGER_H
#define PROTOBUFMANAGER_H

#include <QObject>
#include "hydroponic_data.pb.h"
#include "udphandler.h"

using namespace hydroponic;

class ProtobufManager : public QObject
{
    Q_OBJECT
public:
    explicit ProtobufManager(QObject *parent = nullptr);
    ~ProtobufManager();

    enum HMessageType{
        DATA = 0,
        HEART_BEAT,
        MESSAGE_OK,
        MESSAGE_ERROR,
        MESSAGE_TIMEOUT
    };

    Q_INVOKABLE ProtobufManager::HMessageType getMessageType();
    Q_INVOKABLE uint32_t getDeviceId();
    Q_INVOKABLE QString getSectorName();
    Q_INVOKABLE float getECval();
    Q_INVOKABLE float getPh();
    Q_INVOKABLE float getMoisture(); // change return type later
    Q_INVOKABLE float getTemperature();
    Q_INVOKABLE int getWaterLevel();
    Q_INVOKABLE bool getValveState();
    Q_INVOKABLE bool getPumpState();
    Q_INVOKABLE bool getLedState();


signals:
    void messageReceived();

public slots:
    void packageReceived();

private:
    UdpHandler *udpHandler = nullptr;

    //Class declaration of protobuf messages
    Hydroponic hydroponicMessage;    // Top level message
    DataPackage dataMessage;
    HeartBeat heartBeatMessage;
    MessageOk messageOk;
    MessageError messageError;
    MessageTimeout messageTimeout;

    HMessageType messageType;

    bool parseProtobuf(const QByteArray arr);
    bool serializeToArray(QByteArray *buffer, MessageType messageType);
};

#endif // PROTOBUFMANAGER_H
