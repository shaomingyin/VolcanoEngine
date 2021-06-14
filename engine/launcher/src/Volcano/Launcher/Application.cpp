//
//
#include <QDir>
#include <QUrl>
#include <QString>
#include <QFileInfo>
#include <QCommandLineParser>

#include <Volcano/Launcher/Application.hpp>

VOLCANO_LAUNCHER_BEGIN

Application::Application(int &argc, char *argv[]):
    QGuiApplication(argc, argv)
{
    connect(&m_window, &QQuickView::statusChanged, this, &Application::onWindowStatusChanged);
}

Application::~Application(void)
{
}

bool Application::init(void)
{
    QCommandLineParser cmdline;

    cmdline.addPositionalArgument("game-dir", "Specifies the game root directory.");
    cmdline.addHelpOption();
    cmdline.addVersionOption();
    cmdline.process(*this);

    QString gameDir = QDir::currentPath();
    auto args = cmdline.positionalArguments();
    if (!args.empty()) {
        if (args.size() != 1) {
            cmdline.showHelp(EXIT_FAILURE);
            return false;
        }
        gameDir = args[0];
    }

    QFileInfo fi(gameDir);
    if (!fi.isDir()) {
        qFatal("Invalid game directory '%s'.", qPrintable(gameDir));
        return false;
    }

    if (!gameDir.endsWith('/'))
        gameDir += '/';

    auto index = gameDir + "index.qml";
    if (!QFileInfo::exists(index)) {
        qFatal("Cannot find '%s'.", qPrintable(index));
    }

    QSize size(800, 600);
    m_window.setMinimumSize(size);
    m_window.setMaximumSize(size);

    m_window.create();
    m_window.setSource(QUrl::fromUserInput(index));

    startTimer(16, Qt::PreciseTimer);

    return true;
}

void Application::timerEvent(QTimerEvent *event)
{
    m_window.update();
}

void Application::onWindowStatusChanged(QQuickView::Status st)
{
    if (st == QQuickView::Error)
        quit();

    if (st == QQuickView::Ready)
        setup();
}

void Application::setup(void)
{
    m_window.show();
}

VOLCANO_LAUNCHER_END
