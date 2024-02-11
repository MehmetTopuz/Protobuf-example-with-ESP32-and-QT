#ifndef UDPHANDLER_H
#define UDPHANDLER_H

#include <QObject>
#include <QUdpSocket>

class UdpHandler : public QObject
{
    Q_OBJECT
public:
    explicit UdpHandler(QObject *parent = nullptr);
    UdpHandler(QString serverIp, quint16 serverPort);
    ~UdpHandler();

    quint64 sendBytes(QByteArray arr, QHostAddress destIp, quint16 destPort);
    bool isDataAvailable();
    int readBytes(QByteArray *buffer);
    void clearReceiveBuffer();
    QHostAddress getSenderAddress();
    quint16 getSenderPort();
private:
    QUdpSocket *socket = nullptr;

    QByteArray *receiveBuffer = nullptr;
    bool isDataReceived = false;

    QHostAddress sender;
    quint16 senderPort;

signals:
    void dataReceived();

public slots:
    void receiveHandler();

};

#endif // UDPHANDLER_H
