//
//
#include <SFML/Window/Context.hpp>

#include <Volcano/Launcher/Window.hpp>

VOLCANO_LAUNCHER_BEGIN

Window::Window(sf::VideoMode mode, const sf::String& title, sf::Uint32 style, const sf::ContextSettings& settings)
    : sf::RenderWindow(mode, title, style, settings) {
    logInfo("Making current context...");
    if (!setActive(true)) {
        logError("Failed to make current context.");
    }
    logInfo("Initializing GL3W...");
    int ret = gl3wInit(&gl3w_, [](const char* name) {
        return GL3WglProc(sf::Context::getFunction(name));
    });
    if (ret != GL3W_OK) {
        logError("Failed to initialize GL3W.");
        throw std::runtime_error("Failed to init GL3W.");
    }
    gl3wProcs = &gl3w_;
}

VOLCANO_LAUNCHER_END
