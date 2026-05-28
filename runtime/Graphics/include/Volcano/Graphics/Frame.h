//
//
#ifndef VOLCANO_GRAPHICS_FRAME_H
#define VOLCANO_GRAPHICS_FRAME_H

#include <vector>

#include <Volcano/Math.h>
#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Pass.h>

VOLCANO_GRAPHICS_BEGIN

class Frame final {
public:
    enum class Bool {
        Clear = 0,
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
        View = 0,
        Max
    };

public:
    Frame();

public:
    bool get(Bool k) const noexcept {
        return bool_[static_cast<size_t>(k)];
    }

    void set(Bool k, bool v) noexcept {
        bool_[static_cast<size_t>(k)] = v;
    }

    const Eigen::Affine3f& get(Affine3f k) const noexcept {
        return affine3f_[static_cast<size_t>(k)];
    }

    void set(Affine3f k, const Eigen::Affine3f& v) noexcept {
        affine3f_[static_cast<size_t>(k)] = v;
    }

    void draw() const noexcept;

private:
    bool bool_[static_cast<size_t>(Bool::Max)];
    Eigen::Affine3f affine3f_[static_cast<size_t>(Affine3f::Max)];
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_FRAME_H
