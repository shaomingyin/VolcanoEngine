//
//
#ifndef VOLCANO_GRAPHICS_CONTEXT_H
#define VOLCANO_GRAPHICS_CONTEXT_H

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Buffer.h>
#include <Volcano/Graphics/Shader.h>
#include <Volcano/Graphics/Program.h>
#include <Volcano/Graphics/Texture.h>

VOLCANO_GRAPHICS_BEGIN

class Context {
public:
	Context();
	virtual ~Context();

public:
	virtual Buffer* createBuffer(Buffer::Type type, uint32_t size) = 0;
	virtual Shader* createShader(Shader::Type type) = 0;
	virtual Program* createProgram() = 0;
	virtual Texture* createTexture(Texture::Type type, Texture::Format format, Texture::Usage usage, uint32_t width, uint32_t height, uint32_t depth = 0) = 0;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_CONTEXT_H
