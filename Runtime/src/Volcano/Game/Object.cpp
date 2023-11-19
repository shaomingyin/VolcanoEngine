//
//
#include <Volcano/Game/Object.hpp>

VOLCANO_GAME_BEGIN

Object::Object(QObject *parent)
    : QObject(parent) {
}

bool Object::isEnabled(void) const {
    return isEnabled_;
}

void Object::setEnabled(bool v) {
    if (isEnabled_ != v) {
        isEnabled_ = v;
        emit enableChanged(v);
    }
}

VOLCANO_GAME_END
