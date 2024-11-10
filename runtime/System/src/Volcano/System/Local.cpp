//
//
#include <Volcano/ScopeGuard.h>
#include <Volcano/Graphics/Mesh.h>
#include <Volcano/System/Local.h>

#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

VOLCANO_SYSTEM_BEGIN

Local::Local()
    : window_("Untitled", 1024, 768, Window::FlagResizable | Window::FlagClear)
    , input_(window_.id())
    , console_({ 0, 0, 1024.0f, 768.0f / 2.0f })
    , current_view_(0) {
}

Local::~Local() {
}

void Local::handleEvent(const SDL_Event& evt) {
    window_.handleEvent(evt);
    input_.handleEvent(evt);
}

bool Local::beginFrame() {
    if (!Base::beginFrame()) {
        return false;
    }

    if (!window_.beginFrame()) {
        return false;
    }

    glClear(GL_COLOR_BUFFER_BIT);

    // TODO

    return false;
}

void Local::endFrame() {
    window_.endFrame();
    Base::endFrame();
}

void Local::loadingFrame(Duration elapsed) {
    int progress = loadingProgress();
    const std::string& text = loadingText();

    ImGui::SetWindowPos({ 0.0f, 0.0f });
    ImGui::SetWindowSize({ float(window_.width()), float(window_.height()) });

    std::string title;
    if (text.empty()) {
        title = fmt::format("Loading... {}%", progress);
    } else {
        title = fmt::format("{} {}%", text, progress);
    }

    ImGui::SetNextWindowPos({ 0.0f, 0.0f });
    ImGui::SetNextWindowSize({ float(window_.width()), float(window_.height()) });
    if (ImGui::Begin("Loading", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground)) {
        ImGui::Text(title.c_str());
        ImGui::End();
    }
}

void Local::readyFrame(Duration elapsed) {
}

void Local::playingFrame(Duration elapsed) {
    sound_space_.frame(elapsed);
    buildView();
    renderer_.render(views_[current_view_], window_);
}

void Local::pausedFrame(Duration elapsed) {
}

void Local::stoppingFrame(Duration elapsed) {
}

void Local::errorFrame(Duration elapsed) {
}

void Local::loadEntity(World::Entity ent, const nlohmann::json& json) {
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

void Local::buildView() {
    //int exp = current_view_;
    //while (current_view_.compare_exchange_strong(exp, !current_view_));
}

VOLCANO_SYSTEM_END
