//
//
#include <Volcano/World/Component.h>

VOLCANO_WORLD_BEGIN

Component::Component(QObject* parent)
    : QObject(parent) {
}

Component::~Component() {
}

entt::handle Component::handle() noexcept {
    return handle_;
}

void Component::attach(entt::handle handle) {
    handle_ = handle;
}

void Component::detach() {
}

VOLCANO_WORLD_END
