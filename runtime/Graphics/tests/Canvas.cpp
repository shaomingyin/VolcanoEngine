#include <array>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <Volcano/Graphics/Canvas.h>
#include <Volcano/Graphics/View.h>

namespace {
using namespace Volcano;
using namespace Volcano::Graphics;

TEST(Canvas, OwnsTextAndStateAndCanBeReused) {
    Canvas canvas;
    canvas.begin();
    canvas.save();
    canvas.setOpacity(0.5f);
    canvas.setClip({0,0,32,32});
    canvas.beginPath();
    canvas.roundedRect(0,0,20,20,4);
    canvas.fill();
    canvas.restore();
    canvas.end();
    EXPECT_TRUE(canvas.ready());
    auto copy = canvas;
    View view;
    view.begin();
    view.add(copy);
    view.end();
    EXPECT_TRUE(view.ready());
    ASSERT_EQ(view.overlays().size(),1);
    EXPECT_EQ(&view.overlays().front().get(),&copy);
    canvas.begin();
    EXPECT_FALSE(canvas.ready());
    canvas.end();
    EXPECT_TRUE(copy.ready());
    EXPECT_TRUE(canvas.sampledTextures().empty());
}

TEST(Canvas, RejectsNonFiniteParameters) {
    Canvas canvas;
    canvas.begin();
    EXPECT_THROW(canvas.setOpacity(-1),std::invalid_argument);
    EXPECT_THROW(canvas.moveTo(std::numeric_limits<float>::infinity(),0),std::invalid_argument);
    EXPECT_THROW(canvas.setStrokeWidth(0),std::invalid_argument);
    EXPECT_THROW(canvas.setClip({0,0,-1,1}),std::invalid_argument);
    Eigen::Affine2f singular = Eigen::Affine2f::Identity();
    singular.linear().setZero();
    EXPECT_THROW(canvas.setTransform(singular),std::invalid_argument);
    canvas.end();
}

std::unique_ptr<Font> testFont(const char* variable) {
    const char* path = std::getenv(variable);
    if (path == nullptr) {
        return {};
    }
    std::ifstream file(path,std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot read test font");
    }
    std::vector<std::uint8_t> bytes(std::istreambuf_iterator<char>(file),{});
    return std::make_unique<Font>(bytes);
}

TEST(Font, RejectsEmptyAndInvalidDataWithoutGPU) {
    EXPECT_THROW(Font(std::span<const std::uint8_t>{}), std::invalid_argument);
    for (int attempt = 0; attempt < 3; ++attempt) {
        EXPECT_THROW(Font((std::array<std::uint8_t, 3>{1, 2, 3})), std::runtime_error);
    }
}

TEST(Font, OwnsBytesShapesMeasuresAndRasterizesWithoutGPU) {
    auto font = testFont("VE_TEST_FONT");
    if (!font) {
        GTEST_SKIP() << "Set VE_TEST_FONT to a TTF/OTF/TTC fixture";
    }
    const auto glyphs = font->shape("Hello AV fi",24);
    ASSERT_FALSE(glyphs.empty());
    EXPECT_GT(font->measure("Hello",24).width,0);
    EXPECT_GT(font->measure("Hello  ",24).width,font->measure("Hello",24).width);
    EXPECT_GT(font->measure("Hello\nworld",24).height,font->measure("Hello",24).height);
    const auto bitmap = font->rasterize(glyphs.front().index,24);
    EXPECT_GT(bitmap.width,0);
    EXPECT_EQ(bitmap.pixels.size(),static_cast<std::size_t>(bitmap.width*bitmap.height));
    EXPECT_THROW(font->shape("hello",0),std::invalid_argument);
    EXPECT_THROW(font->setFallbacks(std::array<const Font*,1>{font.get()}),std::invalid_argument);
    auto chinese = testFont("VE_TEST_CJK_FONT");
    if (chinese) {
        font->setFallbacks(std::array<const Font*,1>{chinese.get()});
        bool fallback = false;
        for (const auto& glyph : font->shape("中文字体缓存",24)) {
            if (glyph.font) {
                EXPECT_NE(glyph.index,0);
                fallback = fallback || glyph.font==chinese.get();
            }
        }
        EXPECT_TRUE(fallback);
    }
}
}
