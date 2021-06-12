//
//
#include <cstring>

#include <QString>
#include <QScopedPointer>
#include <QSurfaceFormat>
#include <QQuickWindow>
#include <QCoreApplication>

#include <Volcano/OpenGL/Common.hpp>

#include <Volcano/Game/Component.hpp>
#include <Volcano/Game/DirectionalLight.hpp>
#include <Volcano/Game/Entity.hpp>
#include <Volcano/Game/Light.hpp>
#include <Volcano/Game/Object.hpp>
#include <Volcano/Game/PointLight.hpp>
#include <Volcano/Game/Scene.hpp>
#include <Volcano/Game/SpotLight.hpp>
#include <Volcano/Game/World.hpp>

#include <Volcano/Client/WorldView.hpp>

#include <Volcano/Editor/Application.hpp>
#include <Volcano/Launcher/Application.hpp>
#include <Volcano/Server/Application.hpp>

#include <Volcano/System/Common.hpp>

VOLCANO_SYSTEM_BEGIN

template <typename T>
void registerType(const char *uri, const char *qmlName)
{
    qmlRegisterType<T>(uri, VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, qmlName);
}

template <typename T>
void registerUncreatableType(const char *uri, const char *qmlName)
{
    ::qmlRegisterUncreatableType<T>(uri, VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR,
        qmlName, QString("Cannot create instance of <%1>").arg(qmlName));
}

static void registerBasicTypes(void)
{
    auto gameUri = "volcano.game";

    registerUncreatableType<Game::Object>(gameUri, "Object");
    registerUncreatableType<Game::Component>(gameUri, "Component");
    registerUncreatableType<Game::Light>(gameUri, "Light");

    registerType<Game::World>(gameUri, "World");
    registerType<Game::Scene>(gameUri, "Scene");
    registerType<Game::Entity>(gameUri, "Entity");
    registerType<Game::DirectionalLight>(gameUri, "DirectionalLight");
    registerType<Game::PointLight>(gameUri, "PointLight");
    registerType<Game::SpotLight>(gameUri, "SpotLight");
}

static void registerClientTypes(void)
{
    auto uri = "volcano.client";

    registerType<Client::WorldView>(uri, "WorldView");
}

static void registerServerTypes(void)
{
    // auto uri = "volcano.server";
}

static int editorMain(int argc, char *argv[])
{
    Editor::Application app(argc, argv);
    Editor::Application::setApplicationName("Editor");

    registerClientTypes();
    registerServerTypes();

    if (!app.init())
        return EXIT_FAILURE;

    return app.exec();
}

static int launcherMain(int argc, char *argv[])
{
    Launcher::Application app(argc, argv);
    Launcher::Application::setApplicationName("Launcher");

    registerClientTypes();

    if (!app.init())
        return EXIT_FAILURE;

    return app.exec();
}

static int serverMain(int argc, char *argv[])
{
    Server::Application app(argc, argv);
    Server::Application::setApplicationName("Server");

    registerServerTypes();

    if (!app.init())
        return EXIT_FAILURE;

    return app.exec();
}

static void printHelp(const char *argv0)
{
    qInfo("Usage:");
    qInfo("  %s [Command] [Options]", argv0);
    qInfo("Command:");
    qInfo("  edit");
    qInfo("  launch");
    qInfo("  serve");
    qInfo("  help");
    qInfo("  version");
    qInfo("Options:");
    qInfo("  see `%s [Command] --help' for details.", argv0);
}

static int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("VolcanoEngine");
    QCoreApplication::setApplicationVersion(VOLCANO_VERSION_STR);

    if (argc < 2) {
        printHelp(argv[0]);
        return EXIT_SUCCESS;
    }

    qInfo("VolcanoEngine %s", VOLCANO_VERSION_STR);

    {
        QSurfaceFormat format;
        OpenGL::makeSurfaceFormat(format);
        QSurfaceFormat::setDefaultFormat(format);
    }

    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);

    registerBasicTypes();

    if (strcmp(argv[1], "edit") == 0) {
        qInfo("Starting editor...");
        return editorMain(argc - 1, argv + 1);
    }

    if (strcmp(argv[1], "launch") == 0) {
        qInfo("Starting launcher...");
        return launcherMain(argc - 1, argv + 1);
    }

    if (strcmp(argv[1], "serve") == 0) {
        qInfo("Starting server...");
        return serverMain(argc - 1, argv + 1);
    }

    if (strcmp(argv[1], "help") == 0) {
        printHelp(argv[0]);
        return EXIT_SUCCESS;
    }

    if (strcmp(argv[1], "version") == 0)
        return EXIT_SUCCESS;

    qFatal("Unknown command: `%s', user `%s help' for details", argv[1], argv[0]);

    return EXIT_FAILURE;
}

VOLCANO_SYSTEM_END

int main(int argc, char *argv[])
{
    return Volcano::System::main(argc, argv);
}
