//
//
#include <QGuiApplication>
#include <QCommandLineParser>

#include <Volcano/Server/Main.hpp>

VOLCANO_SERVER_BEGIN

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QCommandLineParser parser;

    return app.exec();
}

VOLCANO_SERVER_END
