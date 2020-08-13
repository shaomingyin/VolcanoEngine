//
//
#include <QSurfaceFormat>
#include <QQmlEngine>

#include <Volcano.hpp>

VOLCANO_BEGIN

template <typename T>
void registerType(const char *typeName)
{
    qmlRegisterType<T>("Volcano", VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, typeName);
}

template <typename T>
void registerUncreatableType(const char *typeName)
{
    qmlRegisterUncreatableType<T>("Volcano", VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, typeName,
        QString("Cannot create instance of '%1'.").arg(typeName));
}

VOLCANO_API bool init(void)
{
    qDebug() << "Initializing default OpenGL surface format...";

    QSurfaceFormat format;

    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setRedBufferSize(8);
    format.setGreenBufferSize(8);
    format.setBlueBufferSize(8);
    format.setAlphaBufferSize(8);
    format.setDepthBufferSize(24);

#ifdef VOLCANO_DEBUG
    format.setOption(QSurfaceFormat::DebugContext, 1);
#endif

    QSurfaceFormat::setDefaultFormat(format);

    qDebug() << "Registering Volcano QML types...";

    registerUncreatableType<Volcano::Node>("Node");
    registerUncreatableType<Volcano::Light>("Light");

    registerType<Game>("Game");
    registerType<Entity>("Entity");
    registerType<Camera>("Camera");
    registerType<Scene>("Scene");
    registerType<Mesh>("Mesh");
    registerType<DirectionalLight>("DirectionalLight");
    registerType<PointLight>("PointLight");
    registerType<SpotLight>("SpotLight");
    registerType<Viewport>("Viewport");

    return true;
}

VOLCANO_END
