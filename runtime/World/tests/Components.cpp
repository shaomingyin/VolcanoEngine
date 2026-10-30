#include <filesystem>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

#include <gtest/gtest.h>

#include <Volcano/World/AudioListener.h>
#include <Volcano/World/AudioSource.h>
#include <Volcano/World/DirectionalLight.h>
#include <Volcano/World/Inherent.h>
#include <Volcano/World/Model.h>
#include <Volcano/World/OrthographicCamera.h>
#include <Volcano/World/PerspectiveCamera.h>
#include <Volcano/World/PointLight.h>
#include <Volcano/World/RigidBody.h>
#include <Volcano/World/Scene.h>
#include <Volcano/World/SpotLight.h>

namespace {
using namespace Volcano;
using namespace Volcano::World;
const float nan = std::numeric_limits<float>::quiet_NaN();
const float inf = std::numeric_limits<float>::infinity();

static_assert(std::has_virtual_destructor_v<Camera>);
static_assert(std::has_virtual_destructor_v<Light>);
static_assert(!std::is_base_of_v<DirectionalLight, SpotLight>);
static_assert(std::is_same_v<decltype(std::declval<PerspectiveCamera&>().offset()), const Transform&>);
static_assert(std::is_same_v<decltype(std::declval<SpotLight&>().direction()), const Eigen::Vector3f&>);
static_assert(std::is_same_v<decltype(std::declval<RigidBody&>().shape()), const RigidBody::Shape&>);
static_assert(std::is_same_v<decltype(std::declval<Model&>().source()), const std::filesystem::path&>);
static_assert(std::is_same_v<decltype(std::declval<Transform&>().translation()), const Eigen::Vector3f&>);

static_assert([] {
    RigidBody::Sphere sphere(2.0f);
    sphere.setRadius(3.0f);
    RigidBody::Capsule capsule(1.0f, 2.0f);
    capsule.setDimensions(3.0f, 4.0f);
    return sphere.radius() == 3.0f && capsule.halfHeight() == 3.0f && capsule.radius() == 4.0f;
}());

static_assert([] {
    Inherent metadata("object");
    metadata.setName("renamed");
    metadata.disable();
    metadata.hide();
    const auto copy = metadata;
    metadata.enable();
    metadata.show();
    return copy.name() == "renamed" && !copy.isEnabled() && !copy.isVisible() &&
        metadata.isEnabled() && metadata.isVisible();
}());

TEST(WorldComponents, CameraChangesAreValidatedAndPreservePreviousValuesOnFailure) {
    PerspectiveCamera camera;
    camera.setFov(60);
    camera.setAspect(2);
    Camera& base = camera;
    base.setZRange(1, 10);
    const auto before = camera.toMatrix();
    EXPECT_THROW(camera.setFov(180), std::invalid_argument);
    EXPECT_THROW(camera.setAspect(nan), std::invalid_argument);
    EXPECT_THROW(base.setZRange(-1, 10), std::invalid_argument);
    EXPECT_THROW(base.setZRange(1, inf), std::invalid_argument);
    EXPECT_EQ(camera.fov(), 60);
    EXPECT_EQ(camera.aspect(), 2);
    EXPECT_TRUE(camera.toMatrix().isApprox(before));
    Transform offset;
    offset.setTranslation({1, 2, 3});
    camera.setOffset(offset);
    offset.reset();
    EXPECT_EQ(camera.offset().translation(), Eigen::Vector3f(1, 2, 3));
    auto copy = camera;
    copy.setFov(45);
    EXPECT_EQ(camera.fov(), 60);
    OrthographicCamera orthographic;
    orthographic.setRect(-2, 2, -1, 1);
    orthographic.setZRange(-10, 10);
    EXPECT_THROW(orthographic.setRect(1, 1, -1, 1), std::invalid_argument);
    EXPECT_EQ(orthographic.rect(), Eigen::Vector4f(-2, 2, -1, 1));
    EXPECT_TRUE(orthographic.toMatrix().allFinite());
    std::unique_ptr<Camera> owned = std::make_unique<PerspectiveCamera>();
    EXPECT_TRUE(owned->toMatrix().allFinite());
}

TEST(WorldComponents, LightsValidateNormalizeAndPreserveTheirState) {
    DirectionalLight sun;
    sun.setDirection({3, 0, 4});
    EXPECT_TRUE(sun.direction().isApprox(Eigen::Vector3f(0.6f, 0, 0.8f)));
    EXPECT_THROW(sun.setDirection(Eigen::Vector3f::Zero()), std::invalid_argument);
    EXPECT_THROW(sun.setDirection({nan, 0, 1}), std::invalid_argument);
    Light& base = sun;
    base.setColor({2, 3, 4});
    base.setStrength(2);
    EXPECT_THROW(base.setColor({-1, 0, 0}), std::invalid_argument);
    EXPECT_THROW(base.setStrength(inf), std::invalid_argument);
    EXPECT_THROW(base.setStrength(std::numeric_limits<float>::max()), std::invalid_argument);
    EXPECT_EQ(base.color(), Eigen::Vector3f(2, 3, 4));
    EXPECT_EQ(base.strength(), 2);
    PointLight point;
    EXPECT_THROW(point.setRange(0), std::invalid_argument);
    EXPECT_EQ(point.range(), 10);
    SpotLight spot;
    spot.setDirection({0, 0, -2});
    spot.setRange(20);
    spot.setAngles(10, 25);
    for (const auto& angles : {Eigen::Vector2f(-1, 20), Eigen::Vector2f(20, 10),
            Eigen::Vector2f(20, 20), Eigen::Vector2f(10, 91), Eigen::Vector2f(nan, 30)}) {
        EXPECT_THROW(spot.setAngles(angles.x(), angles.y()), std::invalid_argument);
    }
    EXPECT_EQ(spot.innerAngle(), 10);
    EXPECT_EQ(spot.outerAngle(), 25);
    EXPECT_EQ(spot.direction(), Eigen::Vector3f(0, 0, -1));
    std::unique_ptr<Light> owned = std::make_unique<SpotLight>();
    EXPECT_EQ(owned->strength(), 1);
}

TEST(WorldComponents, RigidBodyAndShapesEnforceTheirOwnInvariants) {
    RigidBody body;
    body.setMotion(RigidBody::Motion::Kinematic);
    body.setMass(2);
    body.setFriction(0.5f);
    body.setRestitution(1);
    body.setLinearDamping(0);
    body.setAngularDamping(1);
    body.setGravityFactor(-1);
    for (const float bad : {-1.0f, 0.0f, nan, inf}) {
        EXPECT_THROW(body.setMass(bad), std::invalid_argument);
        EXPECT_THROW((RigidBody::Sphere{bad}), std::invalid_argument);
        EXPECT_THROW((RigidBody::Capsule{1, bad}), std::invalid_argument);
        EXPECT_THROW((RigidBody::Capsule{bad, 1}), std::invalid_argument);
        EXPECT_THROW((RigidBody::Box{Eigen::Vector3f(1, bad, 1)}), std::invalid_argument);
    }
    EXPECT_THROW(body.setMotion(static_cast<RigidBody::Motion>(99)), std::invalid_argument);
    EXPECT_THROW(body.setFriction(-1), std::invalid_argument);
    EXPECT_THROW(body.setRestitution(2), std::invalid_argument);
    EXPECT_THROW(body.setLinearDamping(nan), std::invalid_argument);
    EXPECT_THROW(body.setAngularDamping(2), std::invalid_argument);
    EXPECT_THROW(body.setGravityFactor(inf), std::invalid_argument);
    EXPECT_EQ(body.mass(), 2);
    EXPECT_EQ(body.motion(), RigidBody::Motion::Kinematic);
    RigidBody::Capsule capsule(1, 2);
    EXPECT_THROW(capsule.setDimensions(3, -1), std::invalid_argument);
    EXPECT_EQ(capsule.halfHeight(), 1);
    EXPECT_EQ(capsule.radius(), 2);
    body.setShape(capsule);
    Transform offset;
    offset.setTranslation({1, 2, 3});
    body.setOffset(offset);
    offset.reset();
    EXPECT_EQ(body.offset().translation(), Eigen::Vector3f(1, 2, 3));
    auto copy = body;
    copy.setOffset(offset);
    EXPECT_EQ(body.offset().translation(), Eigen::Vector3f(1, 2, 3));
    copy.setShape(RigidBody::Sphere{3});
    EXPECT_EQ(std::get<RigidBody::Capsule>(body.shape()).radius(), 2);
}

TEST(WorldComponents, MetadataAndResourceDescriptionsHaveIndependentValueSemantics) {
    Inherent metadata("object");
    metadata.setName("renamed");
    metadata.disable();
    metadata.hide();
    auto copy = metadata;
    copy.setName("copy");
    copy.enable();
    copy.show();
    EXPECT_EQ(metadata.name(), "renamed");
    EXPECT_FALSE(metadata.isEnabled());
    EXPECT_FALSE(metadata.isVisible());
    Model model("models/object.vemodel");
    auto other = model;
    other.setSource("models/other.vemodel");
    EXPECT_EQ(model.source(), "models/object.vemodel");
    EXPECT_EQ(other.source(), "models/other.vemodel");
}

TEST(WorldComponents, OwnedDescriptionsSupportCopyAndMoveAssignment) {
    const std::string name(128, 'x');
    Inherent original(name, false, false);
    Inherent copied;
    copied = original;
    Inherent moved(std::move(copied));
    Inherent assigned;
    assigned = std::move(moved);
    EXPECT_EQ(assigned.name(), name);
    EXPECT_FALSE(assigned.isEnabled());
    EXPECT_FALSE(assigned.isVisible());
    assigned.setName("changed");
    EXPECT_EQ(original.name(), name);
    copied = Inherent("reused");
    moved.setName("reused");
    EXPECT_EQ(copied.name(), "reused");
    EXPECT_EQ(moved.name(), "reused");

    const std::filesystem::path path = "models" / std::filesystem::path(name + ".vemodel");
    Model model(path);
    Model modelCopy;
    modelCopy = model;
    Model modelMoved(std::move(modelCopy));
    Model modelAssigned;
    modelAssigned = std::move(modelMoved);
    EXPECT_EQ(modelAssigned.source(), path);
    modelAssigned.setSource("models/changed.vemodel");
    EXPECT_EQ(model.source(), path);

    AudioSource source("sounds/test.wav");
    source.setGain(0.5f);
    source.play();
    AudioSource sourceCopy;
    sourceCopy = source;
    AudioSource sourceMoved(std::move(sourceCopy));
    AudioSource sourceAssigned;
    sourceAssigned = std::move(sourceMoved);
    EXPECT_EQ(sourceAssigned.source(), "sounds/test.wav");
    EXPECT_EQ(sourceAssigned.gain(), 0.5f);
    EXPECT_EQ(sourceAssigned.playback(), AudioSource::Playback::Playing);
    sourceAssigned.stop();
    EXPECT_EQ(source.playback(), AudioSource::Playback::Playing);
}

TEST(WorldComponents, AudioDescriptionsExpressPlaybackWithoutOwningBackendObjects) {
    AudioSource source("sounds/test.wav");
    EXPECT_EQ(source.playback(), AudioSource::Playback::Stopped);
    source.pause();
    EXPECT_EQ(source.playback(), AudioSource::Playback::Stopped);
    source.setGain(0.5f);
    source.setPitch(2);
    source.setLooping(true);
    source.setSpatial(false);
    source.play();
    source.pause();
    EXPECT_EQ(source.playback(), AudioSource::Playback::Paused);
    auto copy = source;
    copy.setSource("sounds/other.wav");
    copy.play();
    source.stop();
    EXPECT_EQ(source.source(), "sounds/test.wav");
    EXPECT_EQ(source.playback(), AudioSource::Playback::Stopped);
    EXPECT_EQ(copy.playback(), AudioSource::Playback::Playing);
    EXPECT_TRUE(source.isLooping());
    EXPECT_FALSE(source.isSpatial());
    AudioListener listener;
    listener.setGain(0);
    Transform offset;
    offset.setTranslation({0, 1, 0});
    listener.setOffset(offset);
    EXPECT_EQ(listener.offset().translation(), Eigen::Vector3f(0, 1, 0));
    for (const float bad : {-1.0f, nan, inf}) {
        EXPECT_THROW(source.setGain(bad), std::invalid_argument);
        EXPECT_THROW(source.setPitch(bad), std::invalid_argument);
        EXPECT_THROW(listener.setGain(bad), std::invalid_argument);
    }
    EXPECT_THROW(source.setPitch(0), std::invalid_argument);
    EXPECT_EQ(source.gain(), 0.5f);
    EXPECT_EQ(source.pitch(), 2);
}

class ThrowingScene final: public Scene {
public:
    entt::entity player() const noexcept override {
        return entt::null;
    }

protected:
    void onUpdate(Clock::duration) override {
        throw std::runtime_error("Game update failed");
    }
};

TEST(WorldComponents, SceneUpdateReportsErrorsToTheHost) {
    ThrowingScene scene;
    scene.setName("game");
    scene.setDescription("description");
    EXPECT_EQ(scene.name(), "game");
    EXPECT_EQ(scene.description(), "description");
    EXPECT_THROW(scene.update({}), std::runtime_error);
}
TEST(WorldComponents, SceneOwnsSharedTextureDescriptionsAndComponentsKeepLogicalReferences) {
    ThrowingScene scene;
    scene.addTexture("shared",std::make_unique<FileTexture>("textures/panel.vetexture"));
    EXPECT_EQ(scene.texture("shared").source(),Texture::Source::File);
    EXPECT_THROW(scene.addTexture("shared",std::make_unique<FileTexture>("other")),std::invalid_argument);
    EXPECT_THROW(scene.addTexture("empty",nullptr),std::invalid_argument);
    EXPECT_THROW(scene.texture("missing"),std::out_of_range);
    Model model("panel.vemodel");
    model.setTextureOverride("panel.vematerial","shared");
    auto copy=model;
    model.setTextureOverride("panel.vematerial","other");
    EXPECT_EQ(copy.textureOverrides().at("panel.vematerial"),"shared");
    DrawingTexture drawing({64,32},"provider");
    drawing.setDependencies({"shared"});
    EXPECT_EQ(drawing.dependencies().front(),"shared");
    EXPECT_THROW(drawing.setSize({0,32}),std::invalid_argument);
}

}
