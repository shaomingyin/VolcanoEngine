//
//
#include <Volcano/ScopeGuard.h>
#include <Volcano/Launcher/Application.h>

VOLCANO_LAUNCHER_BEGIN

Application::Application(const Options& options)
    : options_(options) {
}

Application::~Application() {
}

bool Application::init(const std::string& title, int width, int height) {
    return System::Client::init(title, width, height);
}

void Application::frame(Duration elapsed) {
}

VOLCANO_LAUNCHER_END
