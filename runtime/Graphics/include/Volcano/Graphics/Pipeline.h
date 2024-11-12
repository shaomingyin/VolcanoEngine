//
//
#ifndef VOLCANO_GRAPHICS_PIPELINE_H
#define VOLCANO_GRAPHICS_PIPELINE_H

#include <array>
#include <bitset>

#include <Volcano/Color.h>
#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/View.h>
#include <Volcano/Graphics/Program.h>

VOLCANO_GRAPHICS_BEGIN

class Pipeline {
public:
    enum class BoolAttribute {
        Clear = 0,
        Max
    };

    enum class Vector4fAttribute {
        ClearColor = 0,
        Max
    };

    enum class Vector4iAttribute {
        Viewport = 0,
        Max
    };

public:
    Pipeline();
    virtual ~Pipeline();

public:
    void reset();
    void apply() const;

    bool isEnabled(BoolAttribute attr) const {
        return bool_attributes_[static_cast<int>(attr)];
    }

    void enable(BoolAttribute attr) {
        bool_attributes_.set(static_cast<int>(attr));
    }

    void disable(BoolAttribute attr) {
        bool_attributes_.reset(static_cast<int>(attr));
    }

    const Eigen::Vector4f& get(Vector4fAttribute attr) const {
        return vector4f_attributes_[static_cast<int>(attr)];
    }

    const Eigen::Vector4i& get(Vector4iAttribute attr) const {
        return vector4i_attributes_[static_cast<int>(attr)];
    }

    void set(Vector4fAttribute attr, Eigen::Vector4f v) {
        vector4f_attributes_[static_cast<int>(attr)] = v;
    }

    void set(Vector4iAttribute attr, Eigen::Vector4i v) {
        vector4i_attributes_[static_cast<int>(attr)] = v;
    }

    Program& program() {
        return program_;
    }

    const Program& program() const {
        return program_;
    }

private:
    std::bitset<static_cast<size_t>(BoolAttribute::Max)> bool_attributes_;
    std::array<Eigen::Vector4f, static_cast<size_t>(Vector4fAttribute::Max)> vector4f_attributes_;
    std::array<Eigen::Vector4i, static_cast<size_t>(Vector4iAttribute::Max)> vector4i_attributes_;
    Program program_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_PIPELINE_H
