//
//
#include <Volcano/System/Renderer.hpp>

VOLCANO_SYSTEM_BEGIN

Renderer::Renderer(void)
{
}

Renderer::~Renderer(void)
{
}

bool Renderer::init(int width, int height)
{
	return true;
}

void Renderer::shutdown(void)
{

}

void Renderer::update(void)
{
	glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);

	if (m_isClearEnabled) {
		glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// TODO
}

const Eigen::Vector4i &Renderer::viewport(void) const
{
	return m_viewport;
}

void Renderer::setViewport(const Eigen::Vector4i &v)
{
	m_viewport = v;
}

bool Renderer::isClearEnabled(void) const
{
	return m_isClearEnabled;
}

void Renderer::setClearEnabled(bool v)
{
	m_isClearEnabled = v;
}

const Eigen::Vector3f &Renderer::clearColor(void) const
{
	return m_clearColor;
}

void Renderer::setClearColor(const Eigen::Vector3f &color)
{
	m_clearColor = color;
}

bool Renderer::isDrawTriangles(void) const
{
	return false;
}

void Renderer::setDrawTriangles(bool v)
{
}

bool Renderer::isDrawNormals(void) const
{
	return false;
}

void Renderer::setDrawNormals(bool v)
{
}

VOLCANO_SYSTEM_END
