//
//
#include <QDir>
#include <QUrl>
#include <QStringList>
#include <QGuiApplication>
#include <QCommandLineParser>

#include <Volcano/Game/RegisterTypes.hpp>
#include <Volcano/Player/Main.hpp>
#include <Volcano/Player/MainWindow.hpp>

VOLCANO_PLAYER_BEGIN

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addPositionalArgument("game", "Game url");
    parser.addHelpOption();
    parser.process(app);

    QUrl url;
    QStringList args = parser.positionalArguments();

    if (!args.isEmpty())
    {
        if (args.size() > 1)
        {
            qFatal("Too many game url.");
            return EXIT_FAILURE;
        }

        url = QUrl::fromUserInput(args[0]);
    }
    else
    {
        url = QUrl::fromLocalFile(QDir::currentPath() + "/index.qml");
    }

    if (!url.isValid())
    {
        qCritical() << "Invalid game url:" << url;
        return EXIT_FAILURE;
    }

    Game::registerTypes();

    MainWindow mainWindow;

    mainWindow.init(url);
    mainWindow.resize(800, 600);
    mainWindow.show();

    return app.exec();
}

VOLCANO_PLAYER_END
