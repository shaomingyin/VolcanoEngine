//
//
#include <QDir>
#include <QUrl>
#include <QString>
#include <QStringList>
#include <QScopedPointer>
#include <QCommandLineParser>
#include <QCoreApplication>

#include <Volcano/Game/Camera.hpp>
#include <Volcano/Game/DirectionalLight.hpp>
#include <Volcano/Game/Entity.hpp>
#include <Volcano/Game/Light.hpp>
#include <Volcano/Game/Model.hpp>
#include <Volcano/Game/OrthoCamera.hpp>
#include <Volcano/Game/PerpectiveCamera.hpp>
#include <Volcano/Game/PointLight.hpp>
#include <Volcano/Game/SpotLight.hpp>
#include <Volcano/Game/World.hpp>

#include <Volcano/Launcher/Application.hpp>

#include <Volcano/Boot/Common.hpp>

VOLCANO_BOOT_BEGIN

static void printHelp(void)
{

}

static int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printHelp();
        return EXIT_FAILURE;
    }

    QStringList args;
    QString mode(argv[1]);

    args << argv[0];

    for (int i = 2; i < argc; ++i)
        args << argv[i];

    QScopedPointer<QCoreApplication> app;

    if (mode == "launch")
        app.reset(new Launcher::Application(args))

    QGuiApplication app(argc, argv);

    QGuiApplication::setApplicationName("VolcanoLauncher");
    QGuiApplication::setApplicationVersion(QString("%1.%2.%3").arg(VOLCANO_VERSION_MAJOR).arg(VOLCANO_VERSION_MINOR));

    parser.process(app);

    QUrl url = QUrl::fromLocalFile(QDir::currentPath() + "/index.qml");

    const QStringList &sourceList = parser.positionalArguments();
    if (!sourceList.empty())
    {
        if (sourceList.count() != 1)
            return EXIT_FAILURE;
        url = QUrl::fromUserInput(sourceList[0]);
    }

    if (!url.isValid())
        return EXIT_FAILURE;


#if 0
    QScopedPointer<MainWindow> mainWindow(new MainWindow());
    if (!mainWindow)
        return EXIT_FAILURE;

    mainWindow->create();
    mainWindow->resize(800, 600);
    mainWindow->show();
#endif

    return app.exec();
}

VOLCANO_BOOT_END

int main(int argc, char *argv[])
{
    return Volcano::Boot::main(argc, argv);
}
