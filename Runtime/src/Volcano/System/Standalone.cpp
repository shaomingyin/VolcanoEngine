//
//
#include <Volcano/System/Standalone.hpp>

VOLCANO_SYSTEM_BEGIN

Standalone::Standalone(QObject* parent)
    : Engine(parent)
    , frameTimer_(0) {
}

bool Standalone::init() {
    if (!Engine::init()) {
        return false;
    }

    if (!graphicsRenderer_.init()) {
        return false;
    }

    frameTimer_ = startTimer(16, Qt::PreciseTimer);
    elapsedTimier_.start();

    return true;
}

View& Standalone::view() {
    return graphicsRenderer_;
}

void Standalone::timerEvent(QTimerEvent* e) {
    if (e->timerId() == frameTimer_) {
        frame();
    }
}

void Standalone::frame() {
    auto elapsed = std::chrono::milliseconds(elapsedTimier_.restart());

    // TODO update world?

    graphicsRenderer_.beginFrame();
    // TODO draw world?
    graphicsRenderer_.endFrame();
}

VOLCANO_SYSTEM_END
