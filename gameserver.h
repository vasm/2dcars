//@vsabadazh

#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "model.h"

class GameServer : public QObject
{
    Q_OBJECT
public:
    explicit GameServer(Model *model, QObject *parent = 0);
    ~GameServer();

    static const quint16 port=50000;

private:
    QTcpServer* _server;
    Model*      _model;

signals:
    void spawnNetworkController(QTcpSocket*);

public slots:
    void start();
    void stop();

protected slots:
    void incomingConnection();
};

#endif // GAMESERVER_H
