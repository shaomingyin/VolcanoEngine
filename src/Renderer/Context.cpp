//
//
#include <QOpenGLContext>

#include <Volcano/Renderer/Context.h>

VOLCANO_RENDERER_BEGIN

Context *Context::c_current = nullptr;

Context::Context(QObject *parent):
    QObject(parent),
    m_gl(nullptr)
{
}

Context::~Context(void)
{
    if (c_current == this)
        c_current = nullptr;
}

bool Context::init(void)
{
    Q_ASSERT(m_gl == nullptr);

    QOpenGLContext *context = QOpenGLContext::currentContext();
    if (context == nullptr)
        return false;

    m_gl = context->versionFunctions<OpenGLFunctions>();
    if (m_gl == nullptr)
        return false;

    return true;
}

void Context::render(View &view)
{
    Q_ASSERT(c_current == this);

    if (view.clear()) {
        const QColor &color = view.clearColor();
        m_gl->glClearColor(color.redF(), color.greenF(), color.blueF(), color.alphaF());
        m_gl->glClear(GL_COLOR_BUFFER_BIT);
    }
}

VOLCANO_RENDERER_END
