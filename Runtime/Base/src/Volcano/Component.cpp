//
//
#include <Volcano/Component.hpp>

VOLCANO_BEGIN

Component::Component(QObject* parent)
    : Actor(parent) {
}

Component::Component(Context& context, QObject* parent)
    : Actor(context, parent) {
}

VOLCANO_END
