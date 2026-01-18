//
//
#ifndef VOLCANO_GAME_TRANSFORM_HPP
#define VOLCANO_GAME_TRANSFORM_HPP

#include <Volcano/Math.hpp>
#include <Volcano/Game/Common.hpp>

VOLCANO_GAME_BEGIN

class Transform {
    Eigen::Vector3f position;
    Eigen::Vector3f scale;
    Eigen::Quaternionf rotation;

    Eigen::Matrix4f toMatrix() const {
        auto ret(Eigen::Affine3f::Identity());
        ret.scale(scale);
        ret.rotate(rotation);
        ret.translate(position);
        return ret.matrix();
    }

    void toMatrix(Eigen::Matrix4f& out) const {
        auto ret(Eigen::Affine3f::Identity());
        ret.scale(scale);
        ret.rotate(rotation);
        ret.translate(position);
        out = ret.matrix();
    }

    void LoadIdentity() {
        position = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
        scale = Eigen::Vector3f(1.0f, 1.0f, 1.0f);
        rotation = Eigen::Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
    }
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_TRANSFORM_HPP
