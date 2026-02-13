//
//
#ifndef VOLCANO_WORLD_TRANSFORM_H
#define VOLCANO_WORLD_TRANSFORM_H

#include <Volcano/Math.h>
#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

struct Transform {
    Eigen::Vector3f translation = { 0.0f, 0.0f, 0.0f };
    Eigen::Vector3f scale = { 1.0f, 1.0f, 1.0f };
    Eigen::Quaternionf rotation = { 0.0f, 0.0f, 0.0f, 1.0f };

    Eigen::Matrix4f matrix() const noexcept {
        auto m(Eigen::Affine3f::Identity());
        m.scale(scale);
        m.rotate(rotation);
        m.translate(translation);
        return m.matrix();
    }

    void Reset() noexcept {
        translation = { 0.0f, 0.0f, 0.0f };
        scale = { 1.0f, 1.0f, 1.0f };
        rotation = { 0.0f, 0.0f, 0.0f, 1.0f };
    }
};

inline Transform operator*(const Transform& a, const Transform& b) noexcept {
    Transform ret = a;
    ret.translation += b.translation;
    ret.scale *= b.scale;
    ret.rotation *= b.rotation;
    return ret;
}

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_TRANSFORM_H
