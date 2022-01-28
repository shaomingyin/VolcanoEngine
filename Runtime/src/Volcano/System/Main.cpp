//
//
#include <QUrl>
#include <QDir>
#include <QScopedPointer>
#include <QObject>
#include <QThread>
#include <QCommandLineParser>
#include <QGuiApplication>

#include <Volcano/Transform.hpp>

#include <Volcano/Game/Object.hpp>
#include <Volcano/Game/Manifest.hpp>
#include <Volcano/Game/World.hpp>
#include <Volcano/Game/Actor.hpp>
#include <Volcano/Game/Screen.hpp>
#include <Volcano/Game/Entity.hpp>
#include <Volcano/Game/Component.hpp>
#include <Volcano/Game/Camera.hpp>
#include <Volcano/Game/CollisionShape.hpp>
#include <Volcano/Game/CollisionBox.hpp>
#include <Volcano/Game/CollisionCylinder.hpp>
#include <Volcano/Game/CollisionPlane.hpp>
#include <Volcano/Game/CollisionSphere.hpp>
#include <Volcano/Game/CollisionCompound.hpp>

#include <Volcano/Net/Session.hpp>
#include <Volcano/Net/Server.hpp>
#include <Volcano/Net/Client.hpp>

#include <Volcano/System/Common.hpp>
#include <Volcano/System/Engine.hpp>

VOLCANO_SYSTEM_BEGIN

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
    qRegisterMetaType<Transform>();

    const char *uri;

    ///////////////////////////////////////////////////////////////////////////
    uri = "Volcano";

    registerQmlType<Transform>(uri, "Transform");

    ///////////////////////////////////////////////////////////////////////////
    uri = "Volcano.Game";

    registerQmlType<Game::World>(uri, "World");
    registerQmlAnonymousType<Game::Object>(uri);
    registerQmlAnonymousType<Game::Actor>(uri);
    registerQmlType<Game::Manifest>(uri, "Manifest");
    registerQmlType<Game::Screen>(uri, "Screen");
    registerQmlType<Game::Entity>(uri, "Entity");
    registerQmlAnonymousType<Game::Component>(uri);
    registerQmlType<Game::Camera>(uri, "Camera");
    registerQmlAnonymousType<Game::CollisionShape>(uri);
    registerQmlType<Game::CollisionBox>(uri, "CollisionBox");
    registerQmlType<Game::CollisionCylinder>(uri, "CollisionCylinder");
    registerQmlType<Game::CollisionPlane>(uri, "CollisionPlane");
    registerQmlType<Game::CollisionSphere>(uri, "CollisionSphere");
    registerQmlType<Game::CollisionCompound>(uri, "CollisionCompound");

    ///////////////////////////////////////////////////////////////////////////
    // uri = "Volcano.System";
}

static int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QCoreApplication::setOrganizationName("VolcanoEngine");
    QCoreApplication::setApplicationVersion(VOLCANO_VERSION_STR);
    QCoreApplication::setApplicationName("Volcano");

    qInfo("======================================");
    qInfo("VolcanoEngine %s", VOLCANO_VERSION_STR);
    qInfo("======================================\n");

    QUrl indexUrl;
    QString gameDir;

    {
        QCommandLineParser cmdline;
        cmdline.addHelpOption();
        cmdline.addVersionOption();
        cmdline.addPositionalArgument("GameDir", "Game root directory.");
        cmdline.process(app);

        auto args = cmdline.positionalArguments();
        if (!args.isEmpty()) {
            if (args.size() > 1)
                return EXIT_FAILURE;
            gameDir = args[0];
        } else
            gameDir = QDir::currentPath();

        QFileInfo gameDirInfo(gameDir);
        if (!gameDirInfo.isDir()) {
            qCritical("Game dir %s is not a directory.", qPrintable(gameDir));
            return EXIT_FAILURE;
        }

        auto indexPath = gameDir + "/Index.qml";
        QFileInfo indexPathInfo(indexPath);
        if (!indexPathInfo.isFile()) {
            qCritical("Invalid game dir: %s", qPrintable(gameDir));
            return EXIT_FAILURE;
        }

        indexUrl = QUrl::fromLocalFile(indexPath);
    }

    qInfo("Registering Qml types...");
    registerQmlTypes();

    qInfo("Initializing engine...");
    auto engine = std::make_unique<Engine>();
    if (!engine || !engine->init(indexUrl)) {
        qFatal("Failed to initialize engine.");
        return EXIT_FAILURE;
    }

    qInfo("Running....");

    Duration diff;
    TimePoint curr = Clock::now();
    TimePoint next = curr;
    auto pEngine = engine.get();

    while (!pEngine->shouldQuit()) {
        curr = Clock::now();
        if (curr < next) {
            diff = next - curr;
            if (diff > 5ms) {
                auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
                QGuiApplication::processEvents(QEventLoop::AllEvents, ms.count());
            } else
                QThread::yieldCurrentThread();
        }
        next = pEngine->frame();
        QGuiApplication::processEvents();
    }

    return EXIT_SUCCESS;
}

VOLCANO_SYSTEM_END

int main(int argc, char *argv[])
{
    return Volcano::System::main(argc, argv);
}
