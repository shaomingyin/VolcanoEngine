//
//
#ifndef VOLCANO_FUSION_VIEWBUILDER_HPP
#define VOLCANO_FUSION_VIEWBUILDER_HPP

#include <Volcano/Game/Scene.hpp>
#include <Volcano/Graphics/View.hpp>
#include <Volcano/Fusion/Common.hpp>

VOLCANO_FUSION_BEGIN

class ViewBuilder {
public:
    ViewBuilder(const Game::Scene& scene);
    virtual ~ViewBuilder() = default;

public:
    const Game::Scene& scene() const noexcept {
        return scene_;
    }

    virtual void build(Graphics::View& view);

private:
    const Game::Scene& scene_;
};

VOLCANO_FUSION_END

#endif // VOLCANO_FUSION_VIEWBUILDER_HPP
