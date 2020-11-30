//
//
#include <Volcano/ScopeGuard.hpp>
#include <Volcano/Graphics/Renderer.hpp>

#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg_gl.h>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(void):
    m_target(nullptr),
    m_nvg(nullptr)
{
}

Renderer::~Renderer(void)
{
    if (m_target != nullptr) {
        m_target->activate();
        shutdown();
        m_target->deactivate();
    }
}

bool Renderer::init(Surface *target)
{
    VOLCANO_ASSERT(target != nullptr);
    VOLCANO_ASSERT(target->isActivated());
    VOLCANO_ASSERT(m_target == nullptr);
    VOLCANO_ASSERT(m_nvg == nullptr);

    m_nvg = nvgCreateGL3(0);
    if (m_nvg == nullptr)
        return false;

    VOLCANO_SCOPE_EXIT(r1) {
        nvgDeleteGL3(m_nvg);
        m_nvg = nullptr;
    };

#if 0
    if (!m_ddRenderer.init())
        return false;

    VOLCANO_SCOPE_EXIT(r3) { m_ddRenderer.shutdown(); };

    if (!dd::initialize(&m_ddContext, &m_ddRenderer))
        return false;
#endif

    glClearColor(0.3f, 0.3f, 0.3f, 0.0f);

    r1.dismiss();
    //r3.dismiss();

    m_target = target;

	return true;
}

void Renderer::shutdown(void)
{
    VOLCANO_ASSERT(m_target != nullptr);
    VOLCANO_ASSERT(m_target->isActivated());

    // TODO

#if 0
    dd::shutdown(m_ddContext);
    m_ddRenderer.shutdown();
#endif

    if (m_nvg != nullptr) {
        nvgDeleteGL3(m_nvg);
        m_nvg = nullptr;
    }
}

void Renderer::render(const View &v)
{
    VOLCANO_ASSERT(m_target != nullptr);
    VOLCANO_ASSERT(m_target->isActivated());
    VOLCANO_ASSERT(m_nvg != nullptr);

    glViewport(0, 0, m_target->width(), m_target->height());
    glClear(GL_COLOR_BUFFER_BIT);

    // m_program.use();
    // setup view matrix uniform.

#if 0
    m_ddRenderer.resize(m_target->size());
    dd::flush(m_ddContext, int64_t(v.elapsed() * 1000.0f));
#endif
}

VOLCANO_GRAPHICS_END
