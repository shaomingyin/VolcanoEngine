//
//
#ifndef VOLCANO_GRAPHICS_COMMON_HPP
#define VOLCANO_GRAPHICS_COMMON_HPP

#include <Volcano/Common.hpp>

#ifdef VOLCANO_GRAPHICS
#   define VOLCANO_GRAPHICS_API Q_DECL_EXPORT
#else
#   define VOLCANO_GRAPHICS_API Q_DECL_IMPORT
#endif

#define VOLCANO_GRAPHICS_BEGIN VOLCANO_BEGIN namespace Graphics {
#define VOLCANO_GRAPHICS_END } VOLCANO_END

namespace Eigen {
    template <typename Scalar>
    using Vector2 = Vector<Scalar, 2>;

    template <typename Scalar>
    using Vector3 = Vector<Scalar, 3>;

    template <typename Scalar>
    using Vector4 = Vector<Scalar, 4>;

    template <typename Scalar>
    using Matrix2x2 = Matrix<Scalar, 2, 2>;

    template <typename Scalar>
    using Matrix3x3 = Matrix<Scalar, 3, 3>;

    template <typename Scalar>
    using Matrix4x4 = Matrix<Scalar, 4, 4>;

    template <typename Scalar>
    using Matrix2x2 = Matrix2x2<Scalar>;

    template <typename Scalar>
    using Matrix3x3 = Matrix3x3<Scalar>;

    template <typename Scalar>
    using Matrix4x4 = Matrix4x4<Scalar>;

    template <typename Scalar>
    using Affine3 = Transform<Scalar, 3, Affine>;

    template <typename Scalar>
    using AffineCompact3 = Transform<Scalar, 3, AffineCompact>;

    template <typename Scalar>
    using Projective3 = Transform<Scalar, 3, Projective>;

    template <typename Scalar>
    using Isometry3 = Transform<Scalar, 3, Isometry>;

    using Matrix2x2f = Matrix2x2<float>;
    using Matrix3x3f = Matrix3x3<float>;
    using Matrix4x4f = Matrix4x4<float>;
    using Affine3f = Affine3<float>;
    using AffineCompact3f = AffineCompact3<float>;
    using Projective3f = Projective3<float>;
    using Isometry3f = Isometry3<float>;

    template<typename Scalar>
    Matrix4x4<Scalar> perspective(Scalar fovy, Scalar aspect, Scalar zNear, Scalar zFar){
        Projective3f tr;
        tr.matrix().setZero();
        assert(aspect > 0);
        assert(zFar > zNear);
        assert(zNear > 0);
        Scalar radf = M_PI * fovy / 180.0;
        Scalar tan_half_fovy = std::tan(radf / 2.0);
        tr(0,0) = 1.0 / (aspect * tan_half_fovy);
        tr(1,1) = 1.0 / (tan_half_fovy);
        tr(2,2) = - (zFar + zNear) / (zFar - zNear);
        tr(3,2) = - 1.0;
        tr(2,3) = - (2.0 * zFar * zNear) / (zFar - zNear);
        return tr.matrix();
    }

    template<typename Scalar>
    Matrix4x4<Scalar> scale(Scalar x, Scalar y, Scalar z){
        Affine3f tr;
        tr.matrix().setZero();
        tr(0,0) = x;
        tr(1,1) = y;
        tr(2,2) = z;
        tr(3,3) = 1;
        return tr.matrix();
    }

    template<typename Scalar>
    Matrix4x4<Scalar> translate(Scalar x, Scalar y, Scalar z){
        Affine3f tr;
        tr.matrix().setIdentity();
        tr(0,3) = x;
        tr(1,3) = y;
        tr(2,3) = z;
        return tr.matrix();
    }

    template<typename Derived>
    auto lookAt(const Derived& eye, const Derived& center, const Derived& up){
        using M = Matrix4x4<typename Derived::Scalar>;
        using V = Vector3<typename Derived::Scalar>;
        V f = (center - eye).normalized();
        V u = up.normalized();
        V s = f.cross(u).normalized();
        u = s.cross(f);
        M m = M::Zero();
        m(0, 0) = s.x();
        m(0, 1) = s.y();
        m(0, 2) = s.z();
        m(0, 3) = -s.dot(eye);
        m(1, 0) = u.x();
        m(1, 1) = u.y();
        m(1, 2) = u.z();
        m(1, 3) = -u.dot(eye);
        m(2, 0) = -f.x();
        m(2, 1) = -f.y();
        m(2, 2) = -f.z();
        m(2, 3) = f.dot(eye);
        m.row(3) << 0,0,0,1;
        return m;
    }

    template<typename Scalar>
    auto ortho(Scalar left, Scalar right, Scalar bottom, Scalar top, Scalar zNear, Scalar zFar) {
        auto m = Matrix4x4<Scalar>::Identity();
        m(0, 0) = Scalar(2) / (right - left);
        m(1, 1) = Scalar(2) / (top - bottom);
        m(2, 2) = - Scalar(2) / (zFar - zNear);
        m(3, 0) = - (right + left) / (right - left);
        m(3, 1) = - (top + bottom) / (top - bottom);
        m(3, 2) = - (zFar + zNear) / (zFar - zNear);
        return m;
    }
}

#endif // VOLCANO_GRAPHICS_COMMON_HPP
