//
//
#include <QString>
#include <QQmlEngine>
#include <QSurfaceFormat>

#include <Volcano/Game/Component.hpp>
#include <Volcano/Game/Mesh.hpp>
#include <Volcano/Game/Object.hpp>
#include <Volcano/Game/Entity.hpp>
#include <Volcano/Game/Light.hpp>
#include <Volcano/Game/DirectionalLight.hpp>
#include <Volcano/Game/PointLight.hpp>
#include <Volcano/Game/SpotLight.hpp>
#include <Volcano/Game/World.hpp>
#include <Volcano/Game/DynamicWorld.hpp>
#include <Volcano/Game/Context.hpp>

#include <Volcano/Game/Graphics/Camera.hpp>

#include <Volcano/OpenGL/Common.hpp>
#include <Volcano/System/Common.hpp>

VOLCANO_SYSTEM_BEGIN

template <typename T>
static void registerUncreatableType(const char *uri, const char *name)
{
    qmlRegisterUncreatableType<T>(uri, VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, name,
        QString("Cannot create '%1' instance(s).").arg(name));
}

template <typename T>
static void registerType(const char *uri, const char *name)
{
    qmlRegisterType<T>(uri, VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, name);
}

bool init(void)
{
    const char *uri = "volcano.game";

    registerUncreatableType<Game::Component>(uri, "Component");
    registerType<Game::Mesh>(uri, "Mesh");

    registerUncreatableType<Game::Object>(uri, "Object");
    registerType<Game::Entity>(uri, "Entity");

    registerUncreatableType<Game::Light>(uri, "Light");
    registerType<Game::DirectionalLight>(uri, "DirectionalLight");
    registerType<Game::PointLight>(uri, "PointLight");
    registerType<Game::SpotLight>(uri, "SpotLight");

    registerType<Game::World>(uri, "World");
    registerType<Game::DynamicWorld>(uri, "DynamicWorld");

    uri = "volcano.game.graphics";

    registerType<Game::Graphics::Camera>(uri, "Camera");

    QSurfaceFormat format;
    OpenGL::makeSurfaceFormat(format);
    QSurfaceFormat::setDefaultFormat(format);

    return true;
}

VOLCANO_SYSTEM_END
