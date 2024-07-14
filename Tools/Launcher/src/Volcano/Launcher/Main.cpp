//
//
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QGuiApplication>
#include <QCommandLineParser>
#include <QUrl>
#include <QDir>
#include <QFileInfo>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QScopeGuard>
#include <QWindow>

#include <Volcano/Game/Object.hpp>
#include <Volcano/Game/World.hpp>
#include <Volcano/Game/Scene.hpp>
#include <Volcano/Game/DynamicScene.hpp>
#include <Volcano/Game/Actor.hpp>
#include <Volcano/Game/Light.hpp>
#include <Volcano/Game/DirectionalLight.hpp>
#include <Volcano/Game/PointLight.hpp>
#include <Volcano/Game/Scene.hpp>
#include <Volcano/Game/Entity.hpp>
#include <Volcano/Game/Component.hpp>
#include <Volcano/Game/Camera.hpp>

#include <Volcano/Launcher/Common.hpp>
#include <Volcano/Launcher/GameThread.hpp>
#include <Volcano/Launcher/OpenGLWindow.hpp>
#include <Volcano/Launcher/VulkanWindow.hpp>

template <typename T>
static void registerQmlType(const char* uri, const char* name) {
    qmlRegisterType<T>(uri, VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, name);
}

template <typename T>
static void registerUncreatableQmlType(const char* uri, const char* name) {
    qmlRegisterUncreatableType<T>(uri, VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, name,
                                  QString("Cannot create %1.%2 instance.").arg(uri).arg(name));
}

static void registerVolcanoGameTypes() {
    const char* volcano_game_uri = "Volcano.Game";

    registerUncreatableQmlType<Volcano::Game::Object>(volcano_game_uri, "Objhect");
    registerUncreatableQmlType<Volcano::Game::Actor>(volcano_game_uri, "Actor");
    registerUncreatableQmlType<Volcano::Game::Light>(volcano_game_uri, "Light");
    registerUncreatableQmlType<Volcano::Game::Component>(volcano_game_uri, "Component");

    registerQmlType<Volcano::Game::World>(volcano_game_uri, "World");
    registerQmlType<Volcano::Game::Scene>(volcano_game_uri, "Scene");
    registerQmlType<Volcano::Game::DynamicScene>(volcano_game_uri, "DynamicScene");
    registerQmlType<Volcano::Game::DirectionalLight>(volcano_game_uri, "DirectionalLight");
    registerQmlType<Volcano::Game::PointLight>(volcano_game_uri, "PointLight");
    registerQmlType<Volcano::Game::SpotLight>(volcano_game_uri, "SpotLight");
    registerQmlType<Volcano::Game::Entity>(volcano_game_uri, "Entity");
}

static int test() {
    QCoreApplication::addLibraryPath("D:\\Projects\\VolcanoEngine\\VolcanoEngine\\Build\\Desktop_Qt_6_6_3_MSVC2019_64bit-Debug\\bin");
    QDir::setCurrent("D:\\Projects\\VolcanoEngine\\VolcanoEngine\\Build\\Desktop_Qt_6_6_3_MSVC2019_64bit-Debug\\bin");
    QLibrary lib("D:\\Projects\\VolcanoEngine\\VolcanoEngine\\Build\\Desktop_Qt_6_6_3_MSVC2019_64bit-Debug\\lib\\qtcreator\\plugins\\VolcanoEditor.dll");
    lib.load();
    qInfo() << lib.errorString();
    return 0;
}

int main(int argc, char *argv[]) {
    qInfo("Volcano launcher - v%s", VOLCANO_VERSION_STR);
    qInfo("Starting...");

    QGuiApplication app(argc, argv);
    return test();
    QGuiApplication::setApplicationName("VolcanoLauncher");
    QGuiApplication::setApplicationDisplayName("VolcanoLauncher");
    QGuiApplication::setApplicationVersion(VOLCANO_VERSION_STR);

    QCommandLineParser args;
    args.setApplicationDescription("Volcano game launcher.");
    args.addHelpOption();
    args.addVersionOption();
    args.addPositionalArgument("game_url", "Game data url, default to './index.qml'.");
    //args.addOptions({
    //    { { "g", "graphics" }, "Specify the graphics renderer engine, [opengl|vulken]", "graphics_renderer" },
    //});
    args.process(app);

    QUrl url;
    const QStringList url_list = args.positionalArguments();
    if (!url_list.empty()) {
        if (url_list.size() != 1) {
            qFatal("Too many game urls.");
            return EXIT_FAILURE;
        }
        url = QUrl::fromUserInput(url_list.at(0));
    } else {
        auto cwd = QDir::current();
        if (!cwd.exists("index.qml")) {
            qFatal("No game url.");
            return EXIT_FAILURE;
        }
        url = QUrl::fromLocalFile(cwd.path() + "/index.qml");
    }
    if (!url.isValid()) {
        qFatal("Invalid game url '%s'.", qPrintable(url.toDisplayString()));
        return EXIT_FAILURE;
    }

    registerVolcanoGameTypes();

    qInfo("Starting game thread...");
    auto game_thread = QSharedPointer<Volcano::Launcher::GameThread>::create(std::move(url));
    game_thread->start();
    auto game_thread_guard = qScopeGuard([game_thread]() mutable {
        game_thread->exit();
        qInfo("Waiting for game thread exited...");
        game_thread->wait();
    });

    qInfo("Creating main window...");
    QString graphics_renderer = "opengl";
    if (args.isSet("graphics_renderer")) {
        graphics_renderer = args.value("graphics_renderer");
    }
    qInfo("Graphics renderer: %s", qPrintable(graphics_renderer));

    QScopedPointer<QWindow> main_window;
    if (graphics_renderer == "opengl") {
        main_window.reset(new Volcano::Launcher::OpenGLWindow);
    } else if (graphics_renderer == "vulkan") {
        main_window.reset(new Volcano::Launcher::VulkanWindow);
    } else {
        qFatal("Invalid graphics renderer: %s", qPrintable(graphics_renderer));
        return EXIT_FAILURE;
    }

    main_window->setTitle("Volcano");
    main_window->resize(1024, 768);
    main_window->show();

    return app.exec();
}
