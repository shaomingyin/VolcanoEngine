//
//
#include <QOpenGLContext>
#include <QOpenGLFunctions>

#include <Volcano/Viewport.h>
#include <Volcano/Renderer.h>

VOLCANO_BEGIN

Renderer::Renderer(void)
{
}

Renderer::~Renderer(void)
{
}

void Renderer::render(void)
{
    static bool init = false;
    static QOpenGLContext *context = QOpenGLContext::currentContext();
    static QOpenGLFunctions *gl = context->functions();

    if (!init) {
        context = QOpenGLContext::currentContext();
        gl = context->functions();
        init = true;
        qDebug() << "GLES" << context->isOpenGLES();
        qDebug() << "VER" << context->format().version();
    }

    gl->glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    gl->glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::synchronize(QQuickFramebufferObject *obj)
{
    Viewport *viewport = reinterpret_cast<Viewport *>(obj);

    Node *target = viewport->target();
    if (target == nullptr)
        return;

    Camera *camera = viewport->camera();
    if (camera == nullptr)
        return;

    // TODO setup view...
}

bool Renderer::init(void)
{
    return true;
}

VOLCANO_END
