//
//
#ifndef VOLCANO_GRAPHICS_MESH_H
#define VOLCANO_GRAPHICS_MESH_H

#include <string>

#include <assimp/mesh.h>

#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class Mesh {
public:
	Mesh(const std::string& path);
	virtual ~Mesh();

public:
	void load();

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
