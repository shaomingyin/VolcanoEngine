//
//
#ifndef VOLCANO_GRAPHICS_PROGRAM_H
#define VOLCANO_GRAPHICS_PROGRAM_H

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Shader.h>

VOLCANO_GRAPHICS_BEGIN

class Program {
public:
	Program() = default;
	virtual ~Program() = default;

public:
	virtual void attach(const char* source, Shader::Type type) = 0;
	virtual void attach(Shader&& shader) = 0;
	virtual void link() = 0;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_PROGRAM_H
