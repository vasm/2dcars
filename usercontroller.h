//@vsabadazh

#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H

#include <QObject>
#include "model.h"

class UserController : public QObject
{
    Q_OBJECT
public:
    explicit UserController(Model* model, QObject *parent = 0);
    void attachObject(QString name);

protected:
    void timerEvent(QTimerEvent *);

private:
    Model*          _model;
    QString         _attachedObject;
    //store local copy of controlled object:
    float           _x, _y;
    unsigned        _destX,
                    _destY;
    int             _timerID;

signals:
    
public slots:
    void mousePressed(unsigned x, unsigned y);
};

#endif // USERCONTROLLER_H
