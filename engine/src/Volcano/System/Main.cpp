//
//
#include <QUrl>
#include <QDir>
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

#include <Volcano/System/Common.hpp>
#include <Volcano/System/GameWindow.hpp>

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
    uri = "Volcano.System";

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

    QCommandLineParser cmdline;
    cmdline.addHelpOption();
    cmdline.addVersionOption();
    cmdline.addPositionalArgument("url", "Startup URL");

    cmdline.process(app);

    auto cwd = QDir::currentPath();
    QUrl url = QUrl::fromLocalFile(cwd + "/index.qml");
    auto args = cmdline.positionalArguments();
    if (args.size() > 0) {
        if (args.size() != 1) {
            qFatal("Too many startup url.");
            return EXIT_FAILURE;
        }
        url = QUrl::fromUserInput(args[0]);
    }

    if (!url.isValid()) {
        qFatal("Invalid startup url: '%s': %s.", qPrintable(url.toString()), qPrintable(url.errorString()));
        return EXIT_FAILURE;
    }

    registerQmlTypes();
    initGraphicsSettings();

    qInfo("Startup URL: %s", qPrintable(url.toString()));
    QQmlApplicationEngine engine(url);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, [](QObject *object, const QUrl &url) {
        if (object == nullptr) {
            qFatal("Failed to load startup URL: %s.", qPrintable(url.toString()));
        }
    });

    return app.exec();
}

VOLCANO_SYSTEM_END

int main(int argc, char *argv[])
{
    return Volcano::System::main(argc, argv);
}
