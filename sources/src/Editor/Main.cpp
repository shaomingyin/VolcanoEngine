//
//
#include <QGuiApplication>
#include <QCommandLineParser>

#include <Volcano/Editor/Main.hpp>

VOLCANO_EDITOR_BEGIN

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QCommandLineParser parser;

    return app.exec();
}

VOLCANO_EDITOR_END
