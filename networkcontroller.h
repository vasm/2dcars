//@vsabadazh

#ifndef NETWORKCONTROLLER_H
#define NETWORKCONTROLLER_H

#include <QObject>
#include <QTcpSocket>
#include "model.h"

class NetworkController : public QObject
{
    Q_OBJECT
public:
    explicit NetworkController(Model* model, QTcpSocket* sock, QObject *parent = 0);
    
protected:
    void timerEvent(QTimerEvent *);
    void removeControlledObject();

private:
    QTcpSocket*     _socket;
    Model*          _model;
    int             _timerID;

signals:
    
public slots:
    void bytesAvailable();
    void socketStateChanged(QAbstractSocket::SocketState newState);
};

#endif // NETWORKCONTROLLER_H
