//
//
#include <Volcano/World/Cylinder.h>

VOLCANO_WORLD_BEGIN

Cylinder::Cylinder(QObject* parent)
    : Shape(parent)
    , size_(1.0f, 1.0f, 1.0f) {
}

const QVector3D& Cylinder::size() const noexcept {
    return size_;
}

void Cylinder::resize(const QVector3D& v) noexcept {
    if (size_ != v) {
        size_ = v;
        emit sizeChanged(v);
    }
}

VOLCANO_WORLD_END
