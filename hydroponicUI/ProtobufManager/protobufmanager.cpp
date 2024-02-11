#include "protobufmanager.h"


ProtobufManager::ProtobufManager(QObject *parent) : QObject{parent}
{

    this->udpHandler = new UdpHandler("192.168.1.33", 5000);

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

    cmdMessage.set_command(this->cmdToHydroponicCmd(command));
    // set top level message to command message

    hydroponicMessage.set_allocated_cmd(&cmdMessage);

    // serialize to array
    QByteArray arr;
    arr.resize(hydroponicMessage.ByteSizeLong());

    //serializeToArray(&arr, hydroponicMessage);
    hydroponicMessage.SerializeToArray(arr.data(), arr.size());
    // send to ESP32

    this->udpHandler->sendBytes(arr, "192.168.1.35", 5000);

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

bool ProtobufManager::serializeToArray(QByteArray *buffer, hydroponic::Hydroponic message)
{

    return false;
}

CMD ProtobufManager::cmdToHydroponicCmd(HydroponicCMD cmd)
{
    switch (cmd) {
        case HydroponicCMD::CMD_VALVE_ON:
        return hydroponic::CMD::CMD_VALVE_ON;
        break;

        case HydroponicCMD::CMD_VALVE_OFF:
        return hydroponic::CMD::CMD_VALVE_OFF;
        break;

        case HydroponicCMD::CMD_PUMP_ON:
        return hydroponic::CMD::CMD_PUMP_ON;
        break;

        case HydroponicCMD::CMD_PUMP_OFF:
        return hydroponic::CMD::CMD_PUMP_OFF;
        break;

        case HydroponicCMD::CMD_LED_ON:
        return hydroponic::CMD::CMD_LED_ON;
        break;

        case HydroponicCMD::CMD_LED_OFF:
        return hydroponic::CMD::CMD_LED_OFF;
        break;

    }
}
