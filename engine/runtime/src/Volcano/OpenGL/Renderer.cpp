//
//
#include <Volcano/OpenGL/Renderer.hpp>

VOLCANO_OPENGL_BEGIN

Renderer::Renderer(void):
    m_gl(nullptr),
    m_gBuffer(nullptr),
    m_positionTexture(nullptr),
    m_colorTexture(nullptr),
    m_normalTexture(nullptr),
    m_depthTexture(nullptr),
    m_texCoordTexture(nullptr)
{
}

Renderer::~Renderer(void)
{
    // TODO release all resources.
    releaseGBuffer();
}

bool Renderer::init(int width, int height)
{
    Q_ASSERT(m_gl == nullptr);
    Q_ASSERT(width > 0 && height > 0);

    m_gl = currentFunctions();
    if (m_gl == nullptr)
        return false;

    m_size.setWidth(width);
    m_size.setHeight(height);

    updateGBuffer();

    return true;
}

const QSize &Renderer::size(void)
{
    return m_size;
}

void Renderer::resize(const QSize &v)
{
    if (m_size == v)
        return;

    updateGBuffer();

    m_size = v;
}

void Renderer::beginFrame(void)
{
}

void Renderer::endFrame(void)
{
    Q_ASSERT(m_gl != nullptr);

    if (Q_UNLIKELY(m_gBuffer == nullptr)) {
        updateGBuffer();
        if (m_gBuffer == nullptr)
            return;
    }

    m_gl->glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    m_gl->glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::updateGBuffer(void)
{
    Q_ASSERT(m_gl != nullptr);

    if (m_gBuffer != nullptr) {
        if (m_gBuffer->size() == m_size)
            return;
        releaseGBuffer();
    }

    if (m_size.isEmpty())
        return;

    // TODO create gbuffer.
}

void Renderer::releaseGBuffer(void)
{
    Q_ASSERT(m_gBuffer != nullptr);

    m_gBuffer = nullptr;
}

VOLCANO_OPENGL_END
