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

ProtobufManager::HMessageType ProtobufManager::getMessageType()
{
    return this->messageType;
}

uint32_t ProtobufManager::getDeviceId()
{
    return this->dataMessage.deviceid();
}

QString ProtobufManager::getSectorName()
{
    return this->dataMessage.sector().c_str();
}

float ProtobufManager::getECval()
{
    return this->dataMessage.econductivity();
}

float ProtobufManager::getMoisture()
{
    return this->dataMessage.moisture();
}

float ProtobufManager::getTemperature()
{
    return this->dataMessage.temperature();
}

uint32_t ProtobufManager::getWaterLevel()
{
    return this->dataMessage.waterlevel();
}

bool ProtobufManager::getValveState()
{
    return this->dataMessage.valvestate();
}

bool ProtobufManager::getPumpState()
{
    return this->dataMessage.pumpstate();
}

bool ProtobufManager::getLedState()
{
    return this->dataMessage.ledstatus();
}

void ProtobufManager::packageReceived()
{
    QByteArray packet;

    this->udpHandler->readBytes(&packet);

    qInfo() << "Package received.";

    //todo: parse protobuf

    this->parseProtobuf(packet);

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

        this->messageType = HMessageType::DATA;

        break;
    case MessageType::MSG_HEART_BEAT:

        qInfo() << "heart-beat packet received";

        this->heartBeatMessage = this->hydroponicMessage.heartbeat();

        this->messageType = HMessageType::HEART_BEAT;

        break;
    case MessageType::MSG_OK:

        qInfo() << "ok packet received";

        this->messageOk = this->hydroponicMessage.messageok();

        this->messageType = HMessageType::MESSAGE_OK;

        break;
    case MessageType::MSG_ERROR:

        qInfo() << "error packet received";

        this->messageError = this->hydroponicMessage.messageerror();

        this->messageType = HMessageType::MESSAGE_ERROR;

        break;

    case MessageType::MSG_TIMEOUT:

        qInfo() << "timeout packet received";

        this->messageTimeout = this->hydroponicMessage.messagetimeout();

        this->messageType = HMessageType::MESSAGE_TIMEOUT;

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
