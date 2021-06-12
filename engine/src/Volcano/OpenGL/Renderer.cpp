//
//
#include <Volcano/OpenGL/Renderer.hpp>

VOLCANO_OPENGL_BEGIN

Renderer::Renderer(QObject *parent):
    QObject(parent),
    m_functions(nullptr),
    m_clearEnabled(true),
    m_viewport(0, 0, 0, 0)
{
}

Renderer::~Renderer(void)
{
}

bool Renderer::init(int width, int height)
{
    Q_ASSERT(m_functions == nullptr);

    m_functions = currentFunctions();
    if (m_functions == nullptr)
        return false;

    m_width = width;
    m_height = height;

    return true;
}

void Renderer::beginFrame(void)
{
}

void Renderer::endFrame(void)
{
    Q_ASSERT(m_functions != nullptr);

    m_functions->glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    m_functions->glClear(GL_COLOR_BUFFER_BIT);

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
#if 0
    m_matrix[m_matrixMode].setToIdentity();
    m_translate = QVector3D(0.0f, 0.0f, 0.0f);
    m_scale = QVector3D(0.0f, 0.0f, 0.0f);
    m_rotation = QQuaternion(0.0f, 0.0f, 0.0f, 0.0f);
#endif
}

void Renderer::translate(const QVector3D &r)
{
    //m_translate += r;
}

void Renderer::scale(const QVector3D &r)
{
    //m_scale *= r;
}

void Renderer::rotation(float angle, const QVector3D &axis)
{
    //m_rotation *= Eigen::Quaternionf(Eigen::AngleAxisf(angle, axis));
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

VOLCANO_OPENGL_END
