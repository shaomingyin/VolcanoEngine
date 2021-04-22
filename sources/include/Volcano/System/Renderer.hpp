//
//
#ifndef VOLCANO_SYSTEM_RENDERER_HPP
#define VOLCANO_SYSTEM_RENDERER_HPP

#include <GL/gl3w.h>

#include <Volcano/VM/Renderer.hpp>
#include <Volcano/Graphics/Renderer.hpp>
#include <Volcano/System/Common.hpp>

VOLCANO_SYSTEM_BEGIN

class Renderer : public VM::Renderer {
public:
	Renderer(void);
	~Renderer(void) override;

public:
	bool init(int width, int height);
	void shutdown(void);
	void update(void);
	const Eigen::Vector4i &viewport(void) const override;
	void setViewport(const Eigen::Vector4i &v) override;
	bool isClearEnabled(void) const override;
	void setClearEnabled(bool v) override;
	const Eigen::Vector3f &clearColor(void) const override;
	void setClearColor(const Eigen::Vector3f &color) override;
	bool isDrawTriangles(void) const override;
	void setDrawTriangles(bool v) override;
	bool isDrawNormals(void) const override;
	void setDrawNormals(bool v) override;

private:
	Eigen::Vector4i m_viewport;
	bool m_isClearEnabled;
	Eigen::Vector3f m_clearColor;
	Graphics::Renderer m_graphicsRenderer;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_RENDERER_HPP
