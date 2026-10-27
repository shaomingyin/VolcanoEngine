#ifndef VOLCANO_GRAPHICS_VIEW_H
#define VOLCANO_GRAPHICS_VIEW_H

#include <cstddef>
#include <functional>
#include <vector>

#include <Eigen/Geometry>

#include <Volcano/Graphics/Material.h>
#include <Volcano/Graphics/Mesh.h>
#include <Volcano/Graphics/Light.h>
#include <Volcano/Transform.h>

VOLCANO_GRAPHICS_BEGIN

struct DrawItem final {
    std::reference_wrapper<const Mesh> mesh;
    Material material{};
    Eigen::Affine3f transform = Eigen::Affine3f::Identity();
    bool transparent = false;
};

// Owns drawing state and items; Mesh and Texture resources remain externally owned.
class View final {
public:
    struct PreparedItem final {
        std::size_t index;
        Eigen::Affine3f model;
        Eigen::Matrix3f normal;
        float depth;
    };

public:
    // Starts a new submission, retaining camera and environment settings.
    void begin() noexcept;
    void addItem(const Mesh& mesh, const Material& material = {},
        const Transform& transform = {}, bool transparent = false);
    void addItem(const Mesh& mesh, const Material& material,
        const Eigen::Affine3f& transform, bool transparent = false);
    void addLight(const Light& light);
    void end();

    bool ready() const noexcept {
        return state_ == State::Ready;
    }

    const std::vector<PreparedItem>& opaqueItems() const {
        requireReady();
        return opaque_;
    }

    const std::vector<PreparedItem>& transparentItems() const {
        requireReady();
        return transparent_;
    }

    const std::vector<Light::Prepared>& preparedLights() const {
        requireReady();
        return prepared_lights_;
    }

    const std::vector<DrawItem>& items() const noexcept {
        return items_;
    }

    const std::vector<Light>& lights() const noexcept {
        return lights_;
    }

    void setAmbientLight(const Eigen::Vector3f& value) noexcept {
        invalidate();
        ambient_light_ = value;
    }

    const Eigen::Vector3f& ambientLight() const noexcept {
        return ambient_light_;
    }

    void setClearColor(const Colorf& value) noexcept {
        invalidate();
        clear_color_ = value;
    }

    const Colorf& clearColor() const noexcept {
        return clear_color_;
    }

    void setProjection(const Eigen::Matrix4f& value) noexcept {
        invalidate();
        projection_ = value;
    }

    const Eigen::Matrix4f& projection() const noexcept {
        return projection_;
    }

    void setViewMatrix(const Eigen::Matrix4f& value) noexcept {
        invalidate();
        view_ = value;
    }

    const Eigen::Matrix4f& viewMatrix() const noexcept {
        return view_;
    }

private:
    enum class State {
        Idle,
        Building,
        Ready
    };

    void requireBuilding() const;
    void requireReady() const;
    void invalidate() noexcept {
        if (state_ == State::Ready) {
            state_ = State::Idle;
        }
    }

private:
    std::vector<DrawItem> items_;
    std::vector<Light> lights_;
    Eigen::Vector3f ambient_light_ = Eigen::Vector3f::Zero();
    Colorf clear_color_ = Colorf::Black;
    Eigen::Matrix4f projection_ = Eigen::Matrix4f::Identity();
    Eigen::Matrix4f view_ = Eigen::Matrix4f::Identity();
    std::vector<PreparedItem> opaque_;
    std::vector<PreparedItem> transparent_;
    std::vector<Light::Prepared> prepared_lights_;
    State state_ = State::Idle;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_VIEW_H
