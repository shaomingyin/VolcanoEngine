//
//
#ifndef VOLCANO_GRAPHICS_FRAME_H
#define VOLCANO_GRAPHICS_FRAME_H

#include <vector>

#include <Volcano/Math.h>
#include <Volcano/World/Schema.pb.h>
#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Pass.h>

VOLCANO_GRAPHICS_BEGIN

class Frame final {
public:
    enum class Bool {
        Clear = 0,
        AmbientLight,
        Max
    };

    enum class Color {
        Clear = 0,
        AmbientLight,
        Max
    };

    enum class Vector3f {
        Max
    };

    enum class Affine3f {
        Model = 0,
        View,
        Max
    };

    enum class Vector4i {
        Max
    };

    enum class Matrix4f {
        Projection = 0,
        Max
    };

public:
    Frame();

public:
    void reset() noexcept;
    void commit(int x, int y, int width, int height) const noexcept;

    void add(const World::Light& v) noexcept {
        lights_.push_back(v);
    }

    void add(const Transform& transform) noexcept {
    }

#define OPS(T, K, M) \
    const T& get(K k) const noexcept { return M[static_cast<size_t>(k)]; } \
    T& get(K k) noexcept { return M[static_cast<size_t>(k)]; } \
    T operator[](K k) const noexcept { return get(k); } \
    T& operator[](K k) noexcept { return get(k); }

    OPS(bool, Bool, bool_)
    OPS(sf::Color, Color, color_)
    OPS(Eigen::Affine3f, Affine3f, affine3f_)
    //OPS(Eigen::Vector4i, Vector4i, vector4i_)
    OPS(Eigen::Matrix4f, Matrix4f, matrix4f_)

private:
    bool bool_[static_cast<size_t>(Bool::Max)];
    sf::Color color_[static_cast<size_t>(Color::Max)];
    Eigen::Affine3f affine3f_[static_cast<size_t>(Affine3f::Max)];
    //Eigen::Vector4i vector4i_[static_cast<size_t>(Vector4i::Max)];
    Eigen::Matrix4f matrix4f_[static_cast<size_t>(Matrix4f::Max)];

private:
    std::vector<World::Light> lights_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_FRAME_H
