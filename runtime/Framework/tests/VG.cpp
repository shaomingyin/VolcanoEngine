#define GLFW_INCLUDE_NONE

#include <array>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include <GL/gl.hpp>
#include <GLFW/glfw3.h>
#include <gtest/gtest.h>

#include <Volcano/ScopeGuard.h>
#include <Volcano/PhysFile.h>
#include <Volcano/Graphics/FrameBuffer.h>
#include <Volcano/Gui/Button.h>
#include <Volcano/Gui/Layout.h>
#include <Volcano/Gui/Image.h>
#include <Volcano/World/Hud.h>
#include <Volcano/World/OrthographicCamera.h>
#include <Volcano/Framework/Local.h>

namespace {
using namespace Volcano;

class EmptyScene final : public World::Scene {
public:
    EmptyScene() {
        camera_ = create();
        emplace<World::OrthographicCamera>(camera_,-1,1,-1,1,0.1f,10);
    }
    entt::entity player() const noexcept override { return camera_; }

protected:
    void onUpdate(Clock::duration) override {}

private:
    entt::entity camera_;
};

class VGRender : public ::testing::Test {
protected:
    void SetUp() override {
        ASSERT_TRUE(glfwInit());
        glfwWindowHint(GLFW_VISIBLE,GLFW_FALSE);
        local_ = std::make_unique<Framework::Local>(64,64,"VG check");
        target_ = std::make_unique<Graphics::FrameBuffer>(64,64);
    }
    void TearDown() override {
        target_.reset();
        local_.reset();
        glfwTerminate();
    }
    std::array<std::uint8_t,4> pixel(const Graphics::Target& target,int x,int top) {
        target.bind();
        std::array<std::uint8_t,4> result{};
        glReadPixels(x,target.height()-1-top,1,1,GL_RGBA,GL_UNSIGNED_BYTE,result.data());
        return result;
    }
    void rectangle(Graphics::Canvas& canvas,const Colorf& color) {
        canvas.setColor(color);
        canvas.beginPath();
        canvas.rect(0,0,64,64);
        canvas.fill();
    }

protected:
    std::unique_ptr<Framework::Local> local_;
    std::unique_ptr<Graphics::FrameBuffer> target_;
};

TEST_F(VGRender, FillsHolesAndPreservesCommandOrderAndAlpha) {
    Graphics::Canvas canvas;
    canvas.begin();
    canvas.setColor(Colorf::Red);
    canvas.beginPath();
    canvas.rect(4,4,56,56);
    canvas.rect(20,20,24,24);
    canvas.fill(Graphics::Canvas::FillRule::EvenOdd);
    canvas.end();
    local_->renderer().renderVG(canvas,*target_);
    EXPECT_EQ(pixel(*target_,10,10),(std::array<std::uint8_t,4>{255,0,0,255}));
    EXPECT_EQ(pixel(*target_,32,32),(std::array<std::uint8_t,4>{0,0,0,0}));
    canvas.begin();
    rectangle(canvas,{1,0,0,0.5f});
    rectangle(canvas,{0,0,1,0.5f});
    canvas.end();
    local_->renderer().renderVG(canvas,*target_);
    const auto value=pixel(*target_,32,32);
    EXPECT_NEAR(value[0],64,2);
    EXPECT_NEAR(value[2],128,2);
    EXPECT_NEAR(value[3],192,2);
    EXPECT_EQ(glGetError(),GL_NO_ERROR);
}

TEST_F(VGRender, ClipsGradientsTransformsStrokesAndTextureOrientation) {
    Graphics::Canvas canvas;
    canvas.begin();
    canvas.setClip({8,8,48,48});
    canvas.setLinearGradient({8,8},{56,8},Colorf::Red,Colorf::Blue);
    canvas.beginPath();
    canvas.rect(0,0,64,64);
    canvas.fill();
    canvas.end();
    local_->renderer().renderVG(canvas,*target_);
    EXPECT_EQ(pixel(*target_,2,2)[3],0);
    EXPECT_GT(pixel(*target_,12,20)[0],pixel(*target_,12,20)[2]);
    EXPECT_GT(pixel(*target_,52,20)[2],pixel(*target_,52,20)[0]);
    const std::array<std::uint8_t,8> pixels{0,0,255,255,255,0,0,255};
    Graphics::Texture image(1,2,pixels);
    canvas.begin();
    canvas.image(image,{0,0,64,64});
    canvas.end();
    local_->renderer().renderVG(canvas,*target_);
    EXPECT_GT(pixel(*target_,32,4)[0],240);
    EXPECT_GT(pixel(*target_,32,60)[2],240);
    canvas.begin();
    Eigen::Affine2f transform=Eigen::Affine2f::Identity();
    transform.translate(Eigen::Vector2f(8,8));
    canvas.setTransform(transform);
    canvas.setColor(Colorf::Green);
    canvas.setStrokeWidth(6);
    canvas.setLineCap(Graphics::Canvas::LineCap::Round);
    canvas.beginPath();
    canvas.moveTo(8,20);
    canvas.bezierTo(16,20,32,20,40,20);
    canvas.stroke();
    canvas.end();
    local_->renderer().renderVG(canvas,*target_);
    EXPECT_GT(pixel(*target_,28,28)[1],250);
    EXPECT_EQ(pixel(*target_,28,40)[3],0);
    EXPECT_EQ(glGetError(),GL_NO_ERROR);
}

TEST_F(VGRender, RunsViewGenerationBeforeSceneAndOverlayAfterScene) {
    Graphics::FrameBuffer generated(64,64);
    Graphics::Canvas producer,overlay;
    producer.begin();
    rectangle(producer,Colorf::Green);
    producer.end();
    overlay.begin();
    overlay.image(generated.texture(),{0,0,64,64});
    overlay.end();
    Graphics::View view;
    view.begin();
    view.add(generated,producer);
    view.add(overlay);
    view.end();
    local_->renderer().render(view,*target_);
    EXPECT_EQ(pixel(*target_,32,32),(std::array<std::uint8_t,4>{0,255,0,255}));
    Graphics::Canvas feedback;
    feedback.begin();
    feedback.image(target_->texture(),{0,0,64,64});
    feedback.end();
    EXPECT_THROW(local_->renderer().renderVG(feedback,*target_),std::invalid_argument);
}

TEST_F(VGRender, ResolvesSharedSourcesTracksGUIChangesAndRejectsCycles) {
    int generated=0;
    local_->setDrawingProvider("paint",[&](Graphics::Canvas& canvas) {
        ++generated;
        rectangle(canvas,Colorf::Red);
    });
    local_->load([] {
        auto scene=std::make_unique<EmptyScene>();
        scene->addTexture("drawing",std::make_unique<World::DrawingTexture>(Size{64,64},"paint"));
        auto root=std::make_unique<Gui::Layout>();
        auto* image = new Gui::Image(root.get());
        image->setSource("drawing");
        image->setFrame({0,0,64,64});
    
        scene->addTexture("gui",std::make_unique<World::GuiTexture>(Size{64,64},
            std::make_unique<Gui::Document>(std::move(root))));
        auto& hud=scene->emplace<World::Hud>(scene->create());
        hud.setTexture("gui");
        hud.setRect({0,0,64,64});
        return scene;
    });
    local_->render(*target_,local_->scene().player());
    EXPECT_EQ(generated,1);
    EXPECT_EQ(pixel(*target_,32,32)[0],255);
    local_->render(*target_,local_->scene().player());
    EXPECT_EQ(generated,2);
    auto& drawing=static_cast<World::DrawingTexture&>(local_->scene().texture("drawing"));
    drawing.setDependencies({"gui"});
    EXPECT_THROW(local_->render(*target_,local_->scene().player()),std::invalid_argument);
    drawing.setDependencies({});
    local_->render(*target_,local_->scene().player());
    EXPECT_EQ(generated,3);
    EXPECT_EQ(glGetError(),GL_NO_ERROR);
}

TEST_F(VGRender, CachesChineseGlyphsWithinBudgetAcrossEviction) {
    const char* path=std::getenv("VE_TEST_CJK_FONT");
    if (!path) {
        GTEST_SKIP() << "Set VE_TEST_CJK_FONT for Chinese atlas verification";
    }
    std::ifstream file(path,std::ios::binary);
    ASSERT_TRUE(file.good());
    std::vector<std::uint8_t> bytes(std::istreambuf_iterator<char>(file),{});
    Graphics::Font font(bytes);
    auto& renderer=local_->renderer();
    renderer.setFontAtlasBudget(1024*1024);
    const auto start=Clock::now();
    renderer.prewarm(font,"中文字体缓存，设备运行正常。",24);
    const auto warmed=Clock::now();
    const auto coldStats = renderer.fontCacheStats();
    renderer.prewarm(font,"中文字体缓存，设备运行正常。",24);
    EXPECT_EQ(renderer.fontCacheStats().rasterizations,coldStats.rasterizations);
    EXPECT_EQ(renderer.fontCacheStats().uploadedBytes,coldStats.uploadedBytes);
    EXPECT_GT(renderer.fontCacheStats().hits,coldStats.hits);
    Graphics::Canvas canvas;
    canvas.begin();
    canvas.setFont(font,24);
    canvas.setColor(Colorf::White);
    canvas.text(0,28,"中文");
    canvas.end();
    renderer.renderVG(canvas,*target_);
    glFinish();
    std::array<std::uint8_t,64*64*4> first{};
    target_->bind();
    glReadPixels(0,0,64,64,GL_RGBA,GL_UNSIGNED_BYTE,first.data());
    EXPECT_GT(std::count_if(first.begin(),first.end(),[](auto x) { return x>0; }),100);
    for (unsigned int size : {96,128,192,256}) {
        renderer.prewarm(font,"中文字体缓存设备运行正常性能资源占用世界场景",size);
        EXPECT_LE(renderer.fontAtlasBytes(),1024*1024);
    }
    renderer.renderVG(canvas,*target_);
    std::array<std::uint8_t,64*64*4> second{};
    target_->bind();
    glReadPixels(0,0,64,64,GL_RGBA,GL_UNSIGNED_BYTE,second.data());
    EXPECT_EQ(first,second);
    const auto finished=Clock::now();
    RecordProperty("cold_prewarm_us",std::chrono::duration_cast<std::chrono::microseconds>(warmed-start).count());
    RecordProperty("eviction_and_render_us",std::chrono::duration_cast<std::chrono::microseconds>(finished-warmed).count());
    RecordProperty("font_bytes",static_cast<int>(font.byteSize()));
    RecordProperty("glyph_uploaded_bytes",static_cast<int>(renderer.fontCacheStats().uploadedBytes));
    RecordProperty("atlas_bytes",static_cast<int>(renderer.fontAtlasBytes()));
    EXPECT_EQ(glGetError(),GL_NO_ERROR);
}
TEST_F(VGRender, LoadsGUIFontsRollsBackFailedSyncAndCachesUnchangedOutput) {
    const char* path=std::getenv("VE_TEST_CJK_FONT");
    if (!path) {
        GTEST_SKIP() << "Set VE_TEST_CJK_FONT for GUI font loading";
    }
    const std::filesystem::path fontPath(path);
    ASSERT_NE(PHYSFS_init("VGCheck"),0);
    auto cleanup=scopeGuard([this] {
        local_->unload();
        PHYSFS_deinit();
    });
    ASSERT_NE(PHYSFS_mount(fontPath.parent_path().c_str(),nullptr,1),0);
    Gui::Button* button=nullptr;
    int clicked=0;
    local_->load([&] {
        auto scene=std::make_unique<EmptyScene>();
        auto root=std::make_unique<Gui::Layout>();
        auto* label = new Gui::Button(root.get());
        button=label;
        label->setFrame({0,0,64,64});
        label->setFont(fontPath.filename());
        label->setText("中文");
        label->setFontSize(24);
        label->setOnClick([&] { ++clicked; });
    
        scene->addTexture("gui",std::make_unique<World::GuiTexture>(Size{64,64},
            std::make_unique<Gui::Document>(std::move(root))));
        auto& hud=scene->emplace<World::Hud>(scene->create());
        hud.setTexture("gui");
        hud.setRect({0,0,64,64});
        return scene;
    });
    local_->render(*target_,local_->scene().player());
    const auto* texture=&local_->texture("gui");
    local_->synchronizeResources();
    EXPECT_EQ(&local_->texture("gui"),texture);
    local_->render(*target_,local_->scene().player());
    const auto stats=local_->renderer().fontCacheStats();
    local_->render(*target_,local_->scene().player());
    EXPECT_EQ(local_->renderer().fontCacheStats().hits,stats.hits);
    EXPECT_EQ(local_->renderer().fontCacheStats().rasterizations,stats.rasterizations);
    EXPECT_TRUE(local_->pointer("gui",Gui::PointerEvent{Gui::PointerEvent::Action::Down,10,10}));
    EXPECT_TRUE(local_->pointer("gui",Gui::PointerEvent{Gui::PointerEvent::Action::Up,10,10}));
    EXPECT_EQ(clicked,1);
    button->setFont("missing-font.ttf");
    EXPECT_THROW(local_->synchronizeResources(),std::runtime_error);
    EXPECT_EQ(&local_->texture("gui"),texture);
    button->setFont(fontPath.filename());
    local_->render(*target_,local_->scene().player());
    EXPECT_EQ(glGetError(),GL_NO_ERROR);
}

TEST_F(VGRender, RendersCameraOutputAndRejectsMissingProvidersOnLoad) {
    EXPECT_THROW(local_->load([] {
        auto scene=std::make_unique<EmptyScene>();
        scene->addTexture("missing",std::make_unique<World::DrawingTexture>(Size{64,64},"unregistered"));
        return scene;
    }),std::invalid_argument);
    EXPECT_EQ(local_->state(),Framework::Base::State::Idle);
    local_->load([] {
        auto scene=std::make_unique<EmptyScene>();
        scene->addTexture("camera",std::make_unique<World::CameraTexture>(Size{32,32},scene->player()));
        auto& hud=scene->emplace<World::Hud>(scene->create());
        hud.setTexture("camera");
        hud.setRect({0,0,64,64});
        return scene;
    });
    local_->render(*target_,local_->scene().player());
    EXPECT_EQ(local_->texture("camera").size(),(Size{32,32}));
    EXPECT_EQ(pixel(*target_,32,32),(std::array<std::uint8_t,4>{0,0,0,255}));
    const auto* committed = &local_->texture("camera");
    local_->scene().addTexture("missing",
        std::make_unique<World::DrawingTexture>(Size{8, 8}, "unregistered"));
    EXPECT_THROW(local_->synchronizeResources(), std::invalid_argument);
    EXPECT_EQ(&local_->texture("camera"), committed);
    EXPECT_EQ(local_->texture("camera").size(), (Size{32, 32}));
    auto& camera=static_cast<World::CameraTexture&>(local_->scene().texture("camera"));
    camera.setSize({16,24});
    local_->render(*target_,local_->scene().player());
    EXPECT_EQ(local_->texture("camera").size(),(Size{16,24}));
    EXPECT_EQ(glGetError(),GL_NO_ERROR);
}

}

namespace {
TEST(VGResources, ReleasesDrawingProviderCapturesBeforeDestroyingTheContext) {
    std::weak_ptr<Graphics::Texture> captured;
    {
        Framework::Local local(16, 16, "Drawing provider lifetime");
        auto image = std::make_shared<Graphics::Texture>(1, 1);
        captured = image;
        local.setDrawingProvider("owned-image", [image](Graphics::Canvas& canvas) {
            canvas.image(*image, {0, 0, 1, 1});
        });
        image.reset();
        EXPECT_FALSE(captured.expired());
    }
    EXPECT_TRUE(captured.expired());
    EXPECT_EQ(gl3wProcs, nullptr);
}
}
