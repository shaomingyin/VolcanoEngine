//
//
#include <QDir>
#include <QUrl>
#include <QString>
#include <QCommandLineParser>
#include <QObject>
#include <QGuiApplication>
#include <QWindow>

#include "MainWindow.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QGuiApplication::setApplicationName("VolcanoLauncher");
    QGuiApplication::setApplicationVersion(QString("%1.%2").arg(VOLCANO_VERSION_MAJOR).arg(VOLCANO_VERSION_MINOR));
    QGuiApplication::setQuitOnLastWindowClosed(true);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("url", "Specifies the index qml url, default is 'index.qml'.");
    parser.process(app);

    QUrl url;
    const QStringList urls = parser.positionalArguments();

    if (urls.size() < 1)
        url = QUrl::fromLocalFile(QDir::currentPath() + "/index.qml");
    else if (urls.size() == 1)
        url = QUrl::fromUserInput(urls[0], QDir::currentPath());
    else
        return EXIT_FAILURE;

    qInfo() << "URL:" << url.toString();

    qDebug() << "Creating main window...";

    MainWindow *mainWindow(new MainWindow());
    if (mainWindow == nullptr)
        return EXIT_FAILURE;

    mainWindow->resize(800, 600);
    mainWindow->show();

    qDebug() << "Running...";

    int ret = app.exec();
    delete mainWindow;

    return ret;
}
