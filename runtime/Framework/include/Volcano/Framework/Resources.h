#ifndef VOLCANO_FRAMEWORK_RESOURCES_H
#define VOLCANO_FRAMEWORK_RESOURCES_H

#include <memory>
#include <string_view>

#include <Volcano/Asset/Common.h>
#include <Volcano/Graphics/Font.h>
#include <Volcano/Graphics/Mesh.h>
#include <Volcano/Graphics/Texture.h>
#include <Volcano/Framework/Common.h>

VOLCANO_FRAMEWORK_BEGIN
namespace Resources {

// Paths are rooted in PhysFS. Reuse live cached objects or load synchronously; failures throw.
// Callers own the returned handles.
std::shared_ptr<Asset::Scene> loadScene(std::string_view path);

std::shared_ptr<Asset::Model> loadModel(std::string_view path);

std::shared_ptr<Asset::Material> loadMaterial(std::string_view path);

std::shared_ptr<Graphics::Mesh> loadMesh(std::string_view path);

std::shared_ptr<Graphics::Texture> loadTexture(std::string_view path);

std::shared_ptr<Graphics::Font> loadFont(std::string_view path);

// Remove lookup entries without invalidating existing handles.
void clear();

} // namespace Resources
VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_RESOURCES_H
