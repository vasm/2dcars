//@vsabadazh

#include "model.h"

Model::Model(QObject *parent) :
    QObject(parent)
{
}

GameState Model::getStateSnapshot()
{
    //lock mutex during method execution
    QMutexLocker locker(&_mutex);
    return _state;
}

GameObject Model::getObject(QString objectName)
{
    QMutexLocker locker(&_mutex);
    GameState::iterator iter=_state.find(objectName);
    if(iter == _state.end())
        return GameObject();
    else
        return *iter;
}

void Model::modifyObject(QString objectName, GameObject newValue)
{
    QMutexLocker locker(&_mutex);
    _state[objectName]=newValue;
}

void Model::modifyObject(QString objectName, QString propertyName, QVariant newValue)
{
    QMutexLocker locker(&_mutex);
    _state[objectName][propertyName]=newValue;
}
