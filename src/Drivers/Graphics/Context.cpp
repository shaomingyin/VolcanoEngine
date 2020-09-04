//
//
#include <Volcano/Drivers/Graphics/Context.hpp>

VOLCANO_DRIVERS_GRAPHICS_BEGIN

Context::Context(QObject *parent):
    QObject(parent),
    m_glContext(nullptr)
{
}

Context::~Context(void)
{
    if (m_glContext == nullptr)
        return;

    m_glContext->makeCurrent(&m_releaseSurface);
    //m_renderer.release();
    //m_memory.release();
}

bool Context::init(void)
{
    QOpenGLContext *glContext = QOpenGLContext::currentContext();
    if (glContext == nullptr)
        return false;

    m_memory.init();
    m_renderer.init();

    m_releaseSurface.setFormat(glContext->format());
    m_releaseSurface.create();

    m_glContext = glContext;

    return true;
}

VOLCANO_DRIVERS_GRAPHICS_END
