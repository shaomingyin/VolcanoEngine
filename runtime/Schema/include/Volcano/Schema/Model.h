#ifndef VOLCANO_SCHEMA_MODEL_H
#define VOLCANO_SCHEMA_MODEL_H

#include <filesystem>
#include <vector>

#include <Eigen/Geometry>

#include <Volcano/Schema/Common.h>
#include <Volcano/Stream.h>

VOLCANO_SCHEMA_BEGIN

struct Model final {
public:
    struct Part final {
        std::filesystem::path mesh;
        std::filesystem::path material;
        Eigen::Affine3f transform = Eigen::Affine3f::Identity();
    };

public:
    void validate() const;
    static Model read(Stream& stream);
    void write(Stream& stream) const;

public:
    std::vector<Part> parts;
};

VOLCANO_SCHEMA_END

#endif // VOLCANO_SCHEMA_MODEL_H
