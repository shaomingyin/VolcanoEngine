#include <algorithm>
#include <cstring>
#include <filesystem>
#include <limits>
#include <map>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <GL/gl3w.h>
#include <google/protobuf/io/zero_copy_stream.h>

#include <Volcano/ByteArray.h>
#include <Volcano/Cache.h>
#include <Volcano/PhysFile.h>
#include <Volcano/Asset/Common.h>
#include <Volcano/Asset/Mesh.h>
#include <Volcano/Asset/Texture.h>
#include <Volcano/Graphics/Font.h>
#include <Volcano/Graphics/Mesh.h>
#include <Volcano/Graphics/Texture.h>
#include <Volcano/Framework/Common.h>
#include <Volcano/Framework/Resources.h>

VOLCANO_FRAMEWORK_BEGIN
namespace Resources {
namespace {

template <typename T> using ResourceCache = Cache<T, std::filesystem::path>;

struct ResourceHelper final {
    ResourceHelper() = delete;

    template <typename T> static std::unique_ptr<WeakCachePolicy<T, std::filesystem::path>> weakPolicy() {
        return std::make_unique<WeakCachePolicy<T, std::filesystem::path>>();
    }

    static std::filesystem::path normalizePath(const std::filesystem::path& path) {
        const auto text = path.generic_string();
        if (text.empty() || text.find('\0') != std::string::npos || text.find('\\') != std::string::npos ||
            text.find(':') != std::string::npos) {
            throw std::invalid_argument("Invalid PhysFS resource path: " + text);
        }
        for (const auto& part : path) {
            if (part == "..") {
                throw std::invalid_argument("PhysFS resource path cannot contain '..': " + text);
            }
        }
        const auto result = (std::filesystem::path("/") / path.relative_path()).lexically_normal();
        if (result == "/" || result.filename().empty()) {
            throw std::invalid_argument("PhysFS resource path must name a file: " + text);
        }
        return result;
    }

    static std::string resolveRelative(const std::filesystem::path& baseDir, std::string_view relative) {
        if (relative.empty()) {
            return {};
        }
        return normalizePath(baseDir / relative).relative_path().generic_string();
    }

    static void readExactBytes(google::protobuf::io::ZeroCopyInputStream& stream, void* dest, int count) {
        auto* out = static_cast<uint8_t*>(dest);
        while (count > 0) {
            const void* chunk = nullptr;
            int chunkSize = 0;
            if (!stream.Next(&chunk, &chunkSize)) {
                throw std::runtime_error("Unexpected end of stream");
            }
            const int toCopy = std::min(count, chunkSize);
            std::memcpy(out, chunk, toCopy);
            out += toCopy;
            count -= toCopy;
            if (toCopy < chunkSize) {
                stream.BackUp(chunkSize - toCopy);
            }
        }
    }

    template <typename T, typename Loader>
    static std::shared_ptr<T> loadGraphics(std::map<GL3WProcs*, std::shared_ptr<ResourceCache<T>>>& contexts,
                                           const std::filesystem::path& path, Loader&& loader) {
        VOLCANO_ASSERT(gl3wProcs != nullptr);
        std::shared_ptr<ResourceCache<T>> cache;
        {
            const std::lock_guard lock(contextMutex);
            auto& current = contexts[gl3wProcs];
            if (!current) {
                current = std::make_shared<ResourceCache<T>>(weakPolicy<T>());
            }
            // Keep the partition alive if clear() concurrently removes it from the index.
            cache = current;
        }
        return cache->load(path, std::forward<Loader>(loader));
    }

public:
    static inline ResourceCache<Asset::Scene> sceneCache{weakPolicy<Asset::Scene>()};
    static inline ResourceCache<Asset::Model> modelCache{weakPolicy<Asset::Model>()};
    static inline ResourceCache<Asset::Material> materialCache{weakPolicy<Asset::Material>()};
    static inline ResourceCache<Graphics::Font> fontCache{weakPolicy<Graphics::Font>()};

    static inline std::mutex contextMutex;
    static inline std::map<GL3WProcs*, std::shared_ptr<ResourceCache<Graphics::Mesh>>> meshCache;
    static inline std::map<GL3WProcs*, std::shared_ptr<ResourceCache<Graphics::Texture>>> textureCache;
};

} // namespace

std::shared_ptr<Asset::Scene> loadScene(std::string_view filename) {
    const auto path = ResourceHelper::normalizePath(std::filesystem::path(filename));
    return ResourceHelper::sceneCache.load(path, [&path] {
        PhysInputStream stream(path.relative_path());
        auto scene = std::make_shared<Asset::Scene>();
        if (!scene->ParseFromZeroCopyStream(&stream)) {
            throw std::runtime_error("Cannot load scene '" + path.generic_string() + "'");
        }
        const auto baseDir = path.parent_path();
        for (auto& entity : *scene->mutable_entities()) {
            if (entity.has_model()) {
                auto* model = entity.mutable_model();
                if (!model->source().empty()) {
                    model->set_source(ResourceHelper::resolveRelative(baseDir, model->source()));
                }
                for (auto& material : *model->mutable_materials()) {
                    if (!material.albedo_texture().empty()) {
                        material.set_albedo_texture(
                            ResourceHelper::resolveRelative(baseDir, material.albedo_texture()));
                    }
                }
            }
            if (entity.has_perspective_camera() && !entity.perspective_camera().hud_texture().empty()) {
                entity.mutable_perspective_camera()->set_hud_texture(
                    ResourceHelper::resolveRelative(baseDir, entity.perspective_camera().hud_texture()));
            }
            if (entity.has_orthographic_camera() && !entity.orthographic_camera().hud_texture().empty()) {
                entity.mutable_orthographic_camera()->set_hud_texture(
                    ResourceHelper::resolveRelative(baseDir, entity.orthographic_camera().hud_texture()));
            }
        }
        return scene;
    });
}

std::shared_ptr<Asset::Model> loadModel(std::string_view filename) {
    const auto path = ResourceHelper::normalizePath(std::filesystem::path(filename));
    return ResourceHelper::modelCache.load(path, [&path] {
        PhysInputStream stream(path.relative_path());
        auto model = std::make_shared<Asset::Model>();
        if (!model->ParseFromZeroCopyStream(&stream)) {
            throw std::runtime_error("Cannot load model '" + path.generic_string() + "'");
        }
        for (auto& part : *model->mutable_parts()) {
            part.set_mesh(ResourceHelper::normalizePath(path.parent_path() / part.mesh()).relative_path().generic_string());
            part.set_material(ResourceHelper::normalizePath(path.parent_path() / part.material()).relative_path().generic_string());
        }
        return model;
    });
}

std::shared_ptr<Asset::Material> loadMaterial(std::string_view filename) {
    const auto path = ResourceHelper::normalizePath(std::filesystem::path(filename));
    return ResourceHelper::materialCache.load(path, [&path] {
        PhysInputStream stream(path.relative_path());
        auto material = std::make_shared<Asset::Material>();
        if (!material->ParseFromZeroCopyStream(&stream)) {
            throw std::runtime_error("Cannot load material '" + path.generic_string() + "'");
        }
        if (!material->albedo_texture().empty()) {
            material->set_albedo_texture(
                ResourceHelper::normalizePath(path.parent_path() / material->albedo_texture()).relative_path().generic_string());
        }
        if (!material->normal_texture().empty()) {
            material->set_normal_texture(
                ResourceHelper::normalizePath(path.parent_path() / material->normal_texture()).relative_path().generic_string());
        }
        if (!material->metallic_roughness_texture().empty()) {
            material->set_metallic_roughness_texture(
                ResourceHelper::normalizePath(path.parent_path() / material->metallic_roughness_texture()).relative_path().generic_string());
        }
        return material;
    });
}

std::shared_ptr<Graphics::Mesh> loadMesh(std::string_view filename) {
    const auto path = ResourceHelper::normalizePath(std::filesystem::path(filename));
    return ResourceHelper::loadGraphics(ResourceHelper::meshCache, path, [&path] {
        PhysInputStream stream(path.relative_path());
        Asset::Mesh source(stream);
        static_assert(sizeof(Asset::Mesh::Vertex) == sizeof(Graphics::Mesh::Vertex));
        static_assert(sizeof(Graphics::Mesh::Vertex) == 32);
        std::vector<Graphics::Mesh::Vertex> vertices(source.vertexCount());
        std::vector<std::uint32_t> indices(source.indexCount());
        if (!vertices.empty()) {
            ResourceHelper::readExactBytes(source, vertices.data(),
                                           static_cast<int>(vertices.size() * sizeof(Graphics::Mesh::Vertex)));
        }
        if (!indices.empty()) {
            ResourceHelper::readExactBytes(source, indices.data(),
                                           static_cast<int>(indices.size() * sizeof(std::uint32_t)));
        }
        return std::make_shared<Graphics::Mesh>(vertices, indices);
    });
}

std::shared_ptr<Graphics::Texture> loadTexture(std::string_view filename) {
    const auto path = ResourceHelper::normalizePath(std::filesystem::path(filename));
    return ResourceHelper::loadGraphics(ResourceHelper::textureCache, path, [&path] {
        PhysInputStream stream(path.relative_path());
        Asset::Texture source(stream);
        if (source.width() > std::numeric_limits<int>::max() ||
            source.height() > std::numeric_limits<int>::max()) {
            throw std::invalid_argument("Texture dimensions exceed integer range: " + path.generic_string());
        }
        const auto payloadSize = static_cast<std::size_t>(source.width()) * source.height() * 4;
        ByteArray pixels(payloadSize);
        ResourceHelper::readExactBytes(source, pixels.data(), static_cast<int>(payloadSize));

        auto texture = std::make_shared<Graphics::Texture>(static_cast<int>(source.width()),
                                                           static_cast<int>(source.height()));
        texture->uploadPremultiplied(pixels);
        return texture;
    });
}

std::shared_ptr<Graphics::Font> loadFont(std::string_view filename) {
    const auto path = ResourceHelper::normalizePath(std::filesystem::path(filename));
    return ResourceHelper::fontCache.load(path, [&path] {
        PhysInputStream file(path.relative_path());
        const auto bytes = file.readAll();
        return std::make_shared<Graphics::Font>(bytes);
    });
}

void clear() {
    ResourceHelper::sceneCache.clear();
    ResourceHelper::modelCache.clear();
    ResourceHelper::materialCache.clear();
    ResourceHelper::fontCache.clear();
    const std::lock_guard lock(ResourceHelper::contextMutex);
    ResourceHelper::meshCache.clear();
    ResourceHelper::textureCache.clear();
}

} // namespace Resources
VOLCANO_FRAMEWORK_END
