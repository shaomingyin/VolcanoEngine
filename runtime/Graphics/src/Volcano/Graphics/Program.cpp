//
//
#include <Volcano/Error.h>
#include <Volcano/Graphics/Program.h>

VOLCANO_GRAPHICS_BEGIN

Program::Program()
	: id_(glCreateProgram()) {
	if (id_ == 0) {
		throw Error(Errc::InvalidParameter);
	}
}

Program::~Program() {
	glDeleteProgram(id_);
}

void Program::link() {
	GLint ret;
	glLinkProgram(id_);
	glGetProgramiv(id_, GL_LINK_STATUS, &ret);
	if (ret != GL_TRUE) {
		throw Error(Errc::InvalidParameter);
	}
}

VOLCANO_GRAPHICS_END
