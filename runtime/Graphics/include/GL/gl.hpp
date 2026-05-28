//
//
#ifndef GL_HPP
#define GL_HPP

#include <GL/gl3w.h>

#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace gl {

namespace detail {

inline std::string getShaderInfoLog(GLuint shader) {
	GLint length = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
	if (length <= 1) {
		return {};
	}

	std::string log(static_cast<size_t>(length), '\0');
	GLsizei written = 0;
	glGetShaderInfoLog(shader, length, &written, log.data());
	log.resize(static_cast<size_t>(written));
	return log;
}

inline std::string getProgramInfoLog(GLuint program) {
	GLint length = 0;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
	if (length <= 1) {
		return {};
	}

	std::string log(static_cast<size_t>(length), '\0');
	GLsizei written = 0;
	glGetProgramInfoLog(program, length, &written, log.data());
	log.resize(static_cast<size_t>(written));
	return log;
}

inline const char* framebufferStatusName(GLenum status) noexcept {
	switch (status) {
	case GL_FRAMEBUFFER_COMPLETE:
		return "GL_FRAMEBUFFER_COMPLETE";
	case GL_FRAMEBUFFER_UNDEFINED:
		return "GL_FRAMEBUFFER_UNDEFINED";
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
	case GL_FRAMEBUFFER_UNSUPPORTED:
		return "GL_FRAMEBUFFER_UNSUPPORTED";
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		return "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		return "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
	default:
		return "GL_FRAMEBUFFER_UNKNOWN_STATUS";
	}
}

} // namespace detail

class Buffer {
public:
	explicit Buffer(GLenum target = GL_ARRAY_BUFFER)
		: target_(target) {
		glGenBuffers(1, &id_);
	}

	Buffer(const Buffer&) = delete;

	Buffer(Buffer&& other) noexcept
		: id_(std::exchange(other.id_, 0))
		, target_(other.target_) {
	}

	~Buffer() {
		reset();
	}

	Buffer& operator=(const Buffer&) = delete;

	Buffer& operator=(Buffer&& other) noexcept {
		if (this != &other) {
			reset();
			id_ = std::exchange(other.id_, 0);
			target_ = other.target_;
		}
		return *this;
	}

	GLuint id() const noexcept {
		return id_;
	}

	GLenum target() const noexcept {
		return target_;
	}

	explicit operator bool() const noexcept {
		return id_ != 0;
	}

	GLuint release() noexcept {
		return std::exchange(id_, 0);
	}

	void reset(GLuint id = 0) noexcept {
		if (id_ != 0) {
			glDeleteBuffers(1, &id_);
		}
		id_ = id;
	}

	void bind() const noexcept {
		glBindBuffer(target_, id_);
	}

	void unbind() const noexcept {
		glBindBuffer(target_, 0);
	}

	void data(GLsizeiptr size, const void* data, GLenum usage = GL_STATIC_DRAW) const {
		bind();
		glBufferData(target_, size, data, usage);
	}

	void subData(GLintptr offset, GLsizeiptr size, const void* data) const {
		bind();
		glBufferSubData(target_, offset, size, data);
	}

	void* map(GLenum access = GL_WRITE_ONLY) const {
		bind();
		return glMapBuffer(target_, access);
	}

	GLboolean unmap() const {
		bind();
		return glUnmapBuffer(target_);
	}

	void bindBase(GLuint index) const noexcept {
		glBindBufferBase(target_, index, id_);
	}

	void bindBase(GLenum target, GLuint index) const noexcept {
		glBindBufferBase(target, index, id_);
	}

private:
	GLuint id_ = 0;
	GLenum target_ = GL_ARRAY_BUFFER;
};

class VertexBuffer : public Buffer {
public:
	VertexBuffer()
		: Buffer(GL_ARRAY_BUFFER) {
	}

	VertexBuffer(GLsizeiptr size, const void* data, GLenum usage = GL_STATIC_DRAW)
		: VertexBuffer() {
		this->data(size, data, usage);
	}
};

class IndexBuffer : public Buffer {
public:
	IndexBuffer()
		: Buffer(GL_ELEMENT_ARRAY_BUFFER) {
	}

	IndexBuffer(GLsizeiptr size, const void* data, GLenum usage = GL_STATIC_DRAW)
		: IndexBuffer() {
		this->data(size, data, usage);
	}
};

class UniformBuffer : public Buffer {
public:
	UniformBuffer()
		: Buffer(GL_UNIFORM_BUFFER) {
	}

	UniformBuffer(GLsizeiptr size, const void* data, GLenum usage = GL_STATIC_DRAW)
		: UniformBuffer() {
		this->data(size, data, usage);
	}
};

class VertexArray {
public:
	VertexArray() {
		glGenVertexArrays(1, &id_);
	}

	VertexArray(const VertexArray&) = delete;

	VertexArray(VertexArray&& other) noexcept
		: id_(std::exchange(other.id_, 0)) {
	}

	~VertexArray() {
		reset();
	}

	VertexArray& operator=(const VertexArray&) = delete;

	VertexArray& operator=(VertexArray&& other) noexcept {
		if (this != &other) {
			reset();
			id_ = std::exchange(other.id_, 0);
		}
		return *this;
	}

	GLuint id() const noexcept {
		return id_;
	}

	explicit operator bool() const noexcept {
		return id_ != 0;
	}

	GLuint release() noexcept {
		return std::exchange(id_, 0);
	}

	void reset(GLuint id = 0) noexcept {
		if (id_ != 0) {
			glDeleteVertexArrays(1, &id_);
		}
		id_ = id;
	}

	void bind() const noexcept {
		glBindVertexArray(id_);
	}

	static void unbind() noexcept {
		glBindVertexArray(0);
	}

	void enable(GLuint index) const noexcept {
		bind();
		glEnableVertexAttribArray(index);
	}

	void disable(GLuint index) const noexcept {
		bind();
		glDisableVertexAttribArray(index);
	}

	void attribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) const noexcept {
		bind();
		glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	}

	void attribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, size_t offset) const noexcept {
		attribPointer(index, size, type, normalized, stride, reinterpret_cast<const void*>(static_cast<uintptr_t>(offset)));
	}

	void attribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer) const noexcept {
		bind();
		glVertexAttribIPointer(index, size, type, stride, pointer);
	}

	void attribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, size_t offset) const noexcept {
		attribIPointer(index, size, type, stride, reinterpret_cast<const void*>(static_cast<uintptr_t>(offset)));
	}

	void vertexAttribPointer(const Buffer& buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, size_t offset = 0) const noexcept {
		bind();
		buffer.bind();
		enable(index);
		glVertexAttribPointer(index, size, type, normalized, stride, reinterpret_cast<const void*>(static_cast<uintptr_t>(offset)));
	}

	void vertexAttribIPointer(const Buffer& buffer, GLuint index, GLint size, GLenum type, GLsizei stride, size_t offset = 0) const noexcept {
		bind();
		buffer.bind();
		enable(index);
		glVertexAttribIPointer(index, size, type, stride, reinterpret_cast<const void*>(static_cast<uintptr_t>(offset)));
	}

private:
	GLuint id_ = 0;
};

class Texture {
public:
	explicit Texture(GLenum target = GL_TEXTURE_2D)
		: target_(target) {
		glGenTextures(1, &id_);
	}

	Texture(const Texture&) = delete;

	Texture(Texture&& other) noexcept
		: id_(std::exchange(other.id_, 0))
		, target_(other.target_) {
	}

	~Texture() {
		reset();
	}

	Texture& operator=(const Texture&) = delete;

	Texture& operator=(Texture&& other) noexcept {
		if (this != &other) {
			reset();
			id_ = std::exchange(other.id_, 0);
			target_ = other.target_;
		}
		return *this;
	}

	GLuint id() const noexcept {
		return id_;
	}

	GLenum target() const noexcept {
		return target_;
	}

	explicit operator bool() const noexcept {
		return id_ != 0;
	}

	GLuint release() noexcept {
		return std::exchange(id_, 0);
	}

	void reset(GLuint id = 0) noexcept {
		if (id_ != 0) {
			glDeleteTextures(1, &id_);
		}
		id_ = id;
	}

	void bind() const noexcept {
		glBindTexture(target_, id_);
	}

	void bind(GLuint unit) const noexcept {
		glActiveTexture(GL_TEXTURE0 + unit);
		bind();
	}

	void unbind() const noexcept {
		glBindTexture(target_, 0);
	}

	void parameter(GLenum name, GLint value) const noexcept {
		bind();
		glTexParameteri(target_, name, value);
	}

	void parameter(GLenum name, GLfloat value) const noexcept {
		bind();
		glTexParameterf(target_, name, value);
	}

	void image1D(GLint level, GLint internalFormat, GLsizei width, GLint border, GLenum format, GLenum type, const void* data) const noexcept {
		bind();
		glTexImage1D(target_, level, internalFormat, width, border, format, type, data);
	}

	void image2D(GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* data) const noexcept {
		bind();
		glTexImage2D(target_, level, internalFormat, width, height, border, format, type, data);
	}

	void image3D(GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* data) const noexcept {
		bind();
		glTexImage3D(target_, level, internalFormat, width, height, depth, border, format, type, data);
	}

	void subImage1D(GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void* data) const noexcept {
		bind();
		glTexSubImage1D(target_, level, xoffset, width, format, type, data);
	}

	void subImage2D(GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* data) const noexcept {
		bind();
		glTexSubImage2D(target_, level, xoffset, yoffset, width, height, format, type, data);
	}

	void subImage3D(GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* data) const noexcept {
		bind();
		glTexSubImage3D(target_, level, xoffset, yoffset, zoffset, width, height, depth, format, type, data);
	}

	void generateMipMap() const noexcept {
		bind();
		glGenerateMipmap(target_);
	}

private:
	GLuint id_ = 0;
	GLenum target_ = GL_TEXTURE_2D;
};

class Texture1D : public Texture {
public:
	Texture1D()
		: Texture(GL_TEXTURE_1D) {
	}
};

class Texture2D : public Texture {
public:
	Texture2D()
		: Texture(GL_TEXTURE_2D) {
	}
};

class Texture3D : public Texture {
public:
	Texture3D()
		: Texture(GL_TEXTURE_3D) {
	}
};

class Shader {
public:
	explicit Shader(GLenum type)
		: id_(glCreateShader(type))
		, type_(type) {
	}

	Shader(const Shader&) = delete;

	Shader(Shader&& other) noexcept
		: id_(std::exchange(other.id_, 0))
		, type_(other.type_) {
	}

	~Shader() {
		reset();
	}

	Shader& operator=(const Shader&) = delete;

	Shader& operator=(Shader&& other) noexcept {
		if (this != &other) {
			reset();
			id_ = std::exchange(other.id_, 0);
			type_ = other.type_;
		}
		return *this;
	}

	static Shader fromSource(GLenum type, std::string_view source) {
		Shader shader(type);
		shader.source(source);
		shader.compile();
		return shader;
	}

	GLuint id() const noexcept {
		return id_;
	}

	GLenum type() const noexcept {
		return type_;
	}

	explicit operator bool() const noexcept {
		return id_ != 0;
	}

	GLuint release() noexcept {
		return std::exchange(id_, 0);
	}

	void reset(GLuint id = 0) noexcept {
		if (id_ != 0) {
			glDeleteShader(id_);
		}
		id_ = id;
	}

	void source(std::string_view source) const {
		const GLchar* data = source.data();
		const GLint length = static_cast<GLint>(source.size());
		glShaderSource(id_, 1, &data, &length);
	}

	void compile() const {
		glCompileShader(id_);

		GLint compiled = GL_FALSE;
		glGetShaderiv(id_, GL_COMPILE_STATUS, &compiled);
		if (compiled != GL_TRUE) {
			throw std::runtime_error("OpenGL shader compilation failed: " + infoLog());
		}
	}

	std::string infoLog() const {
		return detail::getShaderInfoLog(id_);
	}

private:
	GLuint id_ = 0;
	GLenum type_ = GL_NONE;
};

class Program {
public:
	Program()
		: id_(glCreateProgram()) {
	}

	Program(const Program&) = delete;

	Program(Program&& other) noexcept
		: id_(std::exchange(other.id_, 0)) {
	}

	~Program() {
		reset();
	}

	Program& operator=(const Program&) = delete;

	Program& operator=(Program&& other) noexcept {
		if (this != &other) {
			reset();
			id_ = std::exchange(other.id_, 0);
		}
		return *this;
	}

	GLuint id() const noexcept {
		return id_;
	}

	explicit operator bool() const noexcept {
		return id_ != 0;
	}

	GLuint release() noexcept {
		return std::exchange(id_, 0);
	}

	void reset(GLuint id = 0) noexcept {
		if (id_ != 0) {
			glDeleteProgram(id_);
		}
		id_ = id;
	}

	void attach(const Shader& shader) const noexcept {
		assert(shader);
		glAttachShader(id_, shader.id());
	}

	void detach(const Shader& shader) const noexcept {
		assert(shader);
		glDetachShader(id_, shader.id());
	}

	void attachSource(GLenum type, std::string_view source) const {
		Shader shader = Shader::fromSource(type, source);
		attach(shader);
	}

	void link() const {
		glLinkProgram(id_);

		GLint linked = GL_FALSE;
		glGetProgramiv(id_, GL_LINK_STATUS, &linked);
		if (linked != GL_TRUE) {
			throw std::runtime_error("OpenGL program link failed: " + infoLog());
		}
	}

	void use() const noexcept {
		glUseProgram(id_);
	}

	static void unuse() noexcept {
		glUseProgram(0);
	}

	GLint uniformLocation(const char* name) const noexcept {
		return glGetUniformLocation(id_, name);
	}

	void uniform(const char* name, GLint value) const noexcept {
		glUniform1i(uniformLocation(name), value);
	}

	void uniform(const char* name, GLuint value) const noexcept {
		glUniform1ui(uniformLocation(name), value);
	}

	void uniform(const char* name, GLfloat value) const noexcept {
		glUniform1f(uniformLocation(name), value);
	}

	void uniform2f(const char* name, GLfloat x, GLfloat y) const noexcept {
		glUniform2f(uniformLocation(name), x, y);
	}

	void uniform3f(const char* name, GLfloat x, GLfloat y, GLfloat z) const noexcept {
		glUniform3f(uniformLocation(name), x, y, z);
	}

	void uniform4f(const char* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const noexcept {
		glUniform4f(uniformLocation(name), x, y, z, w);
	}

	void uniformMatrix3fv(const char* name, const GLfloat* value, GLboolean transpose = GL_FALSE, GLsizei count = 1) const noexcept {
		glUniformMatrix3fv(uniformLocation(name), count, transpose, value);
	}

	void uniformMatrix4fv(const char* name, const GLfloat* value, GLboolean transpose = GL_FALSE, GLsizei count = 1) const noexcept {
		glUniformMatrix4fv(uniformLocation(name), count, transpose, value);
	}

	std::string infoLog() const {
		return detail::getProgramInfoLog(id_);
	}

private:
	GLuint id_ = 0;
};

class Renderbuffer {
public:
	Renderbuffer() {
		glGenRenderbuffers(1, &id_);
	}

	Renderbuffer(const Renderbuffer&) = delete;

	Renderbuffer(Renderbuffer&& other) noexcept
		: id_(std::exchange(other.id_, 0)) {
	}

	~Renderbuffer() {
		reset();
	}

	Renderbuffer& operator=(const Renderbuffer&) = delete;

	Renderbuffer& operator=(Renderbuffer&& other) noexcept {
		if (this != &other) {
			reset();
			id_ = std::exchange(other.id_, 0);
		}
		return *this;
	}

	GLuint id() const noexcept {
		return id_;
	}

	explicit operator bool() const noexcept {
		return id_ != 0;
	}

	GLuint release() noexcept {
		return std::exchange(id_, 0);
	}

	void reset(GLuint id = 0) noexcept {
		if (id_ != 0) {
			glDeleteRenderbuffers(1, &id_);
		}
		id_ = id;
	}

	void bind() const noexcept {
		glBindRenderbuffer(GL_RENDERBUFFER, id_);
	}

	static void unbind() noexcept {
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	void storage(GLenum internalFormat, GLsizei width, GLsizei height) const noexcept {
		bind();
		glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
	}

	void storageMultisample(GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height) const noexcept {
		bind();
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalFormat, width, height);
	}

private:
	GLuint id_ = 0;
};

class Framebuffer {
public:
	explicit Framebuffer(GLenum target = GL_FRAMEBUFFER)
		: target_(target) {
		glGenFramebuffers(1, &id_);
	}

	Framebuffer(const Framebuffer&) = delete;

	Framebuffer(Framebuffer&& other) noexcept
		: id_(std::exchange(other.id_, 0))
		, target_(other.target_) {
	}

	~Framebuffer() {
		reset();
	}

	Framebuffer& operator=(const Framebuffer&) = delete;

	Framebuffer& operator=(Framebuffer&& other) noexcept {
		if (this != &other) {
			reset();
			id_ = std::exchange(other.id_, 0);
			target_ = other.target_;
		}
		return *this;
	}

	GLuint id() const noexcept {
		return id_;
	}

	GLenum target() const noexcept {
		return target_;
	}

	explicit operator bool() const noexcept {
		return id_ != 0;
	}

	GLuint release() noexcept {
		return std::exchange(id_, 0);
	}

	void reset(GLuint id = 0) noexcept {
		if (id_ != 0) {
			glDeleteFramebuffers(1, &id_);
		}
		id_ = id;
	}

	void bind() const noexcept {
		glBindFramebuffer(target_, id_);
	}

	void unbind() const noexcept {
		glBindFramebuffer(target_, 0);
	}

	void attachTexture1D(GLenum attachment, const Texture& texture, GLint level = 0) const noexcept {
		bind();
		glFramebufferTexture1D(target_, attachment, texture.target(), texture.id(), level);
	}

	void attachTexture2D(GLenum attachment, const Texture& texture, GLint level = 0) const noexcept {
		bind();
		glFramebufferTexture2D(target_, attachment, texture.target(), texture.id(), level);
	}

	void attachTexture3D(GLenum attachment, const Texture& texture, GLint level, GLint layer) const noexcept {
		bind();
		glFramebufferTexture3D(target_, attachment, texture.target(), texture.id(), level, layer);
	}

	void attachRenderbuffer(GLenum attachment, const Renderbuffer& renderbuffer) const noexcept {
		bind();
		glFramebufferRenderbuffer(target_, attachment, GL_RENDERBUFFER, renderbuffer.id());
	}

	void drawBuffers(const GLenum* buffers, GLsizei count) const noexcept {
		bind();
		glDrawBuffers(count, buffers);
	}

	void drawBuffers(const std::vector<GLenum>& buffers) const noexcept {
		drawBuffers(buffers.data(), static_cast<GLsizei>(buffers.size()));
	}

	GLenum status() const noexcept {
		bind();
		return glCheckFramebufferStatus(target_);
	}

	bool complete() const noexcept {
		return status() == GL_FRAMEBUFFER_COMPLETE;
	}

	void checkComplete() const {
		const GLenum currentStatus = status();
		if (currentStatus != GL_FRAMEBUFFER_COMPLETE) {
			throw std::runtime_error(std::string("OpenGL framebuffer incomplete: ") + detail::framebufferStatusName(currentStatus));
		}
	}

private:
	GLuint id_ = 0;
	GLenum target_ = GL_FRAMEBUFFER;
};

class Sampler {
public:
	Sampler() {
		glGenSamplers(1, &id_);
	}

	Sampler(const Sampler&) = delete;

	Sampler(Sampler&& other) noexcept
		: id_(std::exchange(other.id_, 0)) {
	}

	~Sampler() {
		reset();
	}

	Sampler& operator=(const Sampler&) = delete;

	Sampler& operator=(Sampler&& other) noexcept {
		if (this != &other) {
			reset();
			id_ = std::exchange(other.id_, 0);
		}
		return *this;
	}

	GLuint id() const noexcept {
		return id_;
	}

	explicit operator bool() const noexcept {
		return id_ != 0;
	}

	GLuint release() noexcept {
		return std::exchange(id_, 0);
	}

	void reset(GLuint id = 0) noexcept {
		if (id_ != 0) {
			glDeleteSamplers(1, &id_);
		}
		id_ = id;
	}

	void bind(GLuint unit) const noexcept {
		glBindSampler(unit, id_);
	}

	static void unbind(GLuint unit) noexcept {
		glBindSampler(unit, 0);
	}

	void parameter(GLenum name, GLint value) const noexcept {
		glSamplerParameteri(id_, name, value);
	}

	void parameter(GLenum name, GLfloat value) const noexcept {
		glSamplerParameterf(id_, name, value);
	}

private:
	GLuint id_ = 0;
};

} // namespace gl

#endif // GL_HPP
