//
//
#ifndef VOLCANO_GRAPHICS_PROGRAM_H
#define VOLCANO_GRAPHICS_PROGRAM_H

#include <string>
#include <initializer_list>

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Shader.h>

VOLCANO_GRAPHICS_BEGIN

class Program {
public:
	Program();
	Program(GLuint id);
	Program(const Program&) = delete;
	Program(std::initializer_list<std::pair<GLenum, std::string>> shader_infos);
	virtual ~Program();

	Program(Program&& other)
		: id_(other.id_) {
		other.id_ = 0;
	}

public:
	void link();

	void attachShader(GLuint id) noexcept {
		glAttachShader(id_, id);
	}

	void attachShader(const Shader& shader) noexcept {
		attachShader(shader.id());
	}

	void attachShader(GLenum type, std::string_view src) {
		Shader shader(type, src);
		shader.compile();
		attachShader(shader);
	}

	GLuint id() const noexcept {
		return id_;
	}

	void use() const noexcept {
		VOLCANO_ASSERT(id_ > 0);
		glUseProgram(id_);
	}

	Program& operator=(const Program&) noexcept = delete;

	Program& operator=(Program&& other) noexcept {
		id_ = other.id_;
		other.id_ = 0;
		return (*this);
	}

private:
	GLuint id_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_PROGRAM_H
