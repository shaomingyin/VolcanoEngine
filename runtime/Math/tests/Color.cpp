#include <gtest/gtest.h>
#include <limits>
#include <type_traits>

#include <Volcano/Math/Color.h>

using Volcano::Math::Color;
using Volcano::Math::Colorf;

namespace {

template <typename Operation>
void outOfRange(Operation operation) {
    EXPECT_THROW(operation(), std::out_of_range);
}

TEST(MathColor, Conversions) {
    const auto packed = Color::fromRgba(0x12345678u);
    EXPECT_TRUE((packed.red() == 0x12 && packed.green() == 0x34));
    EXPECT_TRUE((packed.blue() == 0x56 && packed.alpha() == 0x78));
    EXPECT_TRUE((packed.toRgba() == 0x12345678u));
    EXPECT_TRUE((Color::fromRgbF(0, 0.5f, 1, 0.5f) == Color(0, 128, 255, 128)));
    EXPECT_TRUE((Color::White.toVector() == Volcano::Math::Vector4f(1)));
    EXPECT_TRUE((Color::Transparent.toVector() == Volcano::Math::Vector4f()));
    for (int value = 0; value <= 255; ++value) {
        const Color color(value, 255 - value, value, 255 - value);
        EXPECT_TRUE((Color::fromVector(color.toVector()) == color));
        EXPECT_TRUE((Color::fromRgba(color.toRgba()) == color));
    }
    Color color;
    color.setRed(10);
    color.setGreen(20);
    color.setBlue(30);
    color.setAlpha(40);
    EXPECT_TRUE((color == Color(10, 20, 30, 40)));
    outOfRange([&] { color.setRed(-1); });
    outOfRange([&] { color.setGreen(256); });
    outOfRange([&] { color.setBlue(-1); });
    outOfRange([&] { color.setAlpha(256); });
    EXPECT_TRUE((color == Color(10, 20, 30, 40)));

    for (std::size_t i = 0; i < 4; ++i) {
        for (const int bad : {-1, 256}) {
            int channels[] = {0, 0, 0, 255};
            channels[i] = bad;
            outOfRange([&] { Color invalid(channels[0], channels[1], channels[2], channels[3]); });
        }
        for (const float bad : {-0.01f, 1.01f, std::numeric_limits<float>::quiet_NaN(),
                                std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity()}) {
            float channels[] = {0, 0, 0, 1};
            channels[i] = bad;
            outOfRange([&] { Color::fromRgbF(channels[0], channels[1], channels[2], channels[3]); });
            outOfRange([&] { Color::fromVector({channels[0], channels[1], channels[2], channels[3]}); });
            outOfRange([&] { Color::fromHsvF(channels[0], channels[1], channels[2], channels[3]); });
        }
    }
}

TEST(MathColor, Hsv) {
    const Color primaries[] = {Color::Red, Color::Yellow, Color::Green, Color::Cyan, Color::Blue, Color::Magenta};
    for (int i = 0; i < 6; ++i) {
        EXPECT_TRUE((Color::fromHsvF(i / 6.0f, 1, 1) == primaries[i]));
    }
    EXPECT_TRUE((Color::fromHsvF(1, 1, 1) == Color::Red));
    EXPECT_TRUE((Color::fromHsvF(0.7f, 0, 0.5f, 0.5f) == Color(128, 128, 128, 128)));
    EXPECT_TRUE((Color::Black.toHsvF() == Volcano::Math::Vector3f()));
    EXPECT_TRUE((Color::White.toHsvF() == Volcano::Math::Vector3f(0, 0, 1)));
    for (int red = 0; red <= 255; red += 17) {
        for (int green = 0; green <= 255; green += 17) {
            for (int blue = 0; blue <= 255; blue += 17) {
                const Color color(red, green, blue, 37);
                const auto hsv = color.toHsvF();
                EXPECT_TRUE((hsv.x >= 0 && hsv.x < 1 && hsv.y >= 0 && hsv.y <= 1 && hsv.z >= 0 && hsv.z <= 1));
                EXPECT_TRUE((Color::fromHsvF(hsv.x, hsv.y, hsv.z, color.alphaF()) == color));
            }
        }
    }
}

TEST(MathColor, Arithmetic) {
    for (int left = 0; left <= 255; ++left) {
        for (int right = 0; right <= 255; ++right) {
            const Color a(left, left, left, left), b(right, right, right, right);
            const int sum = left + right;
            const int difference = left - right;
            const int product = left * right / 255;
            const int added = sum > 255 ? 255 : sum;
            const int subtracted = difference < 0 ? 0 : difference;
            EXPECT_TRUE((a + b == Color(added, added, added, added)));
            EXPECT_TRUE((a - b == Color(subtracted, subtracted, subtracted, subtracted)));
            EXPECT_TRUE((a * b == Color(product, product, product, product)));
        }
    }
    Color color(200, 100, 50, 128);
    EXPECT_TRUE((&(color += color) == &color && color == Color(255, 200, 100, 255)));
    EXPECT_TRUE((&(color *= color) == &color && color == Color(255, 156, 39, 255)));
    EXPECT_TRUE((&(color -= color) == &color && color == Color::Transparent));
}

TEST(MathColor, Float) {
    const Colorf hdr(2.5f, 0.5f, -0.25f, 0.5f);
    EXPECT_TRUE((hdr.red() == 2.5f && hdr.green() == 0.5f && hdr.blue() == -0.25f && hdr.alpha() == 0.5f));
    EXPECT_TRUE((hdr.toVector() == Volcano::Math::Vector4f(2.5f, 0.5f, -0.25f, 0.5f)));
    EXPECT_TRUE((hdr.toFloat() == hdr && Colorf::fromVector(hdr.toVector()) == hdr));
    EXPECT_TRUE((hdr.toByte() == Color(255, 128, 0, 128)));
    EXPECT_TRUE((hdr.toRgba() == hdr.toByte().toRgba()));
    outOfRange([&] { hdr.toHsvF(); });
    EXPECT_TRUE((Colorf::fromRgbF(0.1f, 0.2f, 0.3f, 0.4f) == Colorf(0.1f, 0.2f, 0.3f, 0.4f)));
    EXPECT_TRUE((Colorf::fromRgbF(2, -1, 0) == Colorf(2, -1, 0)));
    EXPECT_TRUE((Colorf::fromHsvF(0, 1, 4, 0.5f) == Colorf(4, 0, 0, 0.5f)));
    EXPECT_TRUE((Colorf(4, 0, 0).toHsvF() == Volcano::Math::Vector3f(0, 1, 4)));
    outOfRange([] { Colorf::fromHsvF(-0.1f, 1, 1); });
    outOfRange([] { Colorf::fromHsvF(0, 1.1f, 1); });
    outOfRange([] { Colorf::fromHsvF(0, 1, -1); });

    const Colorf a(0.75f, 2, -1, 0.5f), b(0.5f, 3, 2, 0.5f);
    EXPECT_TRUE((a + b == Colorf(1.25f, 5, 1, 1)));
    EXPECT_TRUE((a - b == Colorf(0.25f, -1, -3, 0)));
    EXPECT_TRUE((a * b == Colorf(0.375f, 6, -2, 0.25f)));
    auto self = a;
    EXPECT_TRUE((&(self += self) == &self && self == a + a));
    EXPECT_TRUE((&(self *= self) == &self && self == Colorf(2.25f, 16, 4, 1)));
    EXPECT_TRUE((&(self -= self) == &self && self == Colorf::Transparent));

    Colorf modified;
    modified.setRed(2);
    modified.setGreen(-0.5f);
    modified.setBlue(0.25f);
    modified.setAlpha(0.75f);
    const auto before = modified;
    for (const float invalid : {std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::infinity(),
                                -std::numeric_limits<float>::infinity()}) {
        for (std::size_t i = 0; i < 4; ++i) {
            float values[] = {0, 0, 0, 1};
            values[i] = invalid;
            outOfRange([&] { Colorf bad(values[0], values[1], values[2], values[3]); });
            outOfRange([&] { Colorf::fromVector({values[0], values[1], values[2], values[3]}); });
            outOfRange([&] { Colorf::fromHsvF(values[0], values[1], values[2], values[3]); });
        }
        outOfRange([&] { modified.setRed(invalid); });
        outOfRange([&] { modified.setGreen(invalid); });
        outOfRange([&] { modified.setBlue(invalid); });
        outOfRange([&] { modified.setAlpha(invalid); });
        EXPECT_TRUE((modified == before));
    }
    for (int value = 0; value <= 255; ++value) {
        const Color color(value, 255 - value, value, 255 - value);
        EXPECT_TRUE((color.toFloat().toByte() == color));
        EXPECT_TRUE((color.toByte() == color));
        EXPECT_TRUE((Colorf::fromRgba(color.toRgba()).toByte() == color));
        const auto floating = color.toFloat();
        const auto hsv = floating.toHsvF();
        const auto restored = Colorf::fromHsvF(hsv.x, hsv.y, hsv.z, floating.alpha());
        EXPECT_TRUE(((restored.toVector() - floating.toVector()).length() < 1e-6f));
    }
}

} // namespace

static_assert(sizeof(Color) == 4);
static_assert(sizeof(Colorf) == sizeof(float) * 4);
static_assert(std::is_same_v<Color, Volcano::Math::BasicColor<std::uint8_t>>);
static_assert(std::is_same_v<Colorf, Volcano::Math::BasicColor<float>>);
static_assert(!std::is_convertible_v<Color, Colorf> && !std::is_convertible_v<Colorf, Color>);
static_assert(std::is_trivially_copyable_v<Color>);
static_assert(Color() == Color::Black);
static_assert(Color::White != Color::Black);
static_assert(Color::Transparent.alpha() == 0);
static_assert(Color::Red.toRgba() == 0xff0000ffu);
static_assert(Color::fromRgba(0xffffffffu) == Color::White);
static_assert(Color::fromRgbF(1, 0, 0) == Color::Red);
static_assert(Color::fromHsvF(0, 1, 1) == Color::Red);
static_assert(Color::Red.toHsvF() == Volcano::Math::Vector3f(0, 1, 1));
static_assert(Color::Red + Color::Blue == Color::Magenta);
static_assert(Colorf() == Colorf::Black);
static_assert(Color::White.toFloat() == Colorf::White);
static_assert(Colorf::Red.toByte() == Color::Red);
static_assert(Colorf::Red + Colorf::Red == Colorf(2, 0, 0, 2));

