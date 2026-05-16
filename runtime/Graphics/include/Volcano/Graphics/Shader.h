//
//
#ifndef VOLCANO_GRAPHICS_SHADER_H
#define VOLCANO_GRAPHICS_SHADER_H

#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class Shader {
public:
	enum class Type {
		Invalid = 0,
		Vertex,
		Fragment,
		Geometry,
		Compute,
	};

public:
	Shader(Type type);
	Shader(const Shader&) = delete;
	Shader(Shader&&) = default;
	virtual ~Shader() = default;

public:
	Shader& operator=(const Shader&) = delete;
	Shader& operator=(Shader&&) = default;

	Type type() const noexcept {
		return type_;
	}

	virtual void source(const char* source) = 0;
	virtual void compile() = 0;

private:
	Type type_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_SHADER_H
