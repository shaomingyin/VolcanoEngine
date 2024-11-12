//
//
#include <Volcano/Error.h>
#include <Volcano/Graphics/Shader.h>

VOLCANO_GRAPHICS_BEGIN

Shader::Shader(GLuint id)
	: id_(id) {
}

Shader::Shader(GLenum type, std::string_view src)
	: id_(glCreateShader(type)) {
	if (id_ == 0) {
		throw Error(Errc::OutOfResource);
	}
	if (!src.empty()) {
		const GLchar* data = src.data();
		const GLsizei size = src.size();
		glShaderSource(id_, 1, &data, &size);
	}
}

Shader::~Shader() {
	if (id_ > 0) {
		glDeleteShader(id_);
	}
}

void Shader::setSource(std::string_view src) {
	if (!src.empty()) {
		const GLchar* data = src.data();
		const GLsizei size = src.size();
		glShaderSource(id_, 1, &data, &size);
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
