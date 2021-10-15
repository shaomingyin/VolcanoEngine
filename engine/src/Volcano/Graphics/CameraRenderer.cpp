//
//
#include <QQuickOpenGLUtils>

#include <Volcano/Graphics/CameraView.hpp>
#include <Volcano/Graphics/CameraRenderer.hpp>

VOLCANO_GRAPHICS_BEGIN

CameraRenderer::CameraRenderer(void):
    m_gl(nullptr),
    m_clearEnabled(true),
    m_clearColor(QColor::fromRgb(0, 0, 0))
{
}

CameraRenderer::~CameraRenderer(void)
{
    // TODO release opengl resources...
}

void CameraRenderer::render(void)
{
    if (Q_UNLIKELY(m_gl == nullptr))
        return;

    if (m_clearEnabled) {
        m_gl->glClearColor(m_clearColor.redF(), m_clearColor.greenF(), m_clearColor.blueF(), m_clearColor.alphaF());
        m_gl->glClear(GL_COLOR_BUFFER_BIT);
    }

    beginFrame();

    // TODO render visible set.

    endFrame();

    QQuickOpenGLUtils::resetOpenGLState();
}

void CameraRenderer::synchronize(QQuickFramebufferObject *item)
{
    if (Q_UNLIKELY(m_gl == nullptr)) {
        if (!init())
            return;
    }

#ifdef VOLCANO_DEBUG
    auto view = qobject_cast<CameraView *>(item);
#else
    auto view = static_cast<CameraView *>(item);
#endif

    Q_ASSERT(view != nullptr);

    m_clearEnabled = view->isBackgroundEnabled();
    if (m_clearEnabled)
        m_clearColor = view->backgroundColor();

    // TODO build visible set
}

bool CameraRenderer::init(void)
{
    Q_ASSERT(m_gl == nullptr);

    m_gl = glFunctions();
    if (m_gl == nullptr)
        return false;

    return true;
}

void CameraRenderer::beginFrame(void)
{
}

void CameraRenderer::endFrame(void)
{
}

VOLCANO_GRAPHICS_END
