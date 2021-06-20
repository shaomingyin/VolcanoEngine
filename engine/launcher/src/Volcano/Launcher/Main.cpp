//
//
#include <QString>
#include <QStringList>
#include <QScopedPointer>
#include <QUrl>
#include <QDir>
#include <QObject>
#include <QQuickView>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QGuiApplication>

#include <Volcano/System/Common.hpp>
#include <Volcano/Launcher/Common.hpp>

VOLCANO_LAUNCHER_BEGIN

static int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    setApplicationName("Launcher");

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

    if (!System::init()) {
        qFatal("Failed to init system.");
        return EXIT_FAILURE;
    }

    QScopedPointer<QQuickView> mainWindow(new QQuickView());
    if (!mainWindow) {
        qFatal("Failed to create main window.");
        return EXIT_FAILURE;
    }

    QObject::connect(mainWindow.get(), &QQuickView::statusChanged, [&url, &mainWindow](QQuickView::Status status) {
        switch (status) {
        case QQuickView::Ready:
            mainWindow->show();
            break;
        case QQuickView::Loading:
            qInfo("Loading %s...", qPrintable(url.toString()));
            break;
        case QQuickView::Error:
            QGuiApplication::exit(EXIT_FAILURE);
            break;
        default:
            break;
        }
    });

    mainWindow->setMinimumWidth(800);
    mainWindow->setMaximumWidth(800);
    mainWindow->setMinimumHeight(600);
    mainWindow->setMaximumHeight(600);

    mainWindow->setSource(url);
    if (mainWindow->status() == QQuickView::Error)
        return EXIT_FAILURE;

    return app.exec();
}

VOLCANO_LAUNCHER_END

int main(int argc, char *argv[])
{
    return Volcano::Launcher::main(argc, argv);
}
