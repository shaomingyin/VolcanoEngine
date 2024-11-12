//
//
#ifndef VOLCANO_GRAPHICS_MESH_H
#define VOLCANO_GRAPHICS_MESH_H

#include <string>

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/RenderableObject.h>

VOLCANO_GRAPHICS_BEGIN

class Mesh: public RenderableObject {
public:
	Mesh(const std::string& path);
	~Mesh() override;

public:
	void load();
	void render() override;

private:
	std::string path_;
	unsigned int color_channels_;
	unsigned int uv_channels_;
	bool has_normals_;
	GLuint vao_;
	GLuint vbo_;
	GLuint ebo_;
	std::future<void> loading_future_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_MESH_H
