//@vsabadazh

#include "networkcontroller.h"
#include <QDebug>

NetworkController::NetworkController(Model *model, QTcpSocket *sock, QObject *parent):
    QObject(parent),
    _socket(sock),
    _model(model)
{
    connect(_socket, SIGNAL(readyRead()), SLOT(bytesAvailable()));
    qRegisterMetaType<QAbstractSocket::SocketState>("QAbstractSocket::SocketState");
    connect(_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), SLOT(socketStateChanged(QAbstractSocket::SocketState)));

    GameObject newObj;
    newObj["type"]="car";
    newObj["controller"]="network";
    newObj["x"]=100;
    newObj["y"]=100;
    newObj["angle"]=1.0f;
    _model->modifyObject("Car 2", newObj);

    _timerID = startTimer(100);
}

void NetworkController::timerEvent(QTimerEvent *)
{
    //qDebug() << "timerEvent();";

    //send current position of user's car
    GameObject car=_model->getObject("Car 1");
    float data[3] = {car["x"].toFloat(),
                     car["y"].toFloat(),
                     car["angle"].toFloat()};
    _socket->write((char*)data, sizeof(float)*3);
}

void NetworkController::removeControlledObject()
{
    _model->modifyObject("Car 2", GameObject());
}

void NetworkController::bytesAvailable()
{
    quint64 bytes=_socket->bytesAvailable();
    //qDebug() << "available bytes " << bytes;
    if(bytes>=3*sizeof(float))
    {
        float data[3];
        _socket->read((char*)data, sizeof(float)*3);

        QString obj="Car 2";
        _model->modifyObject(obj, "x", data[0]);
        _model->modifyObject(obj, "y", data[1]);
        _model->modifyObject(obj, "angle", data[2]);
    }
}

void NetworkController::socketStateChanged(QAbstractSocket::SocketState newState)
{
    qDebug() << "Socket state changed: " << newState;
    if(newState == QAbstractSocket::UnconnectedState)
    {
        removeControlledObject();
        killTimer(_timerID);
    }
}
