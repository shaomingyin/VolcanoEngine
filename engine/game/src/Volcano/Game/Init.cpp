//
//
#include <Volcano/Game/Object.hpp>
#include <Volcano/Game/Light.hpp>
#include <Volcano/Game/Component.hpp>
#include <Volcano/Game/Resource.hpp>
#include <Volcano/Game/Entity.hpp>
#include <Volcano/Game/DirectionalLight.hpp>
#include <Volcano/Game/PointLight.hpp>
#include <Volcano/Game/SpotLight.hpp>
#include <Volcano/Game/Material.hpp>
#include <Volcano/Game/Mesh.hpp>
#include <Volcano/Game/World.hpp>

#include <Volcano/Game/Init.hpp>

VOLCANO_GAME_BEGIN

bool init(void)
{
    const char *uri = "volcano.game";

    registerUncreatableType<Object>(uri, "Object");
    registerUncreatableType<Light>(uri, "Light");
    registerUncreatableType<Component>(uri, "Component");
    registerUncreatableType<Resource>(uri, "Resource");

    registerType<Entity>(uri, "Entity");
    registerType<DirectionalLight>(uri, "DirectionalLight");
    registerType<PointLight>(uri, "PointLight");
    registerType<SpotLight>(uri, "SpotLight");
    registerType<Material>(uri, "Material");
    registerType<Mesh>(uri, "Mesh");
    registerType<World>(uri, "World");

    return true;
}

VOLCANO_GAME_END
