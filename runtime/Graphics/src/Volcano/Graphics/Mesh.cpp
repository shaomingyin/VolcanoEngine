//
//
#include <Volcano/Graphics/Mesh.h>

VOLCANO_GRAPHICS_BEGIN

Mesh::Mesh()
	: vao_(0)
	, vbo_(0)
	, ebo_(0) {
}

Mesh::Mesh(const std::string& path) {
}

Mesh::~Mesh() {
	if (vao_ > 0) {
		glDeleteVertexArrays(1, &vao_);
	}
	if (vbo_ > 0) {
		glDeleteBuffers(1, &vbo_);
	}
	if (ebo_ > 0) {
		glDeleteBuffers(1, &ebo_);
	}
}

bool Mesh::load(aiMesh* imp) {
	VOLCANO_ASSERT(imp != nullptr);
	VOLCANO_ASSERT(vao_ == 0);
	VOLCANO_ASSERT(vbo_ == 0);
	VOLCANO_ASSERT(ebo_ == 0);

	color_channels_ = imp->GetNumColorChannels();
	uv_channels_ = imp->GetNumUVChannels();
	has_normals_ = imp->HasNormals();

	// [XYZ][UV][ABC]
	auto vertex_size = (color_channels_ * sizeof(float)) + (uv_channels_ * sizeof(float));
	if (has_normals_) {
		vertex_size += 3 * sizeof(float);
	}

	auto vbo_size = imp->mNumVertices * vertex_size;

	glGenVertexArrays(1, &vao_);
	glGenBuffers(1, &vbo_);
	glGenBuffers(1, &ebo_);

	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);

	glBufferData(GL_ARRAY_BUFFER, vbo_size, nullptr, GL_STATIC_DRAW);
	void* vbo = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	for (unsigned int i = 0; i < imp->mNumVertices; ++i) {
	}

	if (has_normals_) {

	}

	return true;
}

VOLCANO_GRAPHICS_END
