//@vsabadazh

#ifndef RENDERER_H
#define RENDERER_H

#include <QObject>
#include <QGLWidget>
#include "model.h"

class Renderer : public QGLWidget
{
    Q_OBJECT
public:
    explicit Renderer(Model* model, QWidget *parent = 0);
    ~Renderer();

protected:
    // will be used for rendering
    void timerEvent(QTimerEvent*);

    // input
    void mousePressEvent(QMouseEvent*);
    void keyPressEvent(QKeyEvent *ev);

    void renderCar(float x, float y, bool isUser, float directionAngle);

private:
    Model*              _model;

    int                 _timerID;
    static const int    _frameRate=30;
    unsigned            _windowH, _windowW;
    unsigned            _posX, _posY;
    int                 _dirX, _dirY;
    QImage              _textures[2];
    GLuint              _texID[3];
    QString             _message;

signals:
    void mousePressed(unsigned x, unsigned y);
    void connectRequested();

public slots:
    // initialize and un-initialize OpenGL
    bool init();
    bool release();

    // start and stop rendering process
    void startRendering();
    void stopRendering();

};

#endif // RENDERER_H
