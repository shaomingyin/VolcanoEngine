//
//
#include <QGuiApplication>

#include "MainWindow.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Volcano::World world;

    MainWindow mainWindow(world);
    mainWindow.resize(800, 600);
    mainWindow.show();

    return app.exec();
}
