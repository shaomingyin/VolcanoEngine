//
//
#include <memory>

#include <Volcano/Transform.h>
#include <Volcano/Gui/Button.h>
#include <Volcano/Gui/Document.h>
#include <Volcano/Gui/Layout.h>
#include <Volcano/World/Model.h>
#include <Volcano/World/PerspectiveCamera.h>
#include <Volcano/Game.h>
#include <Volcano/Demo/Game.h>

VOLCANO_DEMO_BEGIN

Game::Game(World::Context& context) : World::Scene(context) {
    emplace<World::PerspectiveCamera>(create(), 60.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
    auto root = std::make_unique<Gui::Layout>();
    root->setBackground({0.03f, 0.08f, 0.16f});
    auto* button = new Gui::Button(root.get());
    button->setFrame({32, 32, 192, 96});
    button->setBackground({0.1f, 0.6f, 0.3f});
    auto* control = button;
    control->setOnClick([control, active = false]() mutable {
        active = !active;
        control->setBackground(active ? Colorf{0.9f, 0.3f, 0.1f} : Colorf{0.1f, 0.6f, 0.3f});
    });

    const auto model = create();
    auto& source = emplace<World::Model>(model, "screen.vemodel");
    source.material(0).setTexture(
        World::Material::Slot::Albedo,
        std::make_unique<Gui::Document>(std::move(root), Size{256, 160}));
    emplace<Transform>(model).setTranslation({0, 0, -3});
}

Game::Game() : Game(context()) {}

Game::~Game() {
}

void Game::onUpdate(Clock::duration elapsed) noexcept {
}

entt::entity Game::player() const noexcept {
    return entt::null;
}

VOLCANO_DEMO_END

std::unique_ptr<Volcano::World::Scene> volcanoCreateGame(Volcano::World::Context& context) {
    return std::make_unique<Volcano::Demo::Game>(context);
}
