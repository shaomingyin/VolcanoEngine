//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_H
#define VOLCANO_GRAPHICS_RENDERER_H

#include <array>
#include <bitset>

#include <Volcano/Color.h>
#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/View.h>
#include <Volcano/Graphics/Target.h>

VOLCANO_GRAPHICS_BEGIN

class Renderer {
public:
    enum class BoolAttribute {
        Clear = 0,
        Max
    };

    enum class Vector4fAttribute {
        ClearColor = 0,
        Max
    };

public:
    Renderer();
    virtual ~Renderer();

public:
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

    void set(Vector4fAttribute attr, Eigen::Vector4f v) {
        vector4f_attributes_[static_cast<int>(attr)] = v;
    }

    void render(const View& view, Target& target);

private:
    std::bitset<static_cast<size_t>(BoolAttribute::Max)> bool_attributes_;
    std::array<Eigen::Vector4f, static_cast<size_t>(Vector4fAttribute::Max)> vector4f_attributes_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_H
