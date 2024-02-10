#include "udphandler.h"

UdpHandler::UdpHandler(QObject *parent)
    : QObject{parent}
{

}

UdpHandler::UdpHandler(QString serverIp, quint16 serverPort)
{
    this->socket = new QUdpSocket(this);

    //connect(this->socket, &QUdpSocket::readyRead, this, &UdpHandler::receiveHandler);
    //connect(this->socket, SIGNAL(readyRead()), this, SLOT(receiveHandler()));
    if(!this->socket->bind(QHostAddress(serverIp),serverPort)){
        qInfo() << "Binding Error.";
    }

    auto result = connect(socket, &QUdpSocket::readyRead, this, &UdpHandler::receiveHandler);

    //qInfo() << "Connect result:" << result;

    this->receiveBuffer = new QByteArray();

}

UdpHandler::~UdpHandler()
{
    this->socket->close();

    if(this->socket != nullptr)
        delete this->socket;
    if(this->receiveBuffer != nullptr)
        delete this->receiveBuffer;
}

quint64 UdpHandler::sendBytes(QByteArray arr, QString ip, quint16 port)
{
    return this->socket->writeDatagram(arr, QHostAddress(ip), port);
}

bool UdpHandler::isDataAvailable()
{
    return this->isDataReceived;
}

int UdpHandler::readBytes(QByteArray *buffer)
{
    buffer->resize(this->receiveBuffer->size());

    for(int i=0; i<this->receiveBuffer->size(); i++){
        buffer->data()[i] = this->receiveBuffer->data()[i];
    }

    this->isDataReceived = true;

    return this->receiveBuffer->size();
}

void UdpHandler::clearReceiveBuffer()
{
    this->receiveBuffer->clear();
}

void UdpHandler::receiveHandler()
{
    QHostAddress sender;
    quint16 senderPort;

    qInfo() << "Receive Handler called";
    this->receiveBuffer->resize(this->socket->pendingDatagramSize());

    while(this->socket->hasPendingDatagrams()){
        this->socket->readDatagram(this->receiveBuffer->data(), this->receiveBuffer->size(), &sender, &senderPort);
    }

    this->isDataReceived = true;

    emit this->dataReceived();
}
