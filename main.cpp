#include <QApplication>
#include "gamecore.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    GameCore core;
    core.init();

    return app.exec();
}
