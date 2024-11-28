//
//
#include <Volcano/ScopeGuard.h>
#include <Volcano/Graphics/Mesh.h>

VOLCANO_GRAPHICS_BEGIN

Mesh::Mesh(const std::string& path)
	: path_(path) {
}

Mesh::~Mesh() {
}

void Mesh::load() {
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

void Mesh::render() {
}

VOLCANO_GRAPHICS_END
