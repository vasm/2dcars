//@vsabadazh

#include "gameserver.h"
#include "model.h"
#include <QDebug>

GameServer::GameServer(Model* model, QObject *parent) :
    QObject(parent),
    _model(model)
{
    _server=new QTcpServer(this);
    connect(_server, SIGNAL(newConnection()), SLOT(incomingConnection()));
}

GameServer::~GameServer()
{
    stop();
}

void GameServer::start()
{
    if(!_server->isListening())
    {
        if(_server->listen(QHostAddress::Any, port))
            qDebug() << "Listening on port " << port;
        else
        {
            qDebug() << "Error opening port";
            _model->modifyObject("info", "is_server", QVariant(false));
        }
    }
}

void GameServer::stop()
{
    if(_server->isListening())
        _server->close();
}

void GameServer::incomingConnection()
{
    qDebug() << "incomingConnection();";

    QTcpSocket* newSocket = _server->nextPendingConnection();
    emit spawnNetworkController(newSocket);
}
