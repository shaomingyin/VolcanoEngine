//
//
#include <QScopedPointer>
#include <QCommandLineParser>
#include <QGuiApplication>

#include <Volcano/Launcher/Common.hpp>
#include <Volcano/Launcher/MainWindow.hpp>

VOLCANO_LAUNCHER_BEGIN

static int Main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QGuiApplication::setOrganizationName("VolcanoEngine");
    QGuiApplication::setApplicationDisplayName("Volcano Launcher");
    QGuiApplication::setApplicationName("VolcanoLauncher");

    QScopedPointer<MainWindow> mainWindow(new MainWindow);
    if (!mainWindow || !mainWindow->init())
    {
        return EXIT_FAILURE;
    }

    mainWindow->show();

    return QGuiApplication::exec();
}

VOLCANO_LAUNCHER_END

int main(int argc, char *argv[])
{
    return Volcano::Launcher::Main(argc, argv);
}
