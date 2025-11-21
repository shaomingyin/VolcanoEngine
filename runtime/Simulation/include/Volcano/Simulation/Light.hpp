//
//
#ifndef VOLCANO_SIMULATION_LIGHT_HPP
#define VOLCANO_SIMULATION_LIGHT_HPP

#include <Volcano/Math.hpp>
#include <Volcano/Simulation/Common.hpp>

VOLCANO_SIMULATION_BEGIN

class Light {
public:
    Light():
        color_(1.0f, 1.0f, 1.0f, 1.0f) {
    }

    Light(const Light& other)
        : color_(other.color_) {
    }

    Light(const Eigen::Vector4f& color)
        : color_(color) {
    }

public:
    Light& operator=(const Light& other) {
        color_ = other.color_;
        return (*this);
    }

    const Eigen::Vector4f& color() const noexcept {
        return color_;
    }

    void setColor(const Eigen::Vector4f& v) noexcept {
        color_ = v;
    }

private:
    Eigen::Vector4f color_;
};

VOLCANO_SIMULATION_END

#endif // VOLCANO_SIMULATION_LIGHT_HPP
