//
//
#include <limits>

#include <Volcano/World/Bounds.h>

VOLCANO_WORLD_BEGIN

Bounds::Bounds() noexcept {
    reset();
}

Bounds::Bounds(const Eigen::Vector3f& minimum, const Eigen::Vector3f& maximum) noexcept
    : minimum_(minimum)
    , maximum_(maximum) {
}

const Eigen::Vector3f& Bounds::minimum() const noexcept {
    return minimum_;
}

const Eigen::Vector3f& Bounds::maximum() const noexcept {
    return maximum_;
}

Eigen::Vector3f Bounds::center() const noexcept {
    if (empty()) {
        return Eigen::Vector3f::Zero();
    }
    return (minimum_ + maximum_) * 0.5f;
}

Eigen::Vector3f Bounds::extent() const noexcept {
    if (empty()) {
        return Eigen::Vector3f::Zero();
    }
    return (maximum_ - minimum_) * 0.5f;
}

bool Bounds::empty() const noexcept {
    return (minimum_.array() > maximum_.array()).any();
}

void Bounds::reset() noexcept {
    const float maximum = std::numeric_limits<float>::max();
    minimum_ = Eigen::Vector3f(maximum, maximum, maximum);
    maximum_ = Eigen::Vector3f(-maximum, -maximum, -maximum);
}

void Bounds::extend(const Eigen::Vector3f& point) noexcept {
    minimum_ = minimum_.cwiseMin(point);
    maximum_ = maximum_.cwiseMax(point);
}

VOLCANO_WORLD_END
