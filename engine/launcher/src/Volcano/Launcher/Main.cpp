//
//
#include <QString>
#include <QScopedPointer>
#include <QThread>
#include <QDir>
#include <QFileInfo>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QGuiApplication>

#include <Volcano/Launcher/Common.hpp>
#include <Volcano/Launcher/MainWindow.hpp>
#include <Volcano/Launcher/GameThread.hpp>

VOLCANO_LAUNCHER_BEGIN

static int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QGuiApplication::setOrganizationName("VolcanoEngine");
    QGuiApplication::setApplicationDisplayName("Volcano Launcher");
    QGuiApplication::setApplicationName("VolcanoLauncher");
    QGuiApplication::setApplicationVersion(QString("%1.%2.%3").arg(VOLCANO_VERSION_MAJOR).arg(VOLCANO_VERSION_MINOR).arg(VOLCANO_VERSION_PATCH));

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("gamePath", "Game path.");

    parser.process(app);

    QString gamePath;

    const QStringList args = parser.positionalArguments();
    if (args.size() < 1)
        gamePath = QDir::currentPath();
    else if (args.size() < 2)
        gamePath = args[0];
    else
        return EXIT_FAILURE;

    QFileInfo fi(gamePath);
    if (!fi.isDir())
        return EXIT_FAILURE;

    QScopedPointer<MainWindow> mainWindow(new MainWindow());
    if (!mainWindow || !mainWindow->init())
        return EXIT_FAILURE;

    mainWindow->show();

    QScopedPointer<GameThread> gameThread(new GameThread(gamePath));
    if (gameThread == nullptr)
        return EXIT_FAILURE;

    gameThread->start();

    int ret = QGuiApplication::exec();

    gameThread->wait();

    return ret;
}

VOLCANO_LAUNCHER_END

int main(int argc, char *argv[])
{
    return Volcano::Launcher::main(argc, argv);
}
