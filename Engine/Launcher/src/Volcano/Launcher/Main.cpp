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
#include <QQuickView>
#include <QQmlApplicationEngine>

#include <Volcano/Net/Socket.hpp>
#include <Volcano/Net/Connection.hpp>
#include <Volcano/Net/Server.hpp>
#include <Volcano/Net/Session.hpp>

#include <Volcano/Game/World.hpp>
#include <Volcano/Game/Object.hpp>
#include <Volcano/Game/Entity.hpp>
#include <Volcano/Game/Component.hpp>
#include <Volcano/Game/Light.hpp>
#include <Volcano/Game/DirectionalLight.hpp>
#include <Volcano/Game/PointLight.hpp>
#include <Volcano/Game/SpotLight.hpp>
#include <Volcano/Game/Material.hpp>
#include <Volcano/Game/Mesh.hpp>

#include <Volcano/Graphics/Light.hpp>
#include <Volcano/Graphics/DirectionalLight.hpp>
#include <Volcano/Graphics/PointLight.hpp>
#include <Volcano/Graphics/SpotLight.hpp>
#include <Volcano/Graphics/Material.hpp>
#include <Volcano/Graphics/Mesh.hpp>
#include <Volcano/Graphics/CameraView.hpp>
#include <Volcano/Graphics/Viewable.hpp>

#include <Volcano/System/Client.hpp>
#include <Volcano/System/Server.hpp>

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

    //registerQmlAnonymousType<QIODevice>(uri);
    //registerQmlAnonymousType<QAbstractSocket>(uri);

    ///////////////////////////////////////////////////////////////////////////
    uri = "Volcano.Net";

    registerQmlUncreatableType<Net::Socket>(uri, "Socket");
    registerQmlType<Net::Connection>(uri, "Connection");
    registerQmlType<Net::Server>(uri, "Server");
    registerQmlUncreatableType<Net::Session>(uri, "Session");

    ///////////////////////////////////////////////////////////////////////////
    uri = "Volcano.Game";

    registerQmlUncreatableType<Game::Object>(uri, "Object");
    registerQmlUncreatableType<Game::Component>(uri, "Component");
    registerQmlUncreatableType<Graphics::Light>(uri, "Light");
    registerQmlType<Game::Entity>(uri, "Entity");
    registerQmlType<Game::World>(uri, "World");
    registerQmlUncreatableType<Game::Light>(uri, "Light");
    registerQmlType<Game::DirectionalLight>(uri, "DirectionalLight");
    registerQmlType<Game::PointLight>(uri, "PointLight");
    registerQmlType<Game::SpotLight>(uri, "SpotLight");
    registerQmlType<Game::Material>(uri, "Material");
    registerQmlType<Game::Mesh>(uri, "Mesh");

    ///////////////////////////////////////////////////////////////////////////
    uri = "Volcano.Graphics";

    registerQmlUncreatableType<Graphics::Light>(uri, "Light");
    registerQmlType<Graphics::DirectionalLight>(uri, "DirectionalLight");
    registerQmlType<Graphics::PointLight>(uri, "PointLight");
    registerQmlType<Graphics::SpotLight>(uri, "SpotLight");
    registerQmlType<Graphics::Material>(uri, "Material");
    registerQmlType<Graphics::Mesh>(uri, "Mesh");
    registerQmlUncreatableType<Graphics::Camera>(uri, "Camera");
    registerQmlType<Graphics::CameraView>(uri, "CameraView");
    registerQmlUncreatableType<Graphics::Viewable>(uri, "Viewable");

    ///////////////////////////////////////////////////////////////////////////
    uri = "Volcano.System";

    registerQmlType<System::Client>(uri, "Client");
    registerQmlType<System::Server>(uri, "Server");
}

static void initDefaultGraphicsSettings(void)
{
    QSurfaceFormat fmt = QSurfaceFormat::defaultFormat();

    fmt.setVersion(3, 3);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    fmt.setSwapBehavior(QSurfaceFormat::DoubleBuffer);

#ifdef VOLCANO_DEBUG
    fmt.setOption(QSurfaceFormat::DebugContext, true);
#endif

    fmt.setRedBufferSize(8);
    fmt.setGreenBufferSize(8);
    fmt.setBlueBufferSize(8);
    fmt.setDepthBufferSize(24);

    QSurfaceFormat::setDefaultFormat(fmt);

    QQuickView::setGraphicsApi(QSGRendererInterface::OpenGL);
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
    initDefaultGraphicsSettings();

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
