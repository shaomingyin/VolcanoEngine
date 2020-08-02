//
//
#include <QString>
#include <QApplication>

#include <Volcano.hpp>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QApplication::setApplicationName("VolcanoEditor");
    QApplication::setApplicationVersion(QString("%1.%2").arg(VOLCANO_VERSION_MAJOR).arg(VOLCANO_VERSION_MINOR));

    Volcano::init();

    return app.exec();
}
