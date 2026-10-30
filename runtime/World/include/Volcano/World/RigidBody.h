#ifndef VOLCANO_WORLD_RIGIDBODY_H
#define VOLCANO_WORLD_RIGIDBODY_H

#include <cmath>
#include <stdexcept>
#include <variant>

#include <Eigen/Core>

#include <Volcano/Transform.h>
#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class RigidBody final {
public:
    enum class Motion {
        Static,
        Kinematic,
        Dynamic
    };

    class Box final {
    public:
        Box() = default;
        explicit Box(const Eigen::Vector3f& halfExtents);

        constexpr const Eigen::Vector3f& halfExtents() const noexcept {
            return half_extents_;
        }

        void setHalfExtents(const Eigen::Vector3f& value);

    private:
        Eigen::Vector3f half_extents_ = Eigen::Vector3f::Constant(0.5f);
    };

    class Sphere final {
    public:
        constexpr Sphere() = default;

        constexpr explicit Sphere(float radius) {
            setRadius(radius);
        }

        constexpr float radius() const noexcept {
            return radius_;
        }

        constexpr void setRadius(float value) {
            if (!std::isfinite(value) || value <= 0.0f) {
                throw std::invalid_argument("RigidBody sphere radius must be finite and positive");
            }

            radius_ = value;
        }

    private:
        float radius_ = 0.5f;
    };

    class Capsule final {
    public:
        constexpr Capsule() = default;

        constexpr Capsule(float halfHeight, float radius) {
            setDimensions(halfHeight, radius);
        }

        constexpr float halfHeight() const noexcept {
            return half_height_;
        }

        constexpr float radius() const noexcept {
            return radius_;
        }

        constexpr void setDimensions(float halfHeight, float radius) {
            if (!std::isfinite(halfHeight) || halfHeight <= 0.0f) {
                throw std::invalid_argument("RigidBody capsule half height must be finite and positive");
            }
            if (!std::isfinite(radius) || radius <= 0.0f) {
                throw std::invalid_argument("RigidBody capsule radius must be finite and positive");
            }

            half_height_ = halfHeight;
            radius_ = radius;
        }

    private:
        float half_height_ = 0.5f;
        float radius_ = 0.5f;
    };

    using Shape = std::variant<Box, Sphere, Capsule>;

    constexpr const Shape& shape() const noexcept {
        return shape_;
    }

    void setShape(const Shape& value) {
        shape_ = value;
    }

    constexpr const Transform& offset() const noexcept {
        return offset_;
    }

    void setOffset(const Transform& value) noexcept {
        offset_ = value;
    }

    constexpr Motion motion() const noexcept {
        return motion_;
    }

    constexpr void setMotion(Motion value) {
        switch (value) {
        case Motion::Static:
        case Motion::Kinematic:
        case Motion::Dynamic:
            break;
        default:
            throw std::invalid_argument("Unknown RigidBody motion type");
        }
        motion_ = value;
    }

    constexpr float mass() const noexcept {
        return mass_;
    }

    constexpr void setMass(float value) {
        if (!std::isfinite(value) || value <= 0.0f) {
            throw std::invalid_argument("RigidBody mass must be finite and positive");
        }

        mass_ = value;
    }

    constexpr float friction() const noexcept {
        return friction_;
    }

    constexpr void setFriction(float value) {
        if (!std::isfinite(value) || value < 0.0f) {
            throw std::invalid_argument("RigidBody friction must be finite and nonnegative");
        }

        friction_ = value;
    }

    constexpr float restitution() const noexcept {
        return restitution_;
    }

    constexpr void setRestitution(float value) {
        if (!std::isfinite(value) || value < 0.0f || value > 1.0f) {
            throw std::invalid_argument("RigidBody restitution must be between 0 and 1");
        }

        restitution_ = value;
    }

    constexpr float linearDamping() const noexcept {
        return linear_damping_;
    }

    constexpr void setLinearDamping(float value) {
        if (!std::isfinite(value) || value < 0.0f || value > 1.0f) {
            throw std::invalid_argument("RigidBody linear damping must be between 0 and 1");
        }

        linear_damping_ = value;
    }

    constexpr float angularDamping() const noexcept {
        return angular_damping_;
    }

    constexpr void setAngularDamping(float value) {
        if (!std::isfinite(value) || value < 0.0f || value > 1.0f) {
            throw std::invalid_argument("RigidBody angular damping must be between 0 and 1");
        }

        angular_damping_ = value;
    }

    constexpr float gravityFactor() const noexcept {
        return gravity_factor_;
    }

    constexpr void setGravityFactor(float value) {
        if (!std::isfinite(value)) {
            throw std::invalid_argument("RigidBody gravity factor must be finite");
        }

        gravity_factor_ = value;
    }

private:
    Shape shape_ = Box{};
    Transform offset_;
    Motion motion_ = Motion::Dynamic;
    float mass_ = 1.0f;
    float friction_ = 0.2f;
    float restitution_ = 0.0f;
    float linear_damping_ = 0.05f;
    float angular_damping_ = 0.05f;
    float gravity_factor_ = 1.0f;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_RIGIDBODY_H
