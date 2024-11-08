//
//
#ifndef VOLCANO_GRAPHICS_PROGRAM_H
#define VOLCANO_GRAPHICS_PROGRAM_H

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Shader.h>

VOLCANO_GRAPHICS_BEGIN

class Program {
public:
	Program();
	virtual ~Program();

public:
	void link();

	void attachShader(GLuint id) {
		glAttachShader(id_, id);
	}

	void attachShader(Shader& shader) {
		attachShader(shader.id());
	}

	GLuint id() const {
		return id_;
	}

private:
	GLuint id_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_PROGRAM_H
