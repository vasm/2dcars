//@vsabadazh

#include "gamecore.h"
#include <QDebug>

GameCore::GameCore(QObject *parent) :
    QObject(parent),
    _netController(0)
{
    _userController=new UserController(&_model);
    _controllerThread=new QThread(this);

    _renderer=new Renderer(&_model);
    connect(_renderer, SIGNAL(connectRequested()), SLOT(connectionRequested()));

    _userController->moveToThread(_controllerThread);
    QObject::connect(_renderer, SIGNAL(mousePressed(uint,uint)), _userController, SLOT(mousePressed(uint,uint)), Qt::QueuedConnection);
    _controllerThread->start();

    _connectDialog=new ConnectDialog();
    connect(_connectDialog, SIGNAL(accepted()), SLOT(dialogAccepted()));
    connect(this, SIGNAL(execDialog()), _connectDialog, SLOT(exec()));

    _server = new GameServer(&_model);
    _server->connect(this, SIGNAL(startServer()), SLOT(start()));
    _server->connect(this, SIGNAL(stopServer()), SLOT(stop()));
    connect(_server, SIGNAL(spawnNetworkController(QTcpSocket*)), SLOT(spawnNetworkController(QTcpSocket*)));
    _networkThread = new QThread(this);
    _server->moveToThread(_networkThread);
    _networkThread->start();
}

GameCore::~GameCore()
{
    _controllerThread->quit();
    _networkThread->quit();

    delete _userController;
    delete _renderer;
    delete _connectDialog;
    delete _server;

    if(_netController)
        delete _netController;
}

void GameCore::init()
{
    GameObject info;
    info["type"]="info";
    info["is_server"]=true;
    _model.modifyObject("info", info);

    //the following code can be easily moved to a script:
    GameObject car;
    car["type"]="car";
    car["x"]=rand()%640;
    car["y"]=rand()%480;
    car["controller"]="user";
    _model.modifyObject("Car 1", car);
    _userController->attachObject("Car 1");

    _renderer->init();
    _renderer->show();
    _renderer->startRendering();

    emit startServer();
}

#include "connectdialog.h"

void GameCore::connectionRequested()
{
    //create dialog
    emit execDialog();
}

void GameCore::dialogAccepted()
{
    qDebug("dialog accepted");
    QTcpSocket* socket = new QTcpSocket();
    socket->connectToHost(_connectDialog->address(), _connectDialog->port());
    spawnNetworkController(socket);
}

void GameCore::spawnNetworkController(QTcpSocket* sock)
{
    if(!_netController)
    {
        _netController = new NetworkController(&_model, sock);
        _netController->moveToThread(_networkThread);
    }
}
