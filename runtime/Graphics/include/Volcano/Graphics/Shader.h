//
//
#ifndef VOLCANO_GRAPHICS_SHADER_H
#define VOLCANO_GRAPHICS_SHADER_H

#include <string_view>

#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class Shader {
public:
	Shader(GLuint id = 0);
	Shader(GLenum type, std::string_view src);
	Shader(const Shader&) = delete;
	virtual ~Shader();

	Shader(Shader&& other)
		: id_(other.id_) {
		other.id_ = 0;
	}

public:
	void setSource(std::string_view src);
	void compile();

	GLenum type() const {
		GLint type;
		glGetShaderiv(id_, GL_SHADER_TYPE, &type);
		return type;
	}

	GLuint id() const {
		return id_;
	}

	Shader& operator=(const Shader&) = delete;

	Shader& operator=(Shader&& other) {
		id_ = other.id_;
		other.id_ = 0;
	}

public:
	GLuint id_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_SHADER_H
