//
//
#include <QUrl>
#include <QDir>
#include <QScopedPointer>
#include <QObject>
#include <QQmlEngine>
#include <QSurfaceFormat>
#include <QCommandLineParser>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <Volcano/Rotation.hpp>
#include <Volcano/Shape.hpp>
#include <Volcano/Box.hpp>
#include <Volcano/Cylinder.hpp>
#include <Volcano/Plane.hpp>
#include <Volcano/Sphere.hpp>

#include <Volcano/Game/WorldBase.hpp>
#include <Volcano/Game/World.hpp>
#include <Volcano/Game/DynamicWorld.hpp>
#include <Volcano/Game/Object.hpp>
#include <Volcano/Game/Entity.hpp>
#include <Volcano/Game/Component.hpp>
#include <Volcano/Game/Visual.hpp>
#include <Volcano/Game/Collision.hpp>
#include <Volcano/Game/Material.hpp>
#include <Volcano/Game/Mesh.hpp>
#include <Volcano/Game/Light.hpp>
#include <Volcano/Game/DirectionalLight.hpp>
#include <Volcano/Game/PointLight.hpp>
#include <Volcano/Game/SpotLight.hpp>

#include <Volcano/Graphics/Camera.hpp>
#include <Volcano/Graphics/CameraView.hpp>

#include <Volcano/Net/Server.hpp>
#include <Volcano/Net/Session.hpp>
#include <Volcano/Net/ClientWorld.hpp>

#include <Volcano/Launcher/Common.hpp>
#include <Volcano/Launcher/NetworkAccessManagerFactory.hpp>

VOLCANO_LAUNCHER_BEGIN

template <typename T>
static void registerQmlAnonymousType(const char *uri)
{
    qmlRegisterAnonymousType<T>(uri, VOLCANO_VERSION_MAJOR);
}

template <typename T>
static void registerQmlUncreatableType(const char *uri, const char *qmlName)
{
    qmlRegisterUncreatableType<T>(uri,
        VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, qmlName,
        QString("Cannot create instance of type '%1.%2'.").arg(uri).arg(qmlName));
}

template <typename T>
static void registerQmlType(const char *uri, const char *qmlName)
{
    qmlRegisterType<T>(uri,
        VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, qmlName);
}

static void registerQmlTypes(void)
{
    const char *uri;

    ///////////////////////////////////////////////////////////////////////////
    uri = "Volcano";

    registerQmlType<Rotation>(uri, "Rotation");
    registerQmlUncreatableType<Shape>(uri, "Shape");
    registerQmlType<Box>(uri, "Box");
    registerQmlType<Sphere>(uri, "Sphere");
    registerQmlType<Cylinder>(uri, "Cylinder");
    registerQmlType<Plane>(uri, "Plane");

    ///////////////////////////////////////////////////////////////////////////
    uri = "Volcano.Game";

    registerQmlUncreatableType<Game::WorldBase>(uri, "WorldBase");
    registerQmlType<Game::World>(uri, "World");
    registerQmlType<Game::DynamicWorld>(uri, "DynamicWorld");
    registerQmlUncreatableType<Game::Object>(uri, "Object");
    registerQmlType<Game::Entity>(uri, "Entity");
    registerQmlUncreatableType<Game::Component>(uri, "Component");
    registerQmlType<Game::Visual>(uri, "Visual");
    registerQmlType<Game::Material>(uri, "Material");
    registerQmlType<Game::Mesh>(uri, "Mesh");
    registerQmlType<Game::Collision>(uri, "Collision");
    registerQmlUncreatableType<Game::Light>(uri, "Light");
    registerQmlType<Game::DirectionalLight>(uri, "DirectionalLight");
    registerQmlType<Game::PointLight>(uri, "PointLight");
    registerQmlType<Game::SpotLight>(uri, "SpotLight");

    ///////////////////////////////////////////////////////////////////////////
    uri = "Volcano.Graphics";

    registerQmlUncreatableType<Graphics::Camera>(uri, "Camera");
    registerQmlType<Graphics::CameraView>(uri, "CameraView");

    ///////////////////////////////////////////////////////////////////////////
    uri = "Volcano.Net";

    registerQmlType<Net::Server>(uri, "Server");
    registerQmlUncreatableType<Net::Session>(uri, "Session");
    registerQmlType<Net::ClientWorld>(uri, "ClientWorld");
}

static int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    setApplicationName("VolcanoLauncher");

    qInfo("VolcanoEngine %s", VOLCANO_VERSION_STR);

    QUrl indexUrl;
    QString gameDirectory;

    {
        QCommandLineParser cmdline;
        cmdline.addHelpOption();
        cmdline.addVersionOption();
        cmdline.addPositionalArgument("gameDir", "Game root directory.");
        cmdline.process(app);

        auto args = cmdline.positionalArguments();
        if (!args.isEmpty()) {
            if (args.size() > 1)
                return EXIT_FAILURE;
            gameDirectory = args[0];
        } else
            gameDirectory = QDir::currentPath();

        QFileInfo gameDirectoryInfo(gameDirectory);
        if (!gameDirectoryInfo.isDir()) {
            qCritical("Game dir %s is not a directory.", qPrintable(gameDirectory));
            return EXIT_FAILURE;
        }

        auto indexPath = gameDirectory + "/Index.qml";
        QFileInfo indexPathInfo(indexPath);
        if (!indexPathInfo.isFile()) {
            qCritical("Invalid game dir: %s", qPrintable(gameDirectory));
            return EXIT_FAILURE;
        }

        indexUrl = QUrl::fromLocalFile(indexPath);
    }

    registerQmlTypes();
    Graphics::initDefaultSettings();

    auto networkAccessManagerFactory =
            std::make_unique<NetworkAccessManagerFactory>();
    if (!networkAccessManagerFactory)
        return EXIT_FAILURE;

    auto qmlApplicationEngine = std::make_unique<QQmlApplicationEngine>();
    if (!qmlApplicationEngine)
        return EXIT_FAILURE;

    qmlApplicationEngine->setOutputWarningsToStandardError(true);
    qmlApplicationEngine->setNetworkAccessManagerFactory(networkAccessManagerFactory.get());
    qmlApplicationEngine->load(indexUrl);

    return app.exec();
}

VOLCANO_LAUNCHER_END

int main(int argc, char *argv[])
{
    return Volcano::Launcher::main(argc, argv);
}
