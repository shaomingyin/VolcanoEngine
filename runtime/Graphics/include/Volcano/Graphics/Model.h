#ifndef VOLCANO_GRAPHICS_MODEL_H
#define VOLCANO_GRAPHICS_MODEL_H

#include <filesystem>
#include <utility>
#include <vector>

#include <Eigen/Geometry>

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Material.h>
#include <Volcano/Graphics/Mesh.h>

VOLCANO_GRAPHICS_BEGIN

class Model final {
public:
    struct Part final {
    public:
        const Mesh* mesh = nullptr;
        Material material{};
        Eigen::Affine3f transform = Eigen::Affine3f::Identity();
        bool transparent = false;
    };

public:
    Model() = default;

    explicit Model(std::filesystem::path source, std::vector<Part> parts = {})
        : source_(std::move(source)), parts_(std::move(parts)) {
    }

    const std::filesystem::path& source() const noexcept {
        return source_;
    }

    const std::vector<Part>& parts() const noexcept {
        return parts_;
    }

private:
    std::filesystem::path source_;
    std::vector<Part> parts_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_MODEL_H
