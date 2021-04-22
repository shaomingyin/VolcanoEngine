//
//
#ifndef VOLCANO_VM_RENDERER_HPP
#define VOLCANO_VM_RENDERER_HPP

#include <Volcano/VM/Common.hpp>

VOLCANO_VM_BEGIN

class Renderer {
public:
	Renderer(void) = default;
	virtual ~Renderer(void) = default;

public:
	virtual const Eigen::Vector4i &viewport(void) const = 0;
	virtual void setViewport(const Eigen::Vector4i &v) = 0;
	virtual bool isClearEnabled(void) const = 0;
	virtual void setClearEnabled(bool v) = 0;
	virtual const Eigen::Vector3f &clearColor(void) const = 0;
	virtual void setClearColor(const Eigen::Vector3f &color) = 0;
	virtual bool isDrawTriangles(void) const = 0;
	virtual void setDrawTriangles(bool v) = 0;
	virtual bool isDrawNormals(void) const = 0;
	virtual void setDrawNormals(bool v) = 0;
};

VOLCANO_VM_END

#endif // VOLCANO_VM_RENDERER_HPP
