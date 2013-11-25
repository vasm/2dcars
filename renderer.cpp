//@vsabadazh

#include "renderer.h"

#include <cmath>
#include <QMouseEvent>
#include <QDebug>
#include <QApplication>

Renderer::Renderer(Model *model, QWidget *parent) :
    QGLWidget(parent),
    _model(model),
    _timerID(0),
    _windowH(480),
    _windowW(640),
    _posX(320),
    _posY(240),
    _dirX(10),
    _dirY(10)
{
    Q_ASSERT(model!=0);
    _message="Press 'q' to quit";
}

Renderer::~Renderer()
{
    stopRendering();
}

bool Renderer::init()
{
    setFormat(QGLFormat(QGL::DoubleBuffer));
    glDisable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    resize(_windowW, _windowH);
    updateGL();

    qDebug() << "Loading textures...";
    QString textureNames[] = {"./car2.png",
                              "./car.png"};
    glGenTextures(3, _texID);
    for (int i=0; i<2; ++i)
    {
        qDebug() << textureNames[i];
        _textures[i].load(textureNames[i]);
        _textures[i] = QGLWidget::convertToGLFormat(_textures[i]);

        if(_textures[i].isNull())
            qDebug() << "Failed to load texture " << textureNames[i];
        else
        {
            qDebug() << _textures[i].size() << " texID="<<_texID[i];
            //bind the texture ID
            glBindTexture(GL_TEXTURE_2D, _texID[i]);

            //generate the texture
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _textures[i].width(),
                          _textures[i].height(),
                          0, GL_RGBA, GL_UNSIGNED_BYTE, _textures[i].bits());
            //texture parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
    }

    return true;
}

bool Renderer::release()
{
    return true;
}

void rotate2f(float* x, float* y, float angle)
{
    float xVal=(*x), yVal=(*y);

    float a1=cos(angle), a2=sin(angle);
    float newX=a1*xVal-a2*yVal,
          newY=a2*xVal+a1*yVal;

    *x=newX;
    *y=newY;
}

void Renderer::renderCar(float x, float y, bool isUser, float directionAngle)
{
    //qglColor(color);
    QColor color = isUser? Qt::white : Qt::green;
    glColor4f(color.redF(), color.greenF(), color.blueF(), 0.5f);

    float polygon[4][2]={{-2.0f,  -1.0f},
                          {-2.0f,  1.0f},
                          { 2.0f,  1.0f},
                          { 2.0f, -1.0f}};
    static float texCoords[4][2]={{0.0f, 1.0f},
                                  {1.0f, 1.0f},
                                  {1.0f, 0.0f},
                                  {0.0f, 0.0f}};

    for(int i=0; i<4; ++i)
        rotate2f(&polygon[i][0], &polygon[i][1], directionAngle);
    //glBegin(...)
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_POLYGON);
    glBindTexture(GL_TEXTURE_2D, _texID[1]);
        for(int i=0; i<4; ++i)
        {
            glVertex2f(polygon[i][0]*20+x, polygon[i][1]*20+y);
            glTexCoord2f(texCoords[i][0], texCoords[i][1]);
        }
    glEnd();
}

void Renderer::timerEvent(QTimerEvent *)
{
    //query game state from model and unlock model
    GameState state=_model->getStateSnapshot();

    //clear view
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // чистим буфер изображения и буфер глубины
    glMatrixMode(GL_PROJECTION); // устанавливаем матрицу
    glLoadIdentity(); // загружаем матрицу
    glOrtho(0, _windowW, _windowH, 0, 1, 0); // подготавливаем плоскости для матрицы
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //render objects
    foreach(GameObject obj, state)
    {
        if((obj["type"]).toString() == "car")
        {            
            renderCar(obj["x"].toFloat(), obj["y"].toFloat(), (obj["controller"]=="user"), obj["angle"].toFloat());
        }
    }

    //HUD?
    qglColor(Qt::white);
    QString msg=_message;
    if(!state["info"]["is_server"].toBool())
        msg+=QString(", 'c' to connect to a server");
    renderText(10, 465, 0, msg, QFont());

    //finalize:
    swapBuffers();
}

void Renderer::mousePressEvent(QMouseEvent* ev)
{
    QPoint mousePos=ev->pos();
    //qDebug() << mousePos;
    emit mousePressed(mousePos.x(), mousePos.y());
}

void Renderer::keyPressEvent(QKeyEvent *ev)
{
    //qDebug() << ev->key();
    if(ev->key() == 81)
        qApp->quit();
    else if(ev->key()==67) // 'c'
    {
//        if(!(_model->getObject("info")["is_server"].toBool()))
            emit connectRequested();
    }
}

void Renderer::startRendering()
{
    if(!_timerID)
        _timerID=startTimer(1000/_frameRate);
}

void Renderer::stopRendering()
{
    if(_timerID)
    {
        killTimer(_timerID);
        _timerID=0;
    }
}
