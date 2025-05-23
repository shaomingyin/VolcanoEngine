//
//
#include <Volcano/Framework/Game.h>
#include <Volcano/Framework/Local.h>

VOLCANO_FRAMEWORK_BEGIN

Local::Local(SDL_Storage* rootfs, SDL_Storage* userfs, rttr::type game_type)
    : ContextImpl(rootfs, userfs, game_type)
    , acoustics_space_(nullptr)
    , graphics_renderer_(nullptr)
    , graphics_window_(nullptr) {
    initAcoustics();
    initGraphics();
    initGui();
}

void Local::event(const SDL_Event& evt) {
    ContextImpl::event(evt);

    if (graphics_window_ != nullptr) {
        graphics_window_->handleEvent(evt);
    }
}

void Local::frame(Duration elapsed) {
    ContextImpl::frame(elapsed);

    if (acoustics_space_ != nullptr) {
        //acoustics_space_->
    }

    VOLCANO_ASSERT(graphics_renderer_ != nullptr);
    //graphics_renderer_->render(elapsed, )
}

void Local::loadConfig(const nlohmann::json& j) {
}

void Local::loadScene(const nlohmann::json& j) {
}

void Local::initAcoustics() {
    auto acoustics_space_types = rttr::type::get<Acoustics::Space>().get_derived_classes();
    if (!acoustics_space_types.empty()) {
        acoustics_space_instance_ = acoustics_space_types.begin()->create({ scene() });
        if (acoustics_space_instance_) {
            acoustics_space_ = &acoustics_space_instance_.get_value<Acoustics::Space>();
        } else {
            throw std::runtime_error("Failed to create acoustics space instance.");
        }
    }
}

void Local::initGraphics() {
    auto graphics_renderer_types = rttr::type::get<Graphics::Renderer>().get_derived_classes();
    VOLCANO_ASSERT(!graphics_renderer_types.empty());
    graphics_renderer_instance_ = graphics_renderer_types.begin()->create({ scene() });
    if (!graphics_renderer_instance_) {
        throw std::runtime_error("Failed to create graphics renderer instance.");
    }
    graphics_renderer_ = &graphics_renderer_instance_.get_value<Graphics::Renderer>();
    if (graphics_renderer_ == nullptr) {
        throw std::runtime_error("Failed to create graphics renderer.");
    }
    graphics_window_ = graphics_renderer_->createWindow("Volcano", 800, 600);
    if (graphics_window_ == nullptr) {
        throw std::runtime_error("Failed to create window.");
    }
}

void Local::initGui() {
}

VOLCANO_FRAMEWORK_END
