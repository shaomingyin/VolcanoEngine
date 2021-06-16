//
//
#include <Volcano/Component.hpp>
#include <Volcano/DirectionalLight.hpp>
#include <Volcano/Entity.hpp>
#include <Volcano/Light.hpp>
#include <Volcano/Object.hpp>
#include <Volcano/PointLight.hpp>
#include <Volcano/SpotLight.hpp>
#include <Volcano/World.hpp>
#include <Volcano/Mesh.hpp>
#include <Volcano/Graphics/Camera.hpp>

#include <Volcano/Qml/Common.hpp>

VOLCANO_QML_BEGIN

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


void registerAll(void)
{
    const char *uri = "Volcano";

    registerUncreatableType<Object>(uri, "Object");
    registerUncreatableType<Component>(uri, "Component");
    registerUncreatableType<Light>(uri, "Light");

    registerType<Entity>(uri, "Entity");
    registerType<DirectionalLight>(uri, "DirectionalLight");
    registerType<PointLight>(uri, "PointLight");
    registerType<SpotLight>(uri, "SpotLight");
    registerType<World>(uri, "World");
    registerType<Mesh>(uri, "Mesh");

    uri = "Volcano.Graphics";

    registerType<Graphics::Camera>(uri, "Camera");

#if 0
    uri = "Volcano.Editor";
    registerType<>(editor tools);
#endif
}

VOLCANO_QML_END
