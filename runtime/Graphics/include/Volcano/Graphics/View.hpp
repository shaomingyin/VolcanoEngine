//
//
#ifndef VOLCANO_GRAPHICS_VIEW_HPP
#define VOLCANO_GRAPHICS_VIEW_HPP

#include <vector>

#include <SFML/Graphics/Color.hpp>

#include <Volcano/Math.hpp>
#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/VisibleSet.hpp>

VOLCANO_GRAPHICS_BEGIN

class View: public VisibleSet {
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
    View();

public:
    bool get(Bool k) const noexcept {
        return bool_[static_cast<size_t>(k)];
    }

    void set(Bool k, bool v) noexcept {
        bool_[static_cast<size_t>(k)] = v;
    }

    const sf::Color& get(Color k) const noexcept {
        return color_[static_cast<size_t>(k)];
    }

    void set(Color k, const sf::Color& v) noexcept {
        color_[static_cast<size_t>(k)] = v;
    }

    void set(Color k, float r, float g, float b, float a = 1.0f) noexcept {
        set(k, sf::Color(r, g, b, a));
    }

    const Eigen::Affine3f& get(Affine3f k) const noexcept {
        return affine3f_[static_cast<size_t>(k)];
    }

    void set(Affine3f k, const Eigen::Affine3f& v) noexcept {
        affine3f_[static_cast<size_t>(k)] = v;
    }

private:
    bool bool_[static_cast<size_t>(Bool::Max)];
    sf::Color color_[static_cast<size_t>(Color::Max)];
    Eigen::Affine3f affine3f_[static_cast<size_t>(Affine3f::Max)];
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_VIEW_HPP
