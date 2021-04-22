//
//
#include <Volcano/ScopeGuard.hpp>
#include <Volcano/Graphics/Renderer.hpp>

#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg_gl.h>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(void):
    m_glex(nullptr),
    m_nvg(nullptr)
{
}

Renderer::~Renderer(void)
{
    if (m_nvg != nullptr)
        nvgDeleteGL3(m_nvg);

    if (m_glex != nullptr)
        glexDeleteContext(m_glex);
}

bool Renderer::init(int x, int y, int width, int height)
{
    VOLCANO_ASSERT(m_glex == nullptr);
    VOLCANO_ASSERT(m_nvg == nullptr);

    m_glex = glexCreateContext(this);
    if (m_glex == NULL) {
        VOLCANO_LOGE("Failed to create glex context.");
        return false;
    }

    ScopeGuard glexGuard([&] { glexDeleteContext(m_glex); m_glex = nullptr;  });

    glexMakeCurrent(m_glex);

    glexLogPrefix("Volcano GLEX ");

#ifdef VOLCANO_DEBUG
    glexLogLevel(GLEX_LOG_LEVEL_DEBUG);
#else
    glexLogLevel(GLEX_LOG_LEVEL_WARNING);
#endif

    m_nvg = nvgCreateGL3(0);
    if (m_nvg == nullptr) {
        VOLCANO_LOGE("Failed to create nanovg context.");
        return false;
    }

    ScopeGuard nvgGuard([&] { nvgDeleteGL3(m_nvg); m_nvg = nullptr; });

    glClearColor(0.3f, 0.3f, 0.3f, 0.0f);

    setViewport(x, y, width, height);

    nvgGuard.dismiss();
    glexGuard.dismiss();

	return true;
}

const Eigen::Vector4i &Renderer::viewport(void) const
{
    return m_viewport;
}

void Renderer::setViewport(int x, int y, int width, int height)
{
    VOLCANO_ASSERT(width >= 0);
    VOLCANO_ASSERT(height >= 0);

    m_viewport[0] = x;
    m_viewport[1] = y;
    m_viewport[2] = width;
    m_viewport[3] = height;
}

void Renderer::update(void)
{
    VOLCANO_ASSERT(m_glex != nullptr);

    if (m_viewport[2] < 1 || m_viewport[3] < 1)
        return;

    glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);

    glexMakeCurrent(m_glex);

    glexBeginFrame(m_viewport[2], m_viewport[3]);

    // TODO

    glexEndFrame();
}

VOLCANO_GRAPHICS_END
