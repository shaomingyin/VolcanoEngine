//
//
#include <QDir>
#include <QUrl>
#include <QString>
#include <QStringList>
#include <QScopedPointer>
#include <QCommandLineParser>
#include <QGuiApplication>

#include "MainWindow.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QGuiApplication::setApplicationName("VolcanoLauncher");
    QGuiApplication::setApplicationVersion(QString("%1.%2.%3").arg(VOLCANO_VERSION_MAJOR).arg(VOLCANO_VERSION_MINOR));

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("source", "The source url.");

    parser.process(app);

    QUrl url = QUrl::fromLocalFile(QDir::currentPath() + "/index.qml");

    const QStringList &sourceList = parser.positionalArguments();
    if (!sourceList.empty())
    {
        if (sourceList.count() != 1)
            return EXIT_FAILURE;
        url = QUrl::fromUserInput(sourceList[0]);
    }

    if (!url.isValid())
        return EXIT_FAILURE;

    QScopedPointer<MainWindow> mainWindow(new MainWindow());
    if (!mainWindow)
        return EXIT_FAILURE;

    mainWindow->create();
    mainWindow->resize(800, 600);
    mainWindow->show();

    return app.exec();
}
