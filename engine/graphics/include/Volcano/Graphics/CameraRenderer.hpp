//
//
#ifndef VOLCANO_GAME_GRAPHICS_CAMERARARENDERER_HPP
#define VOLCANO_GAME_GRAPHICS_CAMERARARENDERER_HPP

#include <QQuickFramebufferObject>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/View.hpp>
#include <Volcano/Graphics/ViewChain.hpp>

VOLCANO_GRAPHICS_BEGIN

class CameraView;

class CameraRenderer: public QQuickFramebufferObject::Renderer {
public:
    CameraRenderer(CameraView &cameraView);
    ~CameraRenderer(void) override;

public:
    bool init(void);
    ViewChain &viewChain(void);
    void render(void) override;
    void synchronize(QQuickFramebufferObject *item) override;

private:
    CameraView &m_cameraView;
    ViewChain m_viewChain;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_HPP
