//
//
#include <QDir>
#include <QUrl>
#include <QCommandLineParser>
#include <QQmlApplicationEngine>
#include <QGuiApplication>

#include <Volcano.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QGuiApplication::setOrganizationName("Volcano");
    QGuiApplication::setApplicationName("Launcher");

    QGuiApplication::setApplicationVersion(
        QString("%1.%2.%3").
            arg(VOLCANO_VERSION_MAJOR).
            arg(VOLCANO_VERSION_MINOR).
            arg(VOLCANO_VERSION_PATCH));

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("init", "specifies the init rc.");

    parser.process(app);

    const QStringList pargs = parser.positionalArguments();
    if (pargs.size() != 1)
        return EXIT_FAILURE;

    QQmlApplicationEngine engine;

    Volcano::init();

    engine.load(QUrl::fromUserInput(pargs[0], QDir::currentPath()));

    return app.exec();
}
