//
//
#include <QQmlEngine>

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
    registerUncreatableType<Volcano::Node>("Node");
    registerUncreatableType<Volcano::Light>("Light");

    registerType<Volcano::Entity>("Entity");
    registerType<Volcano::Camera>("Camera");
    registerType<Volcano::Scene>("Scene");
    registerType<Volcano::Mesh>("Mesh");
    registerType<Volcano::DirectionalLight>("DirectionalLight");
    registerType<Volcano::PointLight>("PointLight");
    registerType<Volcano::SpotLight>("SpotLight");
    registerType<Volcano::Viewport>("Viewport");

    return true;
}

VOLCANO_END
