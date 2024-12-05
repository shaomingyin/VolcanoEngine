//
//
#ifndef EIGEN_H
#define EIGEN_H

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>

namespace Eigen {
    template <typename T>
    using Affine2 = Transform<T, 2, Eigen::Affine>;

    template <typename T>
    using Affine3 = Transform<T, 3, Eigen::Affine>;

    using Quaternionf = Quaternion<float>;
    using Quaterniond = Quaternion<double>;
}

#endif // EIGEN_H
