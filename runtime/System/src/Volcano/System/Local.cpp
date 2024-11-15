//
//
#include <Volcano/ScopeGuard.h>
#include <Volcano/Graphics/Mesh.h>
#include <Volcano/System/Local.h>

VOLCANO_SYSTEM_BEGIN

Local::Local()
    : window_("Untitled", 1024, 768, Window::FlagResizable | Window::FlagClear)
    , input_(window_.id())
    , graphics_renderer_(scene())
    , acoustics_space_(scene())
    , console_({ 0, 0, 1024.0f, 768.0f / 2.0f }) {
}

Local::~Local() {
}

void Local::handleEvent(const SDL_Event& evt) {
    window_.handleEvent(evt);
    input_.handleEvent(evt);
}

void Local::loadingFrame(Duration elapsed) {
    int progress = loadingProgress();
    const std::string& text = loadingText();
}

void Local::readyFrame(Duration elapsed) {
}

void Local::playingFrame(Duration elapsed) {
    acoustics_space_.frame(elapsed);
    if (window_.beginFrame()) {
        graphics_renderer_.frame(elapsed);
        window_.endFrame();
    }
}

void Local::pausedFrame(Duration elapsed) {
}

void Local::stoppingFrame(Duration elapsed) {
}

void Local::errorFrame(Duration elapsed) {
}

void Local::loadEntity(entt::handle ent, const nlohmann::json& json) {
    Base::loadEntity(ent, json);

    auto mesh_it = json.find("mesh");
    if (mesh_it != json.end()) {
        if (!mesh_it->is_object()) {
            throw Error(Errc::InvalidType);
        }
        auto mesh_source_it = mesh_it->find("source");
        if (mesh_source_it == mesh_it->end() || !mesh_source_it->is_string()) {
            throw Error(Errc::NotExisted);
        }
        //ent.emplaceOrReplace<Graphics::Mesh>(mesh_source_it->get<std::string>());
    }
}

VOLCANO_SYSTEM_END
