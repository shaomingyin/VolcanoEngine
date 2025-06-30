//
//
#include <Volcano/Framework/Game.h>
#include <Volcano/Framework/Local.h>

VOLCANO_FRAMEWORK_BEGIN

Local::Local(SDL_Storage* rootfs, SDL_Storage* userfs)
    : Base(rootfs, userfs)
    , acoustics_space_(nullptr)
    , graphics_device_(nullptr)
    , graphics_window_(nullptr) {
    initAcoustics();
    initGraphics();
    initGui();
}

void Local::event(const SDL_Event& evt) {
    if (graphics_window_ != nullptr) {
        graphics_window_->event(evt);
    }
}

void Local::frame(Duration elapsed) {
    Base::frame(elapsed);

    if (acoustics_space_ != nullptr) {
        //acoustics_space_->
    }

    VOLCANO_ASSERT(graphics_device_ != nullptr);
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
            acoustics_space_ = acoustics_space_instance_.get_value<Acoustics::Space*>();
        } else {
            throw std::runtime_error("Failed to create acoustics space instance.");
        }
    }
}

void Local::initGraphics() {
    auto graphics_devices_types = rttr::type::get<Graphics::Device>().get_derived_classes();
    VOLCANO_ASSERT(!graphics_devices_types.empty());
    graphics_device_instance_ = graphics_devices_types.begin()->create({ scene() });
    if (!graphics_device_instance_) {
        throw std::runtime_error("Failed to create graphics renderer instance.");
    }
    graphics_device_ = graphics_device_instance_.get_value<Graphics::Device*>();
    if (graphics_device_ == nullptr) {
        throw std::runtime_error("Failed to create graphics renderer.");
    }
    graphics_window_ = graphics_device_->createWindow("Volcano", 800, 600);
    if (graphics_window_ == nullptr) {
        throw std::runtime_error("Failed to create window.");
    }
}

void Local::initGui() {
}

VOLCANO_FRAMEWORK_END
