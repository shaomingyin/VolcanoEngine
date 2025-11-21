//
//
#ifndef VOLCANO_UTISL_RENDERABLE_HPP
#define VOLCANO_UTISL_RENDERABLE_HPP

#include <memory>

#include <Volcano/Graphics/Renderable.hpp>
#include <Volcano/Fusion/Common.hpp>

VOLCANO_FUSION_BEGIN

struct Renderable final {
    Renderable(std::unique_ptr<Graphics::Renderable> p)
        : renderable(std::move(p)) {
    }

    std::unique_ptr<Graphics::Renderable> renderable;
};

VOLCANO_FUSION_END

#endif // VOLCANO_UTISL_RENDERABLE_HPP
