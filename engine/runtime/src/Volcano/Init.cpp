//
//
#include <QQmlEngine>

#include <Volcano/Component.hpp>
#include <Volcano/DirectionalLight.hpp>
#include <Volcano/Entity.hpp>
#include <Volcano/Light.hpp>
#include <Volcano/Object.hpp>
#include <Volcano/PointLight.hpp>
#include <Volcano/SpotLight.hpp>
#include <Volcano/World.hpp>
#include <Volcano/DynamicWorld.hpp>
#include <Volcano/Mesh.hpp>
#include <Volcano/CameraView.hpp>
#include <Volcano/OpenGLCommon.hpp>
#include <Volcano/Init.hpp>

VOLCANO_BEGIN

template <typename T>
static void registerUncreatableType(const char *uri, const char *qmlName)
{
    qmlRegisterUncreatableType<T>(uri, VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, qmlName,
        QString("Cannot create instance of type '%1'.").arg(qmlName));
}

template <typename T>
static void registerType(const char *uri, const char *qmlName)
{
    qmlRegisterType<T>(uri, VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, qmlName);
}

bool init(void)
{
    QSurfaceFormat format;
    glMakeSurfaceFormat(format);
    QSurfaceFormat::setDefaultFormat(format);

    const char *uri = "Volcano";

    registerUncreatableType<Object>(uri, "Object");
    registerUncreatableType<Component>(uri, "Component");
    registerUncreatableType<Light>(uri, "Light");

    registerType<Entity>(uri, "Entity");
    registerType<DirectionalLight>(uri, "DirectionalLight");
    registerType<PointLight>(uri, "PointLight");
    registerType<SpotLight>(uri, "SpotLight");
    registerType<World>(uri, "World");
    registerType<DynamicWorld>(uri, "DynamicWorld");
    registerType<Mesh>(uri, "Mesh");
    registerType<CameraView>(uri, "CameraView");

    return true;
}

VOLCANO_END
