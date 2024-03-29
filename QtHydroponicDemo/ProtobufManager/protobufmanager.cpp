#include "protobufmanager.h"


ProtobufManager::ProtobufManager(QObject *parent) : QObject{parent}
{

    this->udpHandler = new UdpHandler("192.168.1.34", 5000);

    connect(this->udpHandler, &UdpHandler::dataReceived, this, &ProtobufManager::packageReceived);

}

ProtobufManager::~ProtobufManager()
{
    if(this->udpHandler != nullptr)
        delete this->udpHandler;

}

ProtobufManager::HydroponicMessageType ProtobufManager::getMessageType()
{
    return this->messageType;
}

int ProtobufManager::getDeviceId()
{
    /*
     *  There is no type such as uint32_t on the QML side.
     *  It's possible there may be a loss of data, I know.
     *  */
    return (int)this->dataMessage.deviceid();
}

QString ProtobufManager::getSectorName()
{
    return this->dataMessage.sector().c_str();
}

float ProtobufManager::getECval()
{
    return this->dataMessage.econductivity();
}

float ProtobufManager::getPh()
{
    return this->dataMessage.ph();
}

float ProtobufManager::getMoisture()
{
    return this->dataMessage.moisture();
}

float ProtobufManager::getTemperature()
{
    return this->dataMessage.temperature();
}

int ProtobufManager::getWaterLevel()
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

void ProtobufManager::sendCommand(HydroponicCMD command)
{
    // create top level message and command message
    hydroponic::Hydroponic hydroponicMessage;
    hydroponic::Command cmdMessage;

    cmdMessage.set_command(this->cmdLookUpTable[command]);
    // set top level message to command message

    hydroponicMessage.set_allocated_cmd(&cmdMessage);

    // serialize to array
    QByteArray arr;
    arr.resize(hydroponicMessage.ByteSizeLong());

    //serializeToArray(&arr, hydroponicMessage);
    hydroponicMessage.SerializeToArray(arr.data(), arr.size());
    // send to ESP32

    //this->udpHandler->sendBytes(arr, "192.168.1.35", 5000);
    this->udpHandler->sendBytes(arr, this->udpHandler->getSenderAddress(), this->udpHandler->getSenderPort());

    // release
    hydroponicMessage.release_cmd();
}

void ProtobufManager::packageReceived()
{
    QByteArray packet;

    this->udpHandler->readBytes(&packet);

    qInfo() << "Package received.";

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

        this->messageType = HydroponicMessageType::DATA;

        break;
    case MessageType::MSG_HEART_BEAT:

        qInfo() << "heart-beat packet received";

        this->heartBeatMessage = this->hydroponicMessage.heartbeat();

        this->messageType = HydroponicMessageType::HEART_BEAT;

        break;
    case MessageType::MSG_OK:

        qInfo() << "ok packet received";

        this->messageOk = this->hydroponicMessage.messageok();

        this->messageType = HydroponicMessageType::MESSAGE_OK;

        break;
    case MessageType::MSG_ERROR:

        qInfo() << "error packet received";

        this->messageError = this->hydroponicMessage.messageerror();

        this->messageType = HydroponicMessageType::MESSAGE_ERROR;

        break;

    case MessageType::MSG_TIMEOUT:

        qInfo() << "timeout packet received";

        this->messageTimeout = this->hydroponicMessage.messagetimeout();

        this->messageType = HydroponicMessageType::MESSAGE_TIMEOUT;

        break;

    default:

        qInfo() << "Invalid Message Type";
        break;
    }

    emit messageReceived();
    return true;

}
