//
//
#include <QScopeGuard>

#include <Volcano/Graphics/Renderer.hpp>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(QObject *parent):
    QObject(parent),
    m_surface(nullptr),
    m_gl(nullptr)
{
}

Renderer::~Renderer(void)
{
    // TODO
}

bool Renderer::init(QSurface *surface)
{
    Q_ASSERT(surface != nullptr);

    Q_ASSERT(m_surface == nullptr);
    Q_ASSERT(m_gl == nullptr);
    Q_ASSERT(QOpenGLContext::currentContext() != nullptr);

    m_gl = glFunctions();
    if (m_gl == nullptr) {
        return false;
    }

    m_gl->glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    m_surface = surface;

	return true;
}

void Renderer::shutdown(void)
{
    Q_ASSERT(m_surface != nullptr);
}

void Renderer::beginFrame(void)
{
}

void Renderer::endFrame(void)
{
    Q_ASSERT(m_surface != nullptr);
    Q_ASSERT(m_gl != nullptr);

    auto size = m_surface->size();
    m_gl->glViewport(0, 0, size.width(), size.height());
    m_gl->glClear(GL_COLOR_BUFFER_BIT);

    // m_program.use();
    // setup view matrix uniform.
}

Renderer::MatrixMode Renderer::matrixMode(void) const
{
    return m_matrixMode;
}

void Renderer::setMatrixMode(MatrixMode mode)
{
    m_matrixMode = mode;
}

void Renderer::loadIdentity(void)
{
    m_matrix[m_matrixMode].setToIdentity();
    m_translate = QVector3D(0.0f, 0.0f, 0.0f);
    m_scale = QVector3D(0.0f, 0.0f, 0.0f);
    m_rotation = QQuaternion(0.0f, 0.0f, 0.0f, 0.0f);
}

void Renderer::translate(const QVector3D &r)
{
    m_translate += r;
}

void Renderer::scale(const QVector3D &r)
{
    m_scale *= r;
}

void Renderer::rotation(float angle, const QVector3D &r)
{
    m_rotation *= QQuaternion::fromAxisAndAngle(r, angle);
}

void Renderer::bind(Material &material)
{
    m_material = &material;
}

void Renderer::add(Mesh &mesh)
{

}

void Renderer::add(DirectionalLight &directionalLight)
{

}

void Renderer::add(PointLight &pointLight)
{

}

void Renderer::add(SpotLight &spotLight)
{

}

VOLCANO_GRAPHICS_END
