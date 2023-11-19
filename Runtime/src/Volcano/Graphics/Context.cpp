//
//
#include <Volcano/Graphics/Context.hpp>

VOLCANO_GRAPHICS_BEGIN

Context::Context(QObject* parent)
    : QObject(parent) {
}

Context::~Context(void) {
}

bool Context::Init(const QSize& size) {
    return true;
}

void Context::Update(Duration elapsed) {
}

VOLCANO_GRAPHICS_END
