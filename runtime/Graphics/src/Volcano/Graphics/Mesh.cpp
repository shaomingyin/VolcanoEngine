//
//
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Volcano/Error.h>
#include <Volcano/ScopeGuard.h>
#include <Volcano/Graphics/IOSystem.h>
#include <Volcano/Graphics/Mesh.h>

VOLCANO_GRAPHICS_BEGIN

Mesh::Mesh(const std::string& path)
	: path_(path)
	, vao_(0)
	, vbo_(0) 
	, ebo_(0) {
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

void Mesh::load() {
	VOLCANO_ASSERT(vao_ == 0);
	VOLCANO_ASSERT(vbo_ == 0);
	VOLCANO_ASSERT(ebo_ == 0);

#if 0
	PHYSFS_Stat st;
	auto ret = PHYSFS_stat(path_.c_str(), &st);
	if (!ret) {
		throw Error(Errc::NotExisted);
	}

	glGenVertexArrays(1, &vao_);
	if (vao_ == 0) {
		throw Error(Errc::OutOfResource);
	}

	glGenBuffers(1, &vbo_);
	if (vbo_ == 0) {
		throw Error(Errc::OutOfResource);
	}

	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);

	void* vbo_buf = glMapBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
	if (vbo_buf == nullptr) {
		throw Error(Errc::OutOfResource);
	}

	loading_future_ = defaultExecutor.async([fp, vbo_buf, size, fp_guard = std::move(fp_guard)]() mutable {
		Assimp::Importer importer;
		IOSystem io;
		importer.SetIOHandler(&io);
		const aiScene* scene = importer.ReadFile()
	});
#endif
}

VOLCANO_GRAPHICS_END
