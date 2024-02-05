#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "hydroponic_data.pb.h"
#include "UdpHandler/udphandler.h"
#include <QThread>


void udpTask(){

    qInfo() << "Thread running";

    UdpHandler udpServer("127.0.0.1", 5000);

    QByteArray message("Test message");

    udpServer.sendBytes(message, "127.0.0.1", 5000);

    QByteArray receivedMessage;

    while(true){
        if(udpServer.isDataAvailable()){
            udpServer.readBytes(&receivedMessage);
            qInfo() << "Message received:" << receivedMessage;
        }

    }


}

int main(int argc, char *argv[])
{

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    UdpHandler udpServer("127.0.0.1", 5000);

    QByteArray message("Test message");

    udpServer.sendBytes(message, "127.0.0.1", 5000);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
