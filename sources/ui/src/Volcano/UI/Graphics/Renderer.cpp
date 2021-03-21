//
//
#include <Volcano/ScopeGuard.hpp>
#include <Volcano/UI/Graphics/Renderer.hpp>

VOLCANO_UI_GRAPHICS_BEGIN

Renderer::Renderer(void)
{
}

Renderer::~Renderer(void)
{
    // TODO
}

bool Renderer::init(int width, int height)
{
    glClearColor(0.3f, 0.3f, 0.3f, 0.0f);

    m_width = width;
    m_height = height;

	return true;
}

void Renderer::shutdown(void)
{
}

void Renderer::beginFrame(void)
{
}

void Renderer::endFrame(void)
{
    glViewport(0, 0, m_width, m_height);
    glClear(GL_COLOR_BUFFER_BIT);

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

void Renderer::translate(const Eigen::Vector3f &r)
{
    //m_translate += r;
}

void Renderer::scale(const Eigen::Vector3f &r)
{
    //m_scale *= r;
}

void Renderer::rotation(float angle, const Eigen::Vector3f &axis)
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

VOLCANO_UI_GRAPHICS_END
