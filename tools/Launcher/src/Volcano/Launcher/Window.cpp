//
//
#include <SFML/Window/Context.hpp>

#include <Volcano/Launcher/Window.h>

VOLCANO_LAUNCHER_BEGIN

Window::Window(sf::VideoMode mode, const sf::String& title, uint32_t style, const sf::ContextSettings& settings)
    : sf::RenderWindow(mode, title, style, sf::State::Windowed, settings) {
    spdlog::info("Initializing window...");
    spdlog::info("Making current GL context...");
    if (!setActive(true)) {
        spdlog::error("Failed to make current GL context.");
    }
    spdlog::info("Initializing GL3W...");
    int ret = gl3wInit(&gl3w_, [](const char* name) {
        return GL3WglProc(sf::Context::getFunction(name));
    });
    if (ret != GL3W_OK) {
        spdlog::error("Failed to initialize GL3W.");
        throw std::runtime_error("Failed to init GL3W.");
    }
    gl3wProcs = &gl3w_;
}

VOLCANO_LAUNCHER_END
