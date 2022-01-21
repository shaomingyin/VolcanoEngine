//
//
#include <memory>

#include <QThread>
#include <QOpenGLTexture>

#include <Volcano/Graphics/OpenGL/Window.hpp>
#include <Volcano/Graphics/OpenGL/Service.hpp>

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

QQuickRenderTarget *Service::createQuickRenderTarget(int width, int height, int sampleCount)
{
    //QOpenGLTexture texture(QOpenGLTexture::Target2D);
    //texture.
    //QQuickRenderTarget *renderTarget = QQuickRenderTarget::fromOpenGLTexture()
    return nullptr;
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
