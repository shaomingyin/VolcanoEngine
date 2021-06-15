//
//
#include <QString>
#include <QStringList>
#include <QScopedPointer>
#include <QUrl>
#include <QDir>
#include <QObject>
#include <QSurfaceFormat>
#include <QQmlApplicationEngine>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QGuiApplication>

#include <Volcano/Component.hpp>
#include <Volcano/DirectionalLight.hpp>
#include <Volcano/Entity.hpp>
#include <Volcano/Light.hpp>
#include <Volcano/Object.hpp>
#include <Volcano/PointLight.hpp>
#include <Volcano/Scene.hpp>
#include <Volcano/SpotLight.hpp>
#include <Volcano/World.hpp>
#include <Volcano/Mesh.hpp>

#include <Volcano/OpenGL/Common.hpp>
#include <Volcano/Graphics/Camera.hpp>
#include <Volcano/Graphics/PerpectiveCamera.hpp>
#include <Volcano/Graphics/WorldView.hpp>

#include <Volcano/System/Common.hpp>

VOLCANO_SYSTEM_BEGIN

template <typename T>
static void registerUncreatableType(const char *uri, const char *qmlName)
{
    qmlRegisterUncreatableType<T>(uri, VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, qmlName,
        QString("Cannot create instance of type '%1'.").arg(qmlName));
}

template <typename T>
static void registerType(const char *uri, const char *qmlName)
{
    qmlRegisterType<T>(uri, VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, qmlName);
}

static void registerTypes(void)
{
    const char *uri = "Volcano";

    registerUncreatableType<Object>(uri, "Object");
    registerUncreatableType<Component>(uri, "Component");
    registerUncreatableType<Light>(uri, "Light");

    registerType<Entity>(uri, "Entity");
    registerType<DirectionalLight>(uri, "DirectionalLight");
    registerType<PointLight>(uri, "PointLight");
    registerType<SpotLight>(uri, "SpotLight");
    registerType<Scene>(uri, "Scene");
    registerType<World>(uri, "World");
    registerType<Mesh>(uri, "Mesh");

    uri = "Volcano.Graphics";

    registerType<Graphics::WorldCache>(uri, "WorldCache");
    registerType<Graphics::WorldView>(uri, "WorldView");
    registerType<Graphics::Camera>(uri, "Camera");
    registerType<Graphics::PerpectiveCamera>(uri, "PerpectiveCamera");

#if 0
    uri = "Volcano.Editor";
    registerType<>(editor tools);
#endif
}

static void setDefaultSurfaceFormat(void)
{
    QSurfaceFormat format;
    OpenGL::makeSurfaceFormat(format);
    QSurfaceFormat::setDefaultFormat(format);
}

static int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QGuiApplication::setOrganizationName("VolcanoEngine");
    QGuiApplication::setApplicationName("Volcano");
    QGuiApplication::setApplicationVersion(VOLCANO_VERSION_STR);

    qInfo("VolcanoEngine %s", VOLCANO_VERSION_STR);

    QCommandLineParser cmdlineParser;

    QCommandLineOption urlOption(QStringList() << "u" << "url", "Specifies the url, default to 'file://[cwd]/index.qml", "URL");
    cmdlineParser.addOption(urlOption);

    cmdlineParser.addHelpOption();
    cmdlineParser.addVersionOption();

    cmdlineParser.process(app);

    QUrl url = QUrl::fromLocalFile(QDir::currentPath()).toString() + "/index.qml";
    if (cmdlineParser.isSet(urlOption)) {
        url = QUrl::fromUserInput(cmdlineParser.value(urlOption));
        if (!url.isValid()) {
            qFatal("Invalid url input: '%s'.", qPrintable(url.toString()));
            return EXIT_FAILURE;
        }
    }

    QScopedPointer<QQmlApplicationEngine> appEngine(new QQmlApplicationEngine());
    if (!appEngine)
        return EXIT_FAILURE;

    QObject::connect(appEngine.get(), &QQmlApplicationEngine::objectCreated, [&app](QObject *object, const QUrl &url) {
        if (object == nullptr)
            app.quit();
    });

    registerTypes();
    setDefaultSurfaceFormat();

    qInfo("Loading from %s...", qPrintable(url.toString()));
    qDebug() << appEngine->importPathList();

    appEngine->load(url);

    return app.exec();
}

VOLCANO_SYSTEM_END

int main(int argc, char *argv[])
{
    return Volcano::System::main(argc, argv);
}
