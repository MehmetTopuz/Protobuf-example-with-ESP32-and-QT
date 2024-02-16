#ifndef PROTOBUFMANAGER_H
#define PROTOBUFMANAGER_H

#include <QObject>
#include <QMap>
#include "hydroponic_data.pb.h"
#include "udphandler.h"

using namespace hydroponic;

class ProtobufManager : public QObject
{
    Q_OBJECT
public:
    explicit ProtobufManager(QObject *parent = nullptr);
    ~ProtobufManager();

    enum HydroponicMessageType{
        DATA = 0,
        HEART_BEAT,
        MESSAGE_OK,
        MESSAGE_ERROR,
        MESSAGE_TIMEOUT
    };
    Q_ENUM(HydroponicMessageType)

    enum HydroponicCMD{
        CMD_VALVE_ON = 0,
        CMD_VALVE_OFF = 1,
        CMD_PUMP_ON = 2,
        CMD_PUMP_OFF = 3,
        CMD_LED_ON = 4,
        CMD_LED_OFF = 5,
    };

    Q_ENUM(HydroponicCMD)

    Q_INVOKABLE ProtobufManager::HydroponicMessageType getMessageType();
    Q_INVOKABLE int getDeviceId();
    Q_INVOKABLE QString getSectorName();
    Q_INVOKABLE float getECval();
    Q_INVOKABLE float getPh();
    Q_INVOKABLE float getMoisture(); // change return type later
    Q_INVOKABLE float getTemperature();
    Q_INVOKABLE int getWaterLevel();
    Q_INVOKABLE bool getValveState();
    Q_INVOKABLE bool getPumpState();
    Q_INVOKABLE bool getLedState();
    Q_INVOKABLE void sendCommand(ProtobufManager::HydroponicCMD command);


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

    HydroponicMessageType messageType;

    bool parseProtobuf(const QByteArray arr);

    /*
     * We cannot access an enum defined inside the Hydroponic class from QML.
     * Therefore, I want to perform an enum conversion through a look-up table.
     * */
    QMap<HydroponicCMD,hydroponic::CMD> cmdLookUpTable = {
        {HydroponicCMD::CMD_VALVE_ON, hydroponic::CMD::CMD_VALVE_ON},
        {HydroponicCMD::CMD_VALVE_OFF, hydroponic::CMD::CMD_VALVE_OFF},
        {HydroponicCMD::CMD_PUMP_ON, hydroponic::CMD::CMD_PUMP_ON},
        {HydroponicCMD::CMD_PUMP_OFF, hydroponic::CMD::CMD_PUMP_OFF},
        {HydroponicCMD::CMD_LED_ON, hydroponic::CMD::CMD_LED_ON},
        {HydroponicCMD::CMD_LED_OFF, hydroponic::CMD::CMD_LED_OFF}
    };
};

#endif // PROTOBUFMANAGER_H
