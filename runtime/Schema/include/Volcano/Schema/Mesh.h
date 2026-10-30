#ifndef VOLCANO_SCHEMA_MESH_H
#define VOLCANO_SCHEMA_MESH_H

#include <array>
#include <cstdint>
#include <vector>

#include <Volcano/Stream.h>
#include <Volcano/Schema/Common.h>

VOLCANO_SCHEMA_BEGIN

struct Mesh final {
public:
    struct Vertex final {
        std::array<float, 3> position = {0.0f, 0.0f, 0.0f};
        std::array<float, 3> normal = {0.0f, 0.0f, 1.0f};
        std::array<float, 2> texcoord = {0.0f, 0.0f};
    };

public:
    void validate() const;
    static Mesh read(Stream& stream);
    void write(Stream& stream) const;

public:
    std::vector<Vertex> vertices;
    std::vector<std::uint32_t> indices;
};

VOLCANO_SCHEMA_END

#endif // VOLCANO_SCHEMA_MESH_H
