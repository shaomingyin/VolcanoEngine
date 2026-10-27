#include <limits>
#include <span>
#include <stdexcept>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include <Volcano/Graphics/View.h>

namespace {

using namespace Volcano::Graphics;

constexpr std::size_t LightCapacity = 16;

void buildView(View& view, std::span<const Light> lights) {
    view.begin();
    for (const auto& light : lights) {
        view.addLight(light);
    }
    view.end();
}

TEST(GraphicsLighting, ViewOwnsLightsAndEnvironment) {
    View view;
    EXPECT_TRUE(view.lights().empty());
    EXPECT_TRUE(view.ambientLight().isZero());
    EXPECT_EQ(view.clearColor(), Volcano::Colorf::Black);
    Light light;
    view.begin();
    view.addLight(light);
    view.setAmbientLight(Eigen::Vector3f::Constant(0.1f));
    view.setClearColor(Volcano::Colorf::Blue);
    view.end();
    auto copy = view;
    light.strength = 2.0f;
    copy.begin();
    copy.addLight(light);
    copy.setAmbientLight(Eigen::Vector3f::Zero());
    copy.setClearColor(Volcano::Colorf::Red);
    copy.end();
    EXPECT_EQ(view.lights()[0].strength, 1.0f);
    EXPECT_TRUE(view.ambientLight().isApprox(Eigen::Vector3f::Constant(0.1f)));
    EXPECT_EQ(view.clearColor(), Volcano::Colorf::Blue);
    EXPECT_TRUE(view.ready());
    EXPECT_TRUE(copy.ready());
}

TEST(GraphicsLighting, ValidatesShaderLightCapacity) {
    EXPECT_NE(Light::shaderSource(3).find("#define MAX_LIGHTS 3\n"), std::string::npos);
    View view;
    std::vector<Light> lights(3);
    EXPECT_NO_THROW(buildView(view, lights));
    EXPECT_THROW(Light::shaderSource(0), std::invalid_argument);
    EXPECT_THROW(Light::shaderSource(std::numeric_limits<std::size_t>::max()), std::invalid_argument);
}

TEST(GraphicsLighting, PreparesLightsAndValidatesRadiance) {
    View view;
    std::vector<Light> lights;
    EXPECT_NO_THROW(buildView(view, lights));
    lights.resize(LightCapacity);
    EXPECT_NO_THROW(buildView(view, lights));
    lights.push_back({});
    EXPECT_NO_THROW(buildView(view, lights)); // Renderer owns its light capacity limit.
    lights.resize(1);
    auto& light = lights[0];
    for (float value : {-1.0f, std::numeric_limits<float>::infinity(), std::numeric_limits<float>::quiet_NaN()}) {
        light.strength = value;
        EXPECT_THROW(buildView(view, lights), std::invalid_argument);
        light.strength = 1.0f;
        light.color.x() = value;
        EXPECT_THROW(buildView(view, lights), std::invalid_argument);
        light.color.setOnes();
        view.setAmbientLight({value, 0, 0});
        EXPECT_THROW(buildView(view, lights), std::invalid_argument);
        view.setAmbientLight(Eigen::Vector3f::Zero());
    }

    light.color.setConstant(std::numeric_limits<float>::max());
    light.strength = 2.0f;
    EXPECT_THROW(buildView(view, lights), std::invalid_argument);
    light.color.setOnes();
    light.strength = 0.0f;
    EXPECT_NO_THROW(buildView(view, lights));
    light.type = static_cast<Light::Type>(-1);
    EXPECT_THROW(buildView(view, lights), std::invalid_argument);
}

TEST(GraphicsLighting, ValidatesDirectionsRangesAndSpotCones) {
    View view;
    std::vector<Light> lights(1);
    auto& light = lights[0];
    light.direction.setZero();
    EXPECT_THROW(buildView(view, lights), std::invalid_argument);
    light.direction.z() = std::numeric_limits<float>::quiet_NaN();
    EXPECT_THROW(buildView(view, lights), std::invalid_argument);
    light.direction = {0.0f, 0.0f, -10.0f};
    EXPECT_NO_THROW(buildView(view, lights));
    light.type = Light::Type::Point;
    for (float range : {0.0f, -1.0f, std::numeric_limits<float>::infinity()}) {
        light.range = range;
        EXPECT_THROW(buildView(view, lights), std::invalid_argument);
    }

    light.range = 10.0f;
    light.position.x() = std::numeric_limits<float>::quiet_NaN();
    EXPECT_THROW(buildView(view, lights), std::invalid_argument);
    light.position.setZero();
    light.type = Light::Type::Spot;
    light.innerAngle = 0.0f;
    light.outerAngle = 90.0f;
    EXPECT_NO_THROW(buildView(view, lights));
    for (const auto [inner, outer] : {std::pair{-1.0f, 30.0f}, {30.0f, 30.0f}, {31.0f, 30.0f},
                                     {0.0f, 91.0f}, {0.0f, 0.000001f},
                                     {0.0f, std::numeric_limits<float>::quiet_NaN()}}) {
        light.innerAngle = inner;
        light.outerAngle = outer;
        EXPECT_THROW(buildView(view, lights), std::invalid_argument);
    }
}

}
