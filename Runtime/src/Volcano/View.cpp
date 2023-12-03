//
//
#include <Volcano/View.hpp>

VOLCANO_BEGIN

View::View(QObject* parent)
    : QObject(parent) {
    transform_stack_.push(Transform::identity());
}

VOLCANO_END
