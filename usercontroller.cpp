//@vsabadazh

#include "usercontroller.h"

#include <QDebug>
#include <QVariant>

UserController::UserController(Model* model, QObject *parent) :
    QObject(parent),
    _model(model),
    _timerID(0)
{
    Q_ASSERT(model!=0);
}

void UserController::attachObject(QString name)
{
    _attachedObject=name;

    //query object's position to cache it
    GameState stateSnapshot=_model->getStateSnapshot();
    GameState::iterator it=stateSnapshot.find(_attachedObject);

    if(it==stateSnapshot.end())
    {
        qDebug() << "Warning: object named " << _attachedObject << " does not exist";
        _attachedObject.clear();
    }
    else
    {
        GameObject ob=*it;
        bool ok;
        _x=(ob.find("x"))->toUInt(&ok);
        if(!ok)
        {
            qDebug() << "Failed to read x coordinate from state";
            //set some default value
            _x=100;
        }
        _y=(ob.find("y"))->toUInt(&ok);
        if(!ok)
        {
            qDebug() << "Failed to read y coordinate from state";
            _y=100;
        }
    }
}


void UserController::mousePressed(unsigned x, unsigned y)
{
    _destX=x; _destY=y;
    if((!_attachedObject.isEmpty() && (!_timerID)))
        _timerID=startTimer(30);
}

#include <cmath>

inline float vectorToAngle(float x, float y)
{
    if(x==0)
        return (y>0)?M_PI_2:(-M_PI_2);
    else
    {
        float at=atan(y/x);
        return (x>0)?at:(M_PI+at);
    }
}

float l2norm (float x, float y)
{
    return sqrt(x*x+y*y);
}

void UserController::timerEvent(QTimerEvent *)
{
    float dirX=(float)_destX-(float)_x, dirY=(float)_destY-(float)_y;
    float dirLength=sqrt(dirX*dirX + dirY*dirY);
    if(round(dirLength) > 2)
    {
//        float car2x=-1.0f, car2y=-1.0f;
//        GameObject car2=_model->getObject("Car 2");
//        if(!car2.empty())
//        {
//            car2x=car2["x"].toFloat();
//            car2y=car2["y"].toFloat();
//        }

        //normalize vector:
        dirX/=dirLength; dirY/=dirLength;

        //primitive collision detection:
//        if((car2x+car2y < 0) || (l2norm(_x+4*dirX - car2x, _y+4*dirY - car2y) > 80))
//        {
            _x+=dirX*2;
            _y+=dirY*2;
//        }

        //update object state

#warning TODO: compute angle only once
        float angle=vectorToAngle(dirX, dirY);
        //qDebug() << "angle="<<angle;

        _model->modifyObject(_attachedObject, "x", _x);
        _model->modifyObject(_attachedObject, "y", _y);
        _model->modifyObject(_attachedObject, "angle", angle);
    }
    else
    {   //came to the target: stop
        killTimer(_timerID);
        _timerID=0;
    }
}
