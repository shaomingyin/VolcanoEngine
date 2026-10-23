//
//
#ifndef VOLCANO_WORLD_TRANSFORM_H
#define VOLCANO_WORLD_TRANSFORM_H

#include <Volcano/Math/Affine.h>

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

// Physics components retain editable TRS values without decomposing a matrix.
class Transform final {
public:
    Transform() = default;

    Transform(const Math::Vector3f &translation, const Math::Vector3f &scale, const Math::Quaternionf &rotation) noexcept
        : translation_(translation), scale_(scale), rotation_(rotation) {
    }

    Math::Vector3f &translation() noexcept {
        return translation_;
    }

    const Math::Vector3f &translation() const noexcept {
        return translation_;
    }

    Math::Vector3f &scale() noexcept {
        return scale_;
    }

    const Math::Vector3f &scale() const noexcept {
        return scale_;
    }

    Math::Quaternionf &rotation() noexcept {
        return rotation_;
    }

    const Math::Quaternionf &rotation() const noexcept {
        return rotation_;
    }

    Math::Affine3f affine() const {
        // Preserve the existing component's scale-rotate-translate order.
        return Math::Affine3f().scale(scale_).rotate(rotation_).translate(translation_);
    }

    Math::Matrix4f matrix() const {
        return affine().matrix();
    }

    void reset() noexcept {
        *this = Transform();
    }

private:
    Math::Vector3f translation_;
    Math::Vector3f scale_ = Math::Vector3f(1.0f);
    Math::Quaternionf rotation_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_TRANSFORM_H
