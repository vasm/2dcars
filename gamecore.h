//@vsabadazh

#ifndef GAMECORE_H
#define GAMECORE_H

#include <QObject>
#include <QThread>

#include "model.h"
#include "renderer.h"
#include "usercontroller.h"
#include "connectdialog.h"
#include "gameserver.h"
#include "networkcontroller.h"

class GameCore : public QObject
{
    Q_OBJECT
public:
    explicit GameCore(QObject *parent = 0);
    ~GameCore();

    void init();

private:
    Model               _model;
    Renderer*           _renderer;
    UserController*     _userController;
    QThread*            _controllerThread;
    GameServer*         _server;
    NetworkController*  _netController;
    QThread*            _networkThread;
    ConnectDialog*      _connectDialog;


signals:
    void execDialog();
    void startServer();
    void stopServer();

public slots:
    void connectionRequested();
    void dialogAccepted();
    void spawnNetworkController(QTcpSocket*sock);
};

#endif // GAMECORE_H
