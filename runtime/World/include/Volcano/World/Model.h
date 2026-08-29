//
//
#ifndef VOLCANO_WORLD_MODEL_H
#define VOLCANO_WORLD_MODEL_H

#include <memory>
#include <vector>

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Material;
class Mesh;

class Model final {
public:
    struct Part final {
        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<Material> material;
        Transform offset;
    };

public:
    Model() noexcept;
    Model(const Model&) = default;
    Model(Model&&) noexcept = default;

public:
    Model& operator=(const Model&) = default;
    Model& operator=(Model&&) noexcept = default;

    const std::vector<Part>& parts() const noexcept;
    std::vector<Part>& parts() noexcept;

    void add(const Part& part);
    void add(Part&& part);
    bool empty() const noexcept;

private:
    std::vector<Part> parts_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_MODEL_H
