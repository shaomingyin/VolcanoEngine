#ifndef VOLCANO_FRAMEWORK_LOCAL_H
#define VOLCANO_FRAMEWORK_LOCAL_H

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <entt/entity/entity.hpp>

#include <Volcano/Size.h>
#include <Volcano/Asset/Common.h>
#include <Volcano/Graphics/Font.h>
#include <Volcano/Graphics/FrameBuffer.h>
#include <Volcano/Graphics/Mesh.h>
#include <Volcano/Graphics/Model.h>
#include <Volcano/Graphics/Renderer.h>
#include <Volcano/Graphics/Target.h>
#include <Volcano/Graphics/Texture.h>
#include <Volcano/Graphics/View.h>
#include <Volcano/Gui/Event.h>
#include <Volcano/World/Canvas.h>
#include <Volcano/Framework/Base.h>
#include <Volcano/Framework/Window.h>

VOLCANO_FRAMEWORK_BEGIN

// Owns the window and local subsystems; use on the main thread.
class Local : public Base {
public:
    explicit Local(int width = 800, int height = 600, const char* title = "Volcano");
    ~Local() override;

    const Graphics::Texture& texture(entt::entity entity) const;
    const Graphics::Texture& texture(std::string_view id) const;
    void setDrawingProvider(std::string id, std::function<void(World::Canvas&)> provider);

    bool pointer(entt::entity entity, const Gui::PointerEvent& event);
    bool pointer(std::string_view texture, const Gui::PointerEvent& event);
    bool key(entt::entity entity, const Gui::KeyEvent& event);
    bool key(std::string_view texture, const Gui::KeyEvent& event);
    void render(entt::entity camera);
    void render(Graphics::Target& target, entt::entity camera);

    bool shouldClose() const noexcept override;

    Window& window() noexcept {
        return window_;
    }

    const Window& window() const noexcept {
        return window_;
    }

    Graphics::Renderer& renderer() noexcept {
        return renderer_;
    }

    const Graphics::Renderer& renderer() const noexcept {
        return renderer_;
    }

    Graphics::View& view() noexcept {
        return view_;
    }

    const Graphics::View& view() const noexcept {
        return view_;
    }

protected:
    exec::task<void> onLoadScene(const World::Scene& scene) override;
    exec::task<void> onLoadEntity(const World::Scene& scene, entt::entity entity) override;
    void onBeforeTasks() override;
    void onUnloadResources() noexcept override;
    void onCommitResources() noexcept override;
    void onDiscardResources() noexcept override;
    void onBeforeUpdate(Clock::duration elapsed) override;
    void onAfterUpdate(Clock::duration elapsed) override;

    virtual void onInputUpdate(Clock::duration elapsed);
    virtual void onAudioUpdate(Clock::duration elapsed);

    static Size cameraOutputSize(const World::Scene& scene, entt::entity entity);
    static Size outputSize(const World::Scene& scene, entt::entity entity);
    static bool hasOutput(const World::Scene& scene, entt::entity entity);
    static entt::entity findOutputEntity(const World::Scene& scene, std::string_view name);
    static entt::entity findGuiEntity(const World::Scene& scene, std::string_view name);

private:
    struct TextureOutput {
    public:
        explicit TextureOutput(Size size);

    public:
        Graphics::FrameBuffer target;
        World::Canvas canvas;
        std::uint64_t revision = 0;
        bool dirty = true;
    };

private:
    void loadOutput(entt::entity entity, Size size);
    void validateSamples(const World::Canvas& canvas, const std::vector<std::string>& dependencies) const;
    static std::string outputLabel(const World::Scene& scene, entt::entity entity);
    static std::vector<entt::entity> dependencies(const World::Scene& scene, entt::entity entity);
    static std::vector<entt::entity> textureOrder(const World::Scene& scene, const std::vector<entt::entity>& roots);
    void updateTextures(const std::vector<entt::entity>& roots);

private:
    Window window_;
    Graphics::Renderer renderer_;
    Graphics::View view_;
    std::map<std::filesystem::path, std::shared_ptr<Graphics::Font>> fonts_;
    std::map<std::filesystem::path, std::shared_ptr<Graphics::Font>> pending_fonts_;
    std::map<std::filesystem::path, std::shared_ptr<Asset::Material>> materials_;
    std::map<std::filesystem::path, std::shared_ptr<Asset::Material>> pending_materials_;
    std::map<std::filesystem::path, std::shared_ptr<Graphics::Mesh>> meshes_;
    std::map<std::filesystem::path, std::shared_ptr<Graphics::Mesh>> pending_meshes_;
    std::map<std::filesystem::path, std::shared_ptr<Graphics::Texture>> textures_;
    std::map<std::filesystem::path, std::shared_ptr<Graphics::Texture>> pending_textures_;
    std::map<entt::entity, Graphics::Model> pending_render_models_;
    std::map<entt::entity, std::unique_ptr<TextureOutput>> outputs_;
    std::map<entt::entity, std::unique_ptr<TextureOutput>> pending_outputs_;
    std::map<std::string, std::function<void(World::Canvas&)>> drawing_providers_;
    const World::Scene* output_scene_ = nullptr;
    const World::Scene* pending_output_scene_ = nullptr;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_LOCAL_H
