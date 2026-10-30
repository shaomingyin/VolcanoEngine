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
#include <Volcano/Graphics/Canvas.h>
#include <Volcano/Graphics/Font.h>
#include <Volcano/Graphics/FrameBuffer.h>
#include <Volcano/Graphics/Mesh.h>
#include <Volcano/Graphics/Renderer.h>
#include <Volcano/Graphics/Target.h>
#include <Volcano/Graphics/Texture.h>
#include <Volcano/Graphics/View.h>
#include <Volcano/Gui/Event.h>
#include <Volcano/Schema/Material.h>
#include <Volcano/Framework/Base.h>
#include <Volcano/Framework/Window.h>

VOLCANO_FRAMEWORK_BEGIN

// Owns the window and local subsystems; use on the main thread.
class Local: public Base {
public:
    explicit Local(int width = 800, int height = 600, const char* title = "Volcano");
    ~Local() override;

    const Graphics::Texture& texture(std::string_view id) const;
    void setDrawingProvider(std::string id, std::function<void(Graphics::Canvas&)> provider);

    bool pointer(std::string_view texture, const Gui::PointerEvent& event);
    bool key(std::string_view texture, const Gui::KeyEvent& event);
    void render(entt::entity camera);
    void render(Graphics::Target& target, entt::entity camera);

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
    async::task<void> onLoadScene(const World::Scene& scene) override;
    async::task<void> onLoadEntity(const World::Scene& scene, entt::entity entity) override;
    void onBeforeTasks() override;
    void onUnloadResources() noexcept override;
    void onCommitResources() noexcept override;
    void onDiscardResources() noexcept override;
    void onBeforeUpdate(Clock::duration elapsed) override;
    void onAfterUpdate(Clock::duration elapsed) override;

    virtual void onInputUpdate(Clock::duration elapsed);
    virtual void onAudioUpdate(Clock::duration elapsed);

private:
    struct TextureOutput {
    public:
        explicit TextureOutput(Size size);

    public:
        Graphics::FrameBuffer target;
        Graphics::Canvas canvas;
        std::uint64_t revision = 0;
        bool dirty = true;
    };

    void loadOutput(std::string id, Size size);
    void validateSamples(const Graphics::Canvas& canvas, const std::vector<std::string>& dependencies) const;
    static std::vector<std::string> dependencies(const World::Scene& scene, const std::string& id);
    static std::vector<std::string> textureOrder(const World::Scene& scene, const std::vector<std::string>& roots);
    void updateGuiInput();
    void buildView(Graphics::View& view, entt::entity camera);
    void updateTextures(const std::vector<std::string>& roots);

private:
    Window window_;
    Graphics::Renderer renderer_;
    Graphics::View view_;
    Graphics::Canvas hud_;
    std::map<std::filesystem::path, std::shared_ptr<Graphics::Font>> fonts_;
    std::map<std::filesystem::path, std::shared_ptr<Graphics::Font>> pending_fonts_;
    std::map<std::filesystem::path, std::shared_ptr<Schema::Material>> materials_;
    std::map<std::filesystem::path, std::shared_ptr<Schema::Material>> pending_materials_;
    std::map<std::filesystem::path, std::shared_ptr<Graphics::Mesh>> meshes_;
    std::map<std::filesystem::path, std::shared_ptr<Graphics::Mesh>> pending_meshes_;
    std::map<std::filesystem::path, std::shared_ptr<Graphics::Texture>> textures_;
    std::map<std::filesystem::path, std::shared_ptr<Graphics::Texture>> pending_textures_;
    std::map<std::string, std::unique_ptr<TextureOutput>> outputs_;
    std::map<std::string, std::unique_ptr<TextureOutput>> pending_outputs_;
    std::map<std::string, std::function<void(Graphics::Canvas&)>> drawing_providers_;
    const World::Scene* output_scene_ = nullptr;
    const World::Scene* pending_output_scene_ = nullptr;
    std::string pointer_gui_;
    std::string focused_gui_;
    Rectf pointer_rect_{};
    bool mouse_down_ = false;
    bool enter_down_ = false;
    bool space_down_ = false;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_LOCAL_H
