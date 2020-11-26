//
//
#include <Volcano/ScopeGuard.hpp>
#include <Volcano/Graphics/Renderer.hpp>

#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg_gl.h>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(void):
    m_target(nullptr),
    m_vg(nullptr)
{
}

Renderer::~Renderer(void)
{
    shutdown();
}

bool Renderer::init(Surface *target)
{
    VOLCANO_ASSERT(m_vg == nullptr);
    VOLCANO_ASSERT(m_target == nullptr);
    VOLCANO_ASSERT(target != nullptr);

    if (!target->begin())
		return false;

    VOLCANO_SCOPE_EXIT(r1) {
        target->end();
    };

    m_vg = nvgCreateGL3(0);
    if (m_vg == nullptr)
        return false;

    VOLCANO_SCOPE_EXIT(r2) {
        nvgDeleteGL3(m_vg);
        m_vg = nullptr;
    };

#if 0
    if (!m_ddRenderer.init())
        return false;

    VOLCANO_SCOPE_EXIT(r3) { m_ddRenderer.shutdown(); };

    if (!dd::initialize(&m_dd, &m_ddRenderer))
        return false;
#endif

    glClearColor(0.3f, 0.3f, 0.3f, 0.0f);

    r1.dismiss();
    r2.dismiss();
    //r3.dismiss();

    target->end();
    m_target = target;

	return true;
}

void Renderer::shutdown(void)
{
    if (m_target != nullptr) {
        m_target->begin();

        // TODO
#if 0
        dd::shutdown(m_dd);
        m_ddRenderer.shutdown();
#endif

        if (m_vg != nullptr) {
            nvgDeleteGL3(m_vg);
            m_vg = nullptr;
        }

        m_target->end();
        m_target = nullptr;
    }
}

void Renderer::render(const View &v)
{
    VOLCANO_ASSERT(m_vg != nullptr);
	VOLCANO_ASSERT(m_target != nullptr);

	if (!m_target->begin())
		return;

    glViewport(0, 0, m_target->width(), m_target->height());
    glClear(GL_COLOR_BUFFER_BIT);

    // m_program.use();
    // setup view matrix uniform.

#if 0
    m_ddRenderer.resize(m_target->size());
    dd::flush(m_dd, int64_t(v.elapsed() * 1000.0f));
#endif

	m_target->end();
}

VOLCANO_GRAPHICS_END
