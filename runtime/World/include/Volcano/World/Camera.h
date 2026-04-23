//
//
#ifndef VOLCANO_WORLD_CAMERA_H
#define VOLCANO_WORLD_CAMERA_H

#include <Volcano/Math.h>
#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

struct Camera {
    Eigen::Matrix4f toMatrix() const noexcept {
        auto m = Eigen::Matrix4f::Identity();
        return m;
    }

    float znear;
    float zfar;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_CAMERA_H
