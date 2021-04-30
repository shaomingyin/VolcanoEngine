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

bool Renderer::init(const Eigen::Vector2i &size)
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

    m_nvg = nvgCreateGL3(0);
    if (m_nvg == nullptr) {
        VOLCANO_LOGE("Failed to create NanoVG context.");
        return false;
    }

    ScopeGuard nvgGuard([&] { nvgDeleteGL3(m_nvg); m_nvg = nullptr; });

    glClearColor(0.3f, 0.3f, 0.3f, 0.0f);

    setSize(size);

    nvgGuard.dismiss();
    glexGuard.dismiss();

	return true;
}

const Eigen::Vector2i &Renderer::size(void) const
{
    return m_size;
}

void Renderer::setSize(const Eigen::Vector2i &v)
{
    m_size = v;
}

void Renderer::update(void)
{
    VOLCANO_ASSERT(m_glex != nullptr);

    glexMakeCurrent(m_glex);

    glexBeginFrame(m_size.x(), m_size.y());

    // TODO

    glexEndFrame();
}

VOLCANO_GRAPHICS_END
