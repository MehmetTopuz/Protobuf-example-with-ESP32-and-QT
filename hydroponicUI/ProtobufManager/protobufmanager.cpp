#include "protobufmanager.h"


ProtobufManager::ProtobufManager(QObject *parent) : QObject{parent}
{

    this->udpHandler = new UdpHandler("127.0.0.1", 5000);

    connect(this->udpHandler, &UdpHandler::dataReceived, this, &ProtobufManager::packageReceived);

}

ProtobufManager::~ProtobufManager()
{
    if(this->udpHandler != nullptr)
        delete this->udpHandler;

}

void ProtobufManager::packageReceived()
{
    QByteArray packet;

    this->udpHandler->readBytes(&packet);

    qInfo() << "Package received.";

    //todo: parse protobuf

}

bool ProtobufManager::parseProtobuf(const QByteArray arr)
{
    auto result = this->hydroponicMessage.ParseFromArray(arr.data(), arr.size());

    if(!result){
        qInfo() << "Protobuf Parse Error";
        return false;
    }

    switch (this->hydroponicMessage.messagetype()) { // or we can use hydroponicMessage.has_datapackage() method.

    case MessageType::MSG_DATA:

        qInfo() << "data packet received";

        this->dataMessage = this->hydroponicMessage.datapackage();

        break;
    case MessageType::MSG_HEART_BEAT:

        qInfo() << "heart-beat packet received";

        this->heartBeatMessage = this->hydroponicMessage.heartbeat();

        break;
    case MessageType::MSG_OK:

        qInfo() << "ok packet received";

        this->messageOk = this->hydroponicMessage.messageok();

        break;
    case MessageType::MSG_ERROR:

        qInfo() << "error packet received";

        this->messageError = this->hydroponicMessage.messageerror();

        break;

    case MessageType::MSG_TIMEOUT:

        qInfo() << "timeout packet received";

        this->messageTimeout = this->hydroponicMessage.messagetimeout();

        break;

    default:

        qInfo() << "Invalid Message Type";
        break;
    }

    return true;

}

bool ProtobufManager::serializeToArray(QByteArray *buffer, MessageType messageType)
{
    return false;
}
