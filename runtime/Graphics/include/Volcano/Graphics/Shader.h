//
//
#ifndef VOLCANO_GRAPHICS_SHADER_H
#define VOLCANO_GRAPHICS_SHADER_H

#include <string>

#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class Shader {
public:
	Shader(GLenum type);
	Shader(GLenum type, const GLchar* src);
	Shader(GLenum type, const GLchar* src, GLint src_length);
	virtual ~Shader();

public:
	void setSource(const GLchar* src, GLint src_length);
	void compile();

	GLenum type() const {
		return type_;
	}

	GLuint id() const {
		return id_;
	}

	void setSource(const GLchar* src) {
		setSource(src, strlen(src));
	}

public:
	GLenum type_;
	GLuint id_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_SHADER_H
