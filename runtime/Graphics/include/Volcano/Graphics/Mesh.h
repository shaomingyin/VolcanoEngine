//
//
#ifndef VOLCANO_GRAPHICS_MESH_H
#define VOLCANO_GRAPHICS_MESH_H

#include <assimp/mesh.h>

#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class Mesh {
public:
	Mesh();
	virtual ~Mesh();

public:
	bool load(aiMesh* imp);

private:
	unsigned int color_channels_;
	unsigned int uv_channels_;
	bool has_normals_;
	GLuint vao_;
	GLuint vbo_;
	GLuint ebo_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_MESH_H
