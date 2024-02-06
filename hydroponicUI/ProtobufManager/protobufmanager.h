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

signals:

public slots:
    void packageReceived();

private:
    UdpHandler *udpHandler = nullptr;

    //class declaration of protobuf messages
    Hydroponic hydroponicMessage;    // Top level message
    DataPackage dataMessage;
    HeartBeat heartBeatMessage;
    MessageOk messageOk;
    MessageError messageError;
    MessageTimeout messageTimeout;

    bool parseProtobuf(const QByteArray arr);
    bool serializeToArray(QByteArray *buffer, MessageType messageType);
};

#endif // PROTOBUFMANAGER_H
