//
//
#include <QGuiApplication>

#include <Volcano/Launcher/Common.hpp>

VOLCANO_LAUNCHER_BEGIN

static int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    setApplicationName("Volcano");

    return app.exec();
}

VOLCANO_LAUNCHER_END

int main(int argc, char *argv[])
{
    return Volcano::Launcher::main(argc, argv);
}
