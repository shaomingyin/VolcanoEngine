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

#include <Volcano/OpenGL/Common.hpp>
#include <Volcano/Qml/Common.hpp>
#include <Volcano/Launcher/Common.hpp>

VOLCANO_LAUNCHER_BEGIN

static void basicInit(void)
{
    Qml::registerAll();

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

    basicInit();

    qInfo("Loading from %s...", qPrintable(url.toString()));
    appEngine->load(url);

    return app.exec();
}

VOLCANO_LAUNCHER_END

int main(int argc, char *argv[])
{
    return Volcano::Launcher::main(argc, argv);
}
