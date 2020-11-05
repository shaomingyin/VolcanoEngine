//
//
#include <Volcano/Game/Camera.hpp>
#include <Volcano/Game/Component.hpp>
#include <Volcano/Game/DirectionalLight.hpp>
#include <Volcano/Game/Entity.hpp>
#include <Volcano/Game/Light.hpp>
#include <Volcano/Game/Model.hpp>
#include <Volcano/Game/OrthoCamera.hpp>
#include <Volcano/Game/PerpectiveCamera.hpp>
#include <Volcano/Game/PointLight.hpp>
#include <Volcano/Game/SpotLight.hpp>
#include <Volcano/Game/World.hpp>

#include <Volcano/Game/RegisterTypes.hpp>

VOLCANO_GAME_BEGIN

void registerTypes(void)
{
    registerUncreatableType<Camera>("Camera", "Game");
    registerUncreatableType<Light>("Light", "Game");
    registerUncreatableType<Component>("Component", "Game");

    registerType<DirectionalLight>("DirectionalLight", "Game");
    registerType<Entity>("Entity", "Game");
    registerType<Model>("Model", "Game");
    registerType<OrthoCamera>("OrthoCamera", "Game");
    registerType<PerpectiveCamera>("PerpectiveCamera", "Game");
    registerType<PointLight>("PointLight", "Game");
    registerType<SpotLight>("SpotLight", "Game");
    registerType<World>("World", "Game");
}

VOLCANO_GAME_END
