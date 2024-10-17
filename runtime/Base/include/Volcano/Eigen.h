//
//
#ifndef VOLCANO_EIGEN_H
#define VOLCANO_EIGEN_H

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>

namespace Eigen {
	using Quaternionf = Quaternion<float>;
	using Transform3f = Transform<float, 3, Affine>;
}

#endif // VOLCANO_EIGEN_H
