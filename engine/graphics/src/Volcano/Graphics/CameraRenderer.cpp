//
//
#include <QQuickOpenGLUtils>

#include <Volcano/Graphics/CameraView.hpp>
#include <Volcano/Graphics/CameraRenderer.hpp>

VOLCANO_GRAPHICS_BEGIN

CameraRenderer::CameraRenderer(CameraView &cameraView):
    m_cameraView(cameraView)
{
    qDebug() << __PRETTY_FUNCTION__;
}

CameraRenderer::~CameraRenderer(void)
{
}

bool CameraRenderer::init(void)
{
    // TODO init resources...

    return true;
}

ViewChain &CameraRenderer::viewChain(void)
{
    return m_viewChain;
}

void CameraRenderer::render(void)
{
    m_viewChain.consume();

    QQuickOpenGLUtils::resetOpenGLState();
}

void CameraRenderer::synchronize(QQuickFramebufferObject *item)
{
    // m_view = ...
}

VOLCANO_GRAPHICS_END
