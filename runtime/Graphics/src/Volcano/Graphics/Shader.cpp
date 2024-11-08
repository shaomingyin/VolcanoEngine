//
//
#include <Volcano/Error.h>
#include <Volcano/Graphics/Shader.h>

VOLCANO_GRAPHICS_BEGIN

Shader::Shader(GLenum type)
	: Shader(type, nullptr, 0) {
}

Shader::Shader(GLenum type, const GLchar* src)
	: Shader(type, src, strlen(src)) {
}

Shader::Shader(GLenum type, const GLchar* src, GLint src_length)
	: type_(type)
	, id_(glCreateShader(type)) {
	if (id_ == 0) {
		throw Error(Errc::OutOfResource);
	}
	setSource(src, src_length);
}

Shader::~Shader() {
	glDeleteShader(id_);
}

void Shader::setSource(const GLchar* src, GLint src_length) {
	if (src != nullptr && src_length > 0) {
		glShaderSource(id_, 1, &src, &src_length);
	}
}

void Shader::compile() {
	GLint ret;
	glCompileShader(id_);
	glGetShaderiv(id_, GL_COMPILE_STATUS, &ret);
	if (ret != GL_TRUE) {
		throw Error(Errc::InvalidParameter);
	}
}

VOLCANO_GRAPHICS_END
