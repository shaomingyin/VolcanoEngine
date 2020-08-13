//
//
#include <functional>

#include <QDir>
#include <QUrl>
#include <QString>
#include <QScopedPointer>
#include <QCommandLineParser>
#include <QObject>
#include <QGuiApplication>
#include <QQuickView>

#include <Volcano.hpp>

static void statusChanged(QQuickView *view, QQuickView::Status status)
{
    switch (status)
    {
    case QQuickView::Ready:
        if (qobject_cast<Volcano::Game *>(view->rootObject()) == nullptr)
            qGuiApp->exit(1);
        break;
    case QQuickView::Error:
        qGuiApp->exit(1);
        break;
    default:
        break;
    }
}

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

    if (urls.size() < 1)
        url = QUrl::fromLocalFile(QDir::currentPath() + "/index.qml");
    else if (urls.size() == 1)
        url = QUrl::fromUserInput(urls[0], QDir::currentPath());
    else
        return EXIT_FAILURE;

    qInfo() << "URL:" << url.toString();

    Volcano::init();

    qDebug() << "Create Qml engine...";

    QScopedPointer<QQuickView> mainWindow(new QQuickView(url));
    if (!mainWindow)
        return EXIT_FAILURE;

    QObject::connect(mainWindow.data(), &QQuickView::statusChanged,
        std::bind(&statusChanged, mainWindow.data(), std::placeholders::_1));

    mainWindow->show();

    qDebug() << "Running...";

    return app.exec();
}
