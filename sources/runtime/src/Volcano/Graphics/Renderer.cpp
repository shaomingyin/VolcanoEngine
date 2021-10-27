//
//
#include <QThread>
#include <QQuickOpenGLUtils>

#include <Volcano/Graphics/Context.hpp>
#include <Volcano/Graphics/CameraView.hpp>
#include <Volcano/Graphics/Renderer.hpp>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(void):
    m_gl(nullptr),
    m_clearEnabled(true),
    m_clearColor(QColor::fromRgb(0, 0, 0)),
    m_viewPosition(0.0f, 0.0f, 0.0f),
    m_viewDirection(0.0f, 0.0f, -1.0f),
    m_viewUp(0.0f, 1.0f, 0.0f)
{
}

Renderer::~Renderer(void)
{
    // TODO release opengl resources...
}

bool Renderer::init(void)
{
    Q_ASSERT(m_gl == nullptr);

    m_gl = Context::current()->glFunctions();
    if (m_gl == nullptr)
        return false;

    return true;
}

void Renderer::render(void)
{
    Q_ASSERT(m_gl != nullptr);

    if (m_clearEnabled) {
        m_gl->glClearColor(m_clearColor.redF(), m_clearColor.greenF(), m_clearColor.blueF(), m_clearColor.alphaF());
        m_gl->glClear(GL_COLOR_BUFFER_BIT);
    }

    // TODO render visible set...

    QQuickOpenGLUtils::resetOpenGLState();
}

void Renderer::synchronize(QQuickFramebufferObject *item)
{
    Q_ASSERT(m_gl != nullptr);

#ifdef VOLCANO_DEBUG
    auto cameraView = qobject_cast<CameraView *>(item);
#else
    auto cameraView = static_cast<CameraView *>(item);
#endif

    Q_ASSERT(cameraView != nullptr);

    auto gameWorld = cameraView->gameWorld();
    if (Q_UNLIKELY(gameWorld == nullptr))
        return;

    m_clearEnabled = cameraView->isBackgroundEnabled();
    if (m_clearEnabled)
        m_clearColor = cameraView->backgroundColor();

    auto camera = cameraView->camera();
    m_viewPosition = camera->position();
    m_viewDirection = camera->direction();
    m_viewUp = camera->up();

    // TODO build visible set from 'gameWorld'.
}

VOLCANO_GRAPHICS_END
