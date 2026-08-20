//
//
#include <Volcano/World/StaticPlane.h>

VOLCANO_WORLD_BEGIN

StaticPlane::StaticPlane(QObject* parent)
    : Shape(parent)
    , normal_(0.0f, 1.0f, 0.0f)
    , constant_(0.0f) {
}

QVector3D StaticPlane::normal() const noexcept {
    return normal_;
}

float StaticPlane::constant() const noexcept {
    return constant_;
}

void StaticPlane::setNormal(const QVector3D& v) noexcept {
    if (normal_ != v) {
        normal_ =  v;
        emit normalChanged(v);
    }
}

void StaticPlane::setConstant(float v) noexcept {
    if (!qFuzzyCompare(constant_, v)) {
        constant_ =  v;
        emit constantChanged(v);
    }
}

VOLCANO_WORLD_END
