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
#include <Volcano/Graphics/CameraView.hpp>

#include <Volcano/System/Common.hpp>
#include <Volcano/System/Context.hpp>
#include <Volcano/System/Client.hpp>
#include <Volcano/System/Server.hpp>
#include <Volcano/System/GameWindow.hpp>
#include <Volcano/System/NetworkAccessManagerFactory.hpp>

VOLCANO_SYSTEM_BEGIN

template <typename T>
static void registerQmlUncreatableType(const char *uri, const char *qmlName)
{
    qmlRegisterUncreatableType<T>(uri, VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, qmlName,
        QString("Cannot create instance of type '%1.%2'.").arg(uri).arg(qmlName));
}

template <typename T>
static void registerQmlType(const char *uri, const char *qmlName)
{
    qmlRegisterType<T>(uri, VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, qmlName);
}

static void registerQmlTypes(void)
{
    const char *uri;

    ///////////////////////////////////////////////////////////////////////////
    uri = "Volcano.Game";

    registerQmlUncreatableType<Game::Object>(uri, "Object");
    registerQmlUncreatableType<Game::Component>(uri, "Component");
    registerQmlUncreatableType<Game::Light>(uri, "Light");
    registerQmlType<Game::DirectionalLight>(uri, "DirectionalLight");
    registerQmlType<Game::PointLight>(uri, "PointLight");
    registerQmlType<Game::SpotLight>(uri, "SpotLight");
    registerQmlType<Game::Material>(uri, "Material");
    registerQmlType<Game::Mesh>(uri, "Mesh");
    registerQmlType<Game::World>(uri, "World");

    ///////////////////////////////////////////////////////////////////////////
    uri = "Volcano.Graphics";

    registerQmlType<Graphics::CameraView>(uri, "CameraView");

    ///////////////////////////////////////////////////////////////////////////
    uri = "Volcano.System";

    registerQmlUncreatableType<Context>(uri, "Context");
    registerQmlType<Client>(uri, "Client");
    registerQmlType<Server>(uri, "Server");
    registerQmlType<GameWindow>(uri, "GameWindow");
}

static void initGraphicsSettings(void)
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
    QUrl manifestUrl;

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

        auto manifestPath = gameDir + "/Manifest.qml";
        QFileInfo info(manifestPath);
        if (!info.isFile()) {
            qCritical("Invalid manifest: %s", qPrintable(manifestPath));
            return EXIT_FAILURE;
        }

        manifestUrl = QUrl::fromLocalFile(manifestPath);
    }

    if (!manifestUrl.isValid())
        return EXIT_FAILURE;

    registerQmlTypes();
    initGraphicsSettings();

    QScopedPointer<NetworkAccessManagerFactory> namFactory(
        new NetworkAccessManagerFactory(gameDir, QStringList()));
    if (!namFactory)
        return EXIT_FAILURE;

    QScopedPointer<QQmlApplicationEngine> engine(new QQmlApplicationEngine());
    if (!engine)
        return EXIT_FAILURE;

    engine->setOutputWarningsToStandardError(true);
    engine->setNetworkAccessManagerFactory(namFactory.get());
    engine->load(manifestUrl);

    return app.exec();
}

VOLCANO_SYSTEM_END

int main(int argc, char *argv[])
{
    return Volcano::System::main(argc, argv);
}
