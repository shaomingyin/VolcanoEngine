//
//
#include <memory>

#include <QThread>
#include <QOpenGLTexture>

#include <Volcano/Graphics/OpenGL/Service.hpp>
#include <Volcano/Graphics/OpenGL/QuickSurface.hpp>

VOLCANO_GRAPHICS_OPENGL_BEGIN

Service::Service(QObject *parent):
    Volcano::Graphics::Service(parent)
{
}

Service::~Service(void)
{
}

bool Service::init(int width, int height)
{
    return true;
}

void Service::resize(int width, int height)
{
}

QWindow *Service::window(void)
{
    return &m_window;
}

Mesh *Service::createMesh()
{
    return nullptr;
}

void Service::addMesh(Mesh *p)
{
}

Material *Service::createMaterial()
{
    return nullptr;
}

void Service::bindMaterial(Material *p)
{
}

Volcano::Graphics::QuickSurface *Service::createQuickSurface(QQuickWindow *window)
{
    auto surface = std::make_unique<QuickSurface>();
    if (!surface && !surface->init(window))
        return nullptr;
    return surface.release();
}

void Service::addQuickSurface(Volcano::Graphics::QuickSurface *surface)
{
#ifdef VOLCANO_DEBUG
    auto p = qobject_cast<QuickSurface *>(surface);
#else
    auto p = static_cast<QuickSurface *>(surface);
#endif

    Q_ASSERT(p != nullptr);

    p->update();

    // TODO
}

void Service::beginFrame(void)
{
}

void Service::endFrame(void)
{
}

bool Service::onStart(void)
{
    qDebug("Creating OpenGL window...");
    m_window.create();
    m_window.show();

    return true;
}

void Service::onStop(void)
{
    m_window.destroy();
}

VOLCANO_GRAPHICS_OPENGL_END
