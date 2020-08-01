//
//
#include <QDir>
#include <QUrl>
#include <QString>
#include <QScopedPointer>
#include <QCommandLineParser>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <Volcano/Node.hpp>
#include <Volcano/Entity.hpp>
#include <Volcano/Camera.hpp>
#include <Volcano/Scene.hpp>
#include <Volcano/Mesh.hpp>
#include <Volcano/Light.hpp>
#include <Volcano/DirectionalLight.hpp>
#include <Volcano/PointLight.hpp>
#include <Volcano/SpotLight.hpp>
#include <Volcano/Viewport.hpp>

template <typename T>
void registerVolcanoType(const char *typeName)
{
    qmlRegisterType<T>("Volcano", VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, typeName);
}

template <typename T>
void registerUncreatableVolcanoType(const char *typeName)
{
    qmlRegisterUncreatableType<T>("Volcano", VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, typeName,
        QString("Cannot create instance of '%1'.").arg(typeName));
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QGuiApplication::setApplicationName("Volcano");
    QGuiApplication::setApplicationVersion(QString("%1.%2").arg(VOLCANO_VERSION_MAJOR).arg(VOLCANO_VERSION_MINOR));

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("url", "Specifies the index qml url, default is 'index.qml'.");
    parser.process(app);

    QUrl url;
    const QStringList urls = parser.positionalArguments();

    if (urls.size() < 1)
        url = QUrl::fromUserInput("./index.qml", QDir::currentPath());
    else if (urls.size() == 1)
        url = QUrl::fromUserInput(urls[0], QDir::currentPath());
    else
        return EXIT_FAILURE;

    qDebug() << "URL: " << url.toString();

    registerUncreatableVolcanoType<Volcano::Node>("Node");
    registerUncreatableVolcanoType<Volcano::Light>("Light");

    registerVolcanoType<Volcano::Entity>("Entity");
    registerVolcanoType<Volcano::Camera>("Camera");
    registerVolcanoType<Volcano::Scene>("Scene");
    registerVolcanoType<Volcano::Mesh>("Mesh");
    registerVolcanoType<Volcano::DirectionalLight>("DirectionalLight");
    registerVolcanoType<Volcano::PointLight>("PointLight");
    registerVolcanoType<Volcano::SpotLight>("SpotLight");
    registerVolcanoType<Volcano::Viewport>("Viewport");

    QScopedPointer<QQmlApplicationEngine> engine(new QQmlApplicationEngine(url));
    if (!engine)
        return EXIT_FAILURE;

    return app.exec();
}
