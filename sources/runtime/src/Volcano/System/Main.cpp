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

#include <Volcano/NetworkAccessManagerFactory.hpp>

#include <Volcano/Game/World.hpp>
#include <Volcano/Game/DynamicWorld.hpp>
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
#include <Volcano/Graphics/World.hpp>
#include <Volcano/System/Common.hpp>
#include <Volcano/System/ClientWorld.hpp>
#include <Volcano/System/Server.hpp>

VOLCANO_SYSTEM_BEGIN

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
    uri = "Volcano.Game";

    registerQmlUncreatableType<Game::Object>(uri, "Object");
    registerQmlUncreatableType<Game::Component>(uri, "Component");
    registerQmlUncreatableType<Graphics::Light>(uri, "Light");
    registerQmlType<Game::Entity>(uri, "Entity");
    registerQmlType<Game::DirectionalLight>(uri, "DirectionalLight");
    registerQmlType<Game::PointLight>(uri, "PointLight");
    registerQmlType<Game::SpotLight>(uri, "SpotLight");
    registerQmlType<Game::Material>(uri, "Material");
    registerQmlType<Game::Mesh>(uri, "Mesh");
    registerQmlType<Game::World>(uri, "World");
    registerQmlType<Game::DynamicWorld>(uri, "DynamicWorld");

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
    registerQmlType<Graphics::World>(uri, "World");

    ///////////////////////////////////////////////////////////////////////////
    uri = "Volcano.System";

    registerQmlType<ClientWorld>(uri, "ClientWorld");
    registerQmlType<Server>(uri, "Server");
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

    QGuiApplication::setOrganizationName("VolcanoEngine");
    QGuiApplication::setApplicationVersion(VOLCANO_VERSION_STR);
    QGuiApplication::setApplicationName("Volcano");

    qInfo("VolcanoEngine %s", VOLCANO_VERSION_STR);

    QString gameDir;
    QUrl indexUrl;

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
            gameDir = args[0];
        } else
            gameDir = QDir::currentPath();

        auto indexPath = gameDir + "/Index.qml";
        QFileInfo info(indexPath);
        if (!info.isFile()) {
            qCritical("Invalid game dir: %s", qPrintable(gameDir));
            return EXIT_FAILURE;
        }

        indexUrl = QUrl::fromLocalFile(indexPath);
    }

    if (!indexUrl.isValid())
        return EXIT_FAILURE;

    registerQmlTypes();
    initDefaultGraphicsSettings();

    QScopedPointer<NetworkAccessManagerFactory> networkAccessManagerFactory(
        new NetworkAccessManagerFactory(gameDir, QStringList()));
    if (!networkAccessManagerFactory)
        return EXIT_FAILURE;

    QScopedPointer<QQmlApplicationEngine> engine(new QQmlApplicationEngine());
    if (!engine)
        return EXIT_FAILURE;

    engine->setOutputWarningsToStandardError(true);
    engine->setNetworkAccessManagerFactory(networkAccessManagerFactory.get());

    engine->load(indexUrl);

    return app.exec();
}

VOLCANO_SYSTEM_END

int main(int argc, char *argv[])
{
    return Volcano::System::main(argc, argv);
}
