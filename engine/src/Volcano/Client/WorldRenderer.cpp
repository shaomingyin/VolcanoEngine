//
//
#include <memory>

#include <QQuickOpenGLUtils>

#include <Volcano/Client/WorldRenderer.hpp>

VOLCANO_CLIENT_BEGIN

WorldRenderer::WorldRenderer(void):
    m_glRenderer(nullptr)
{
}

WorldRenderer::~WorldRenderer(void)
{
    if (m_glRenderer != nullptr) {
        // TODO
    }
}

void WorldRenderer::render(void)
{
    qDebug("render");

    if (Q_UNLIKELY(m_glRenderer == nullptr))
        return;

    qDebug("render1 %p", OpenGL::currentFunctions());

    //OpenGL::currentFunctions()->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    //OpenGL::currentFunctions()->glClear(GL_COLOR_BUFFER_BIT);

    m_glRenderer->beginFrame();
    m_glRenderer->endFrame();

    QQuickOpenGLUtils::resetOpenGLState();
}

void WorldRenderer::synchronize(QQuickFramebufferObject *item)
{
    Q_UNUSED(item);

    qDebug("sync");

    if (Q_UNLIKELY(m_glRenderer == nullptr)) {
        auto glRenderer = std::make_unique<OpenGL::Renderer>(new OpenGL::Renderer());
        if (!glRenderer || !glRenderer->init(item->width(), item->height()))
            return;
        m_glRenderer = glRenderer.release();
    }

    // TODO
    qDebug("sync2");
}

VOLCANO_CLIENT_END
