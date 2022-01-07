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
#include <Volcano/Game/Collision.hpp>
#include <Volcano/Graphics/Camera.hpp>
#include <Volcano/Graphics/CameraView.hpp>
#include <Volcano/Graphics/Visual.hpp>
#include <Volcano/Graphics/Material.hpp>
#include <Volcano/Graphics/Mesh.hpp>
#include <Volcano/Graphics/Light.hpp>
#include <Volcano/Graphics/DirectionalLight.hpp>
#include <Volcano/Graphics/PointLight.hpp>
#include <Volcano/Graphics/SpotLight.hpp>
#include <Volcano/Net/Session.hpp>
#include <Volcano/Net/Server.hpp>
#include <Volcano/Net/Client.hpp>

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
    registerQmlType<Game::Collision>(uri, "Collision");

    ///////////////////////////////////////////////////////////////////////////
    uri = "Volcano.Graphics";

    registerQmlUncreatableType<Graphics::Camera>(uri, "Camera");
    registerQmlType<Graphics::CameraView>(uri, "CameraView");
    registerQmlType<Graphics::Visual>(uri, "Visual");
    registerQmlType<Graphics::Material>(uri, "Material");
    registerQmlType<Graphics::Mesh>(uri, "Mesh");
    registerQmlUncreatableType<Graphics::Light>(uri, "Light");
    registerQmlType<Graphics::DirectionalLight>(uri, "DirectionalLight");
    registerQmlType<Graphics::PointLight>(uri, "PointLight");
    registerQmlType<Graphics::SpotLight>(uri, "SpotLight");

    ///////////////////////////////////////////////////////////////////////////
    uri = "Volcano.Net";

    registerQmlUncreatableType<Net::Session>(uri, "Session");
    registerQmlType<Net::Server>(uri, "Server");
    registerQmlType<Net::Client>(uri, "Client");
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
