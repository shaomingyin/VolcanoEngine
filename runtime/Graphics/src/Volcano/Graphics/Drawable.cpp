//
//
#include <Volcano/Graphics/Drawable.h>

VOLCANO_GRAPHICS_BEGIN

Drawable::Drawable(Context& context, QObject* parent)
    : QObject(parent)
    , context_(context)
    , flags_(FlagEnabled | FlagVisible)
    , detail_level_(1.0f) {
}

void Drawable::enable() {
    flags_ |= FlagEnabled;
}

void Drawable::disable() {
    flags_ &= ~FlagEnabled;
}


void Drawable::show() {
    flags_ |= FlagVisible;
}

void Drawable::hide() {
    flags_ &= ~FlagVisible;
}

void Drawable::setDetailLevel(float level) {
    detail_level_ = level;
}

void Drawable::tick() {
}

void Drawable::draw() {
}

VOLCANO_GRAPHICS_END
