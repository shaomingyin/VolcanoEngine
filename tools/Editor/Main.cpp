//
//
#include <QString>
#include <QMainWindow>
#include <QApplication>

#include <Volcano.hpp>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QApplication::setApplicationName("VolcanoEditor");
    QApplication::setApplicationVersion(QString("%1.%2").arg(VOLCANO_VERSION_MAJOR).arg(VOLCANO_VERSION_MINOR));

    QMainWindow mainWindow;
    mainWindow.resize(800, 600);
    mainWindow.show();

    return app.exec();
}
