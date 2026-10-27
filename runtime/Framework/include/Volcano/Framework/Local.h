#ifndef VOLCANO_FRAMEWORK_LOCAL_H
#define VOLCANO_FRAMEWORK_LOCAL_H

#include <filesystem>
#include <map>
#include <memory>

#include <entt/entity/entity.hpp>

#include <Volcano/Graphics/Mesh.h>
#include <Volcano/Graphics/Renderer.h>
#include <Volcano/Graphics/Target.h>
#include <Volcano/Graphics/Texture.h>
#include <Volcano/Graphics/View.h>
#include <Volcano/Schema/Material.h>
#include <Volcano/ScopeGuard.h>
#include <Volcano/Framework/Base.h>
#include <Volcano/Framework/Window.h>

VOLCANO_FRAMEWORK_BEGIN

// Owns GLFW, the window and OpenGL context; use on the main thread.
class Local: public Base {
public:
    explicit Local(int width = 800, int height = 600, const char* title = "Volcano");
    ~Local() override;

    void render(entt::entity camera);
    void render(Graphics::Target& target, entt::entity camera);

    Window& window() noexcept;
    const Window& window() const noexcept;

    Graphics::Renderer& renderer() noexcept;
    const Graphics::Renderer& renderer() const noexcept;
    Graphics::View& view() noexcept;
    const Graphics::View& view() const noexcept;

protected:
    void onLoadResources(const World::Scene& scene) override;
    void onUnloadResources() noexcept override;
    void onCommitResources() noexcept override;
    void onDiscardResources() noexcept override;
    void onBeforeUpdate(Clock::duration elapsed) override;
    void onAfterUpdate(Clock::duration elapsed) override;

    virtual void onInputUpdate(Clock::duration elapsed);
    virtual void onAudioUpdate(Clock::duration elapsed);

private:
    void loadMesh(const std::filesystem::path& path);
    const Schema::Material& loadMaterial(const std::filesystem::path& path);
    void loadTexture(const std::filesystem::path& path);

private:
    ScopeGuard<void (*)()> glfw_runtime_;
    Window window_;
    Graphics::Renderer renderer_;
    Graphics::View view_;
    std::map<std::filesystem::path, Schema::Material> materials_;
    std::map<std::filesystem::path, std::unique_ptr<Graphics::Mesh>> meshes_;
    std::map<std::filesystem::path, std::unique_ptr<Graphics::Texture>> textures_;
    std::map<std::filesystem::path, Schema::Material> pending_materials_;
    std::map<std::filesystem::path, std::unique_ptr<Graphics::Mesh>> pending_meshes_;
    std::map<std::filesystem::path, std::unique_ptr<Graphics::Texture>> pending_textures_;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_LOCAL_H
