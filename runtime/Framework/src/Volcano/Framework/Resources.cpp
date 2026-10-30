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

#include <Volcano/Cache.h>
#include <Volcano/PhysFile.h>
#include <Volcano/Graphics/Font.h>
#include <Volcano/Graphics/Mesh.h>
#include <Volcano/Graphics/Texture.h>
#include <Volcano/Schema/Material.h>
#include <Volcano/Schema/Mesh.h>
#include <Volcano/Schema/Model.h>
#include <Volcano/Schema/Texture.h>
#include <Volcano/Framework/Common.h>
#include <Volcano/Framework/Resources.h>

VOLCANO_FRAMEWORK_BEGIN
namespace Resources {
namespace {
std::filesystem::path resourcePath(const std::filesystem::path& path) {
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

template <typename Resource> Resource readResource(const std::filesystem::path& path) {
    try {
        PhysFile file(path.relative_path());
        return Resource::read(file);
    } catch (const std::exception& error) {
        throw std::runtime_error("Cannot load resource '" + path.generic_string() + "': " + error.what());
    }
}

template <typename T> using ResourceCache = Cache<T, std::filesystem::path>;

template <typename T> auto weakPolicy() {
    return std::make_unique<WeakCachePolicy<T, std::filesystem::path>>();
}

ResourceCache<Schema::Model> modelCache(weakPolicy<Schema::Model>());
ResourceCache<Schema::Material> materialCache(weakPolicy<Schema::Material>());
ResourceCache<Schema::Mesh> meshDataCache(weakPolicy<Schema::Mesh>());
ResourceCache<Schema::Texture> textureDataCache(weakPolicy<Schema::Texture>());
ResourceCache<Graphics::Font> fontCache(weakPolicy<Graphics::Font>());

std::mutex contextMutex;
std::map<GL3WProcs*, std::shared_ptr<ResourceCache<Graphics::Mesh>>> meshCache;
std::map<GL3WProcs*, std::shared_ptr<ResourceCache<Graphics::Texture>>> textureCache;

template <typename T, typename Loader>
std::shared_ptr<T> loadGraphics(std::map<GL3WProcs*, std::shared_ptr<ResourceCache<T>>>& contexts,
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
} // namespace

std::shared_ptr<Schema::Model> loadModel(std::string_view filename) {
    const auto path = resourcePath(std::filesystem::path(filename));
    return modelCache.load(path, [&path] {
        auto source = readResource<Schema::Model>(path);
        for (auto& part : source.parts) {
            part.mesh = resourcePath(path.parent_path() / part.mesh).relative_path();
            part.material = resourcePath(path.parent_path() / part.material).relative_path();
        }
        return std::make_shared<Schema::Model>(std::move(source));
    });
}

std::shared_ptr<Schema::Material> loadMaterial(std::string_view filename) {
    const auto path = resourcePath(std::filesystem::path(filename));
    return materialCache.load(path, [&path] {
        auto source = readResource<Schema::Material>(path);
        for (auto* texture :
             {&source.albedoTexture, &source.normalTexture, &source.metallicRoughnessTexture}) {
            if (!texture->empty()) {
                *texture = resourcePath(path.parent_path() / *texture).relative_path();
            }
        }
        return std::make_shared<Schema::Material>(std::move(source));
    });
}

std::shared_ptr<Schema::Mesh> loadMeshData(std::string_view filename) {
    const auto path = resourcePath(std::filesystem::path(filename));
    return meshDataCache.load(path, [&path] {
        return std::make_shared<Schema::Mesh>(readResource<Schema::Mesh>(path));
    });
}

std::shared_ptr<Schema::Texture> loadTextureData(std::string_view filename) {
    const auto path = resourcePath(std::filesystem::path(filename));
    return textureDataCache.load(path, [&path] {
        return std::make_shared<Schema::Texture>(readResource<Schema::Texture>(path));
    });
}

std::shared_ptr<Graphics::Mesh> loadMesh(std::string_view filename) {
    const auto path = resourcePath(std::filesystem::path(filename));
    return loadGraphics(meshCache, path, [&path] {
        const auto source = readResource<Schema::Mesh>(path);
        std::vector<Graphics::Mesh::Vertex> vertices;
        vertices.reserve(source.vertices.size());
        for (const auto& vertex : source.vertices) {
            vertices.push_back({vertex.position, vertex.normal, vertex.texcoord});
        }
        return std::make_shared<Graphics::Mesh>(vertices, source.indices);
    });
}

std::shared_ptr<Graphics::Texture> loadTexture(std::string_view filename) {
    const auto path = resourcePath(std::filesystem::path(filename));
    return loadGraphics(textureCache, path, [&path] {
        const auto source = readResource<Schema::Texture>(path);
        if (source.width > std::numeric_limits<int>::max() ||
            source.height > std::numeric_limits<int>::max()) {
            throw std::invalid_argument("Texture dimensions exceed integer range: " + path.generic_string());
        }
        auto texture = std::make_shared<Graphics::Texture>(static_cast<int>(source.width),
                                                           static_cast<int>(source.height));
        texture->uploadPremultiplied(source.pixels);
        return texture;
    });
}

std::shared_ptr<Graphics::Font> loadFont(std::string_view filename) {
    const auto path = resourcePath(std::filesystem::path(filename));
    return fontCache.load(path, [&path] {
        PhysFile file(path.relative_path());
        const auto bytes = file.readAll();
        return std::make_shared<Graphics::Font>(bytes);
    });
}

void clear() {
    modelCache.clear();
    materialCache.clear();
    meshDataCache.clear();
    textureDataCache.clear();
    fontCache.clear();
    const std::lock_guard lock(contextMutex);
    meshCache.clear();
    textureCache.clear();
}

} // namespace Resources
VOLCANO_FRAMEWORK_END
