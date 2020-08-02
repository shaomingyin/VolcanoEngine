//
//
#include <QDir>
#include <QUrl>
#include <QString>
#include <QScopedPointer>
#include <QCommandLineParser>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <Volcano.hpp>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QGuiApplication::setApplicationName("VolcanoLauncher");
    QGuiApplication::setApplicationVersion(QString("%1.%2").arg(VOLCANO_VERSION_MAJOR).arg(VOLCANO_VERSION_MINOR));

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("url", "Specifies the index qml url, default is 'index.qml'.");
    parser.process(app);

    QUrl url;
    const QStringList urls = parser.positionalArguments();

    qDebug() << "url size" <<urls.size();
    if (urls.size() < 1)
        url = QUrl::fromLocalFile(QDir::currentPath() + "/index.qml");
    else if (urls.size() == 1)
        url = QUrl::fromUserInput(urls[0], QDir::currentPath());
    else
        return EXIT_FAILURE;

    qDebug() << "URL: " << url.toString();

    Volcano::init();

    QScopedPointer<QQmlApplicationEngine> engine(new QQmlApplicationEngine(url));
    if (!engine)
        return EXIT_FAILURE;

    return app.exec();
}
