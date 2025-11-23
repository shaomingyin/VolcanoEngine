//
//
#include <Volcano/Game/Context.hpp>

VOLCANO_GAME_BEGIN

Context* Context::current_ = nullptr;

Context::~Context() {
    if (current_ == this) {
        current_ = nullptr;
    }
}

void Context::makeCurrent() {
    current_ = this;
}

VOLCANO_GAME_END
