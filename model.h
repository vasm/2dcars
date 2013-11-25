//@vsabadazh

#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QMutex>

//Yep, I use Qt containers to avoid wheel-reinvention :)
#include <QMap>
#include <QVariant>
#include <QString>

typedef QMap<QString, QVariant> GameObject;  // string for property name, variant is the property value
typedef QMap<QString, GameObject> GameState; // string for object name

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = 0);

    //get game state for rendering
    GameState getStateSnapshot();

    GameObject getObject(QString objectName);
    //also can create new nodes with this:
    void modifyObject(QString objectName, GameObject newValue);
    void modifyObject(QString objectName, QString propertyName, QVariant newValue);

private:
    QMutex      _mutex;
    GameState   _state;

signals:
    
public slots:
    
};

#endif // MODEL_H
