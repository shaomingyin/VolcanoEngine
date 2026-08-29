//
//
#ifndef VOLCANO_GRAPHICS_FRAME_H
#define VOLCANO_GRAPHICS_FRAME_H

#include <vector>

#include <Volcano/Math.h>
#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class Material;
class Mesh;

class Frame final {
public:
    enum class Bool {
        Clear = 0,
        AmbientLight,
        Max
    };

    enum class Vector3f {
        AmbientLight = 0,
        CameraPosition,
        Max
    };

    enum class Vector4f {
        ClearColor = 0,
        AmbientLight,
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

    struct Item final {
        const Mesh* mesh = nullptr;
        const Material* material = nullptr;
        Transform transform;
    };

    struct Light final {
        enum class Type {
            Directional = 0,
            Point,
            Spot
        };

        Type type = Type::Directional;
        Eigen::Vector3f position = { 0.0f, 0.0f, 0.0f };
        Eigen::Vector3f direction = { 0.0f, -1.0f, 0.0f };
        Eigen::Vector3f color = { 1.0f, 1.0f, 1.0f };
        float strength = 1.0f;
        float range = 10.0f;
        float inner_angle = 15.0f;
        float outer_angle = 30.0f;
    };

public:
    Frame();

public:
    void reset() noexcept;
    void commit(int x, int y, int width, int height) const noexcept;

    void add(const Transform& transform) noexcept;
    void add(const Transform& transform, const Mesh& mesh, const Material* material = nullptr);
    void add(const Item& item);
    void add(const Light& light);

    void clearItems() noexcept {
        items_.clear();
    }

    void clearLights() noexcept {
        lights_.clear();
    }

    const std::vector<Item>& items() const noexcept {
        return items_;
    }

    std::vector<Item>& items() noexcept {
        return items_;
    }

    const std::vector<Light>& lights() const noexcept {
        return lights_;
    }

    std::vector<Light>& lights() noexcept {
        return lights_;
    }

    void setViewport(int width, int height) noexcept {
        width_ = width;
        height_ = height;
    }

    int width() const noexcept {
        return width_;
    }

    int height() const noexcept {
        return height_;
    }

#define OPS(T, K, M) \
    const T& get(K k) const noexcept { return M[static_cast<size_t>(k)]; } \
    T& get(K k) noexcept { return M[static_cast<size_t>(k)]; } \
    T operator[](K k) const noexcept { return get(k); } \
    T& operator[](K k) noexcept { return get(k); }

    OPS(bool, Bool, bool_)
    OPS(Eigen::Vector3f, Vector3f, vector3f_)
    OPS(Eigen::Vector4f, Vector4f, vector4f_)
    OPS(Eigen::Matrix4f, Matrix4f, matrix4f_)
    OPS(Eigen::Affine3f, Affine3f, affine3f_)

private:
    bool bool_[static_cast<size_t>(Bool::Max)];
    Eigen::Vector3f vector3f_[static_cast<size_t>(Vector3f::Max)];
    Eigen::Vector4f vector4f_[static_cast<size_t>(Vector4f::Max)];
    Eigen::Matrix4f matrix4f_[static_cast<size_t>(Matrix4f::Max)];
    Eigen::Affine3f affine3f_[static_cast<size_t>(Affine3f::Max)];

    std::vector<Item> items_;
    std::vector<Light> lights_;
    int width_ = 1;
    int height_ = 1;

private:
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_FRAME_H
