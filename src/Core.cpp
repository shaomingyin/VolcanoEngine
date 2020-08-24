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
    qDebug() << "Registering Volcano QML types...";

    registerType<Node>("Node");
    registerType<World>("World");
    registerUncreatableType<WorldAmbient>("WorldAmbient");
    registerType<Entity>("Entity");
    registerType<Scene>("Scene");
    registerUncreatableType<Resource>("Resource");
    registerType<Model>("Model");
    registerUncreatableType<Camera>("Camera");
    registerType<OrthoCamera>("OrhtoCamera");
    registerType<PerpectiveCamera>("PerpectiveCamera");
    registerUncreatableType<Light>("Light");
    registerType<DirectionalLight>("DirectionalLight");
    registerType<PointLight>("PointLight");
    registerType<SpotLight>("SpotLight");

    return true;
}

VOLCANO_END
