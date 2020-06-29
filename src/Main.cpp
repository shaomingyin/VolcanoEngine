//
//
#include <QString>
#include <QScopedPointer>
#include <QSurfaceFormat>
#include <QDir>
#include <QUrl>
#include <QCommandLineParser>
#include <QMainWindow>
#include <QApplication>

#include <Volcano/Common.h>
#include <Volcano/Node.h>
#include <Volcano/Entity.h>
#include <Volcano/Scene.h>
#include <Volcano/Camera.h>
#include <Volcano/Viewport.h>
#include <Volcano/Renderer.h>
#include <Volcano/Launcher.h>
#include <Volcano/Editor.h>

VOLCANO_BEGIN

static struct Options
{
    QUrl url;
    bool edit;

    Options(void):
        url(QUrl::fromUserInput("index.qml", QDir::currentPath())),
        edit(false)
    { }
} options;

static bool parseCommandLine(QCoreApplication &app)
{
    QCommandLineParser parser;

    parser.addPositionalArgument("url", "specifies the index qml url.");

    QCommandLineOption optEdit("edit", "Editor mode.");
    parser.addOption(optEdit);

    parser.addHelpOption();
    parser.addVersionOption();

    parser.process(app);

    const QStringList pargs = parser.positionalArguments();
    if (pargs.size() == 1)
        options.url = QUrl::fromUserInput(pargs[0], QDir::currentPath());

    options.edit = parser.isSet(optEdit);

    return true;
}

static void initOpenGLFormat(void)
{
    QSurfaceFormat format;

    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setRedBufferSize(8);
    format.setGreenBufferSize(8);
    format.setBlueBufferSize(8);
    format.setAlphaBufferSize(8);
    format.setDepthBufferSize(4);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);

    QSurfaceFormat::setDefaultFormat(format);
}

template <typename T>
void registerVolcanoType(const char *uri, const char *qmlName)
{
    qmlRegisterType<T>(uri, VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, qmlName);
}

static void initQmlTypes(void)
{
    registerVolcanoType<Volcano::Node>("Volcano", "Node");
    registerVolcanoType<Volcano::Entity>("Volcano", "Entity");
    registerVolcanoType<Volcano::Scene>("Volcano", "Scene");
    registerVolcanoType<Volcano::Camera>("Volcano", "Camera");
    registerVolcanoType<Volcano::Viewport>("Volcano", "Viewport");
    registerVolcanoType<Volcano::Renderer::Light>("Volcano.Renderer", "Light");
    registerVolcanoType<Volcano::Renderer::Mesh>("Volcano.Renderer", "Mesh");
}

static int exec(QCoreApplication &app)
{
    if (!parseCommandLine(app))
        return EXIT_FAILURE;

    initOpenGLFormat();
    initQmlTypes();

    QScopedPointer<Launcher> p;

    if (options.edit)
        p.reset(new Editor);
    else
        p.reset(new Launcher);

    if (!p || !p->init(options.url))
        return EXIT_FAILURE;

    p->resize(800, 600);
    p->show();

    return app.exec();
}

VOLCANO_END

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QApplication::setApplicationName("Volcano");
    QApplication::setApplicationVersion(
        QString("%1.%2.%3").
            arg(VOLCANO_VERSION_MAJOR).
            arg(VOLCANO_VERSION_MINOR).
            arg(VOLCANO_VERSION_PATCH));

    return Volcano::exec(app);
}
