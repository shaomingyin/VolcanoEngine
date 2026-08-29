//
//
#ifndef VOLCANO_WORLD_BOUNDS_H
#define VOLCANO_WORLD_BOUNDS_H

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Bounds final {
public:
    Bounds() noexcept;
    Bounds(const Eigen::Vector3f& minimum, const Eigen::Vector3f& maximum) noexcept;
    Bounds(const Bounds&) noexcept = default;
    Bounds(Bounds&&) noexcept = default;

public:
    Bounds& operator=(const Bounds&) noexcept = default;
    Bounds& operator=(Bounds&&) noexcept = default;

    const Eigen::Vector3f& minimum() const noexcept;
    const Eigen::Vector3f& maximum() const noexcept;
    Eigen::Vector3f center() const noexcept;
    Eigen::Vector3f extent() const noexcept;
    bool empty() const noexcept;

    void reset() noexcept;
    void extend(const Eigen::Vector3f& point) noexcept;

private:
    Eigen::Vector3f minimum_;
    Eigen::Vector3f maximum_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_BOUNDS_H
