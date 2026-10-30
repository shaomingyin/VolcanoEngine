#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
#include <limits>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

#include <GL/gl.hpp>
#include <gtest/gtest.h>

#include <Volcano/Math.h>
#include <Volcano/Graphics/FrameBuffer.h>
#include <Volcano/Graphics/GBuffer.h>
#include <Volcano/Graphics/Mesh.h>
#include <Volcano/Graphics/View.h>

namespace {

using namespace Volcano::Graphics;

class GraphicsResources : public ::testing::Test {
protected:
    void SetUp() override {
        saved_ = gl3wProcs;
        gl3wProcs = &procs_;
        next_ = 1;
        limit_queries_ = 0;
        renderbuffers_ = 0;
        framebuffers_ = 0;
        framebuffer_status_ = GL_FRAMEBUFFER_COMPLETE;

        auto generate = [](GLsizei count, GLuint* ids) {
            for (GLsizei i = 0; i < count; ++i) {
                ids[i] = next_++;
            }
        };
        auto release = [](GLsizei, const GLuint*) {
        };
        auto bind = [](GLenum, GLuint) {
        };
        procs_.gl.GenTextures = generate;
        procs_.gl.GenBuffers = generate;
        procs_.gl.GenVertexArrays = generate;
        procs_.gl.GenRenderbuffers = [](GLsizei count, GLuint* ids) {
            renderbuffers_ += count;
            for (GLsizei i = 0; i < count; ++i) {
                ids[i] = next_++;
            }
        };
        procs_.gl.GenFramebuffers = [](GLsizei count, GLuint* ids) {
            framebuffers_ += count;
            for (GLsizei i = 0; i < count; ++i) {
                ids[i] = next_++;
            }
        };
        procs_.gl.DeleteTextures = release;
        procs_.gl.DeleteBuffers = release;
        procs_.gl.DeleteVertexArrays = release;
        procs_.gl.DeleteRenderbuffers = release;
        procs_.gl.DeleteFramebuffers = release;
        procs_.gl.BindTexture = bind;
        procs_.gl.BindBuffer = bind;
        procs_.gl.BindRenderbuffer = bind;
        procs_.gl.BindFramebuffer = bind;
        procs_.gl.BindVertexArray = [](GLuint) {
        };
        procs_.gl.EnableVertexAttribArray = [](GLuint) {
        };
        procs_.gl.VertexAttribPointer = [](GLuint, GLint, GLenum, GLboolean, GLsizei, const void*) {
        };
        procs_.gl.BufferData = [](GLenum, GLsizeiptr, const void*, GLenum) {
        };
        procs_.gl.GetIntegerv = [](GLenum name, GLint* value) {
            ++limit_queries_;
            if (name == GL_MAX_TEXTURE_SIZE) {
                *value = 128;
            } else if (name == GL_MAX_RENDERBUFFER_SIZE) {
                *value = 64;
            } else {
                ADD_FAILURE() << "Unexpected OpenGL limit query";
                *value = 0;
            }
        };
        procs_.gl.TexParameteri = [](GLenum, GLenum, GLint) {
        };
        procs_.gl.TexImage2D = [](GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const void*) {
        };
        procs_.gl.RenderbufferStorage = [](GLenum, GLenum, GLsizei, GLsizei) {
        };
        procs_.gl.FramebufferTexture2D = [](GLenum, GLenum, GLenum, GLuint, GLint) {
        };
        procs_.gl.FramebufferRenderbuffer = [](GLenum, GLenum, GLenum, GLuint) {
        };
        procs_.gl.CheckFramebufferStatus = [](GLenum) -> GLenum {
            return framebuffer_status_;
        };
        procs_.gl.DrawBuffers = [](GLsizei, const GLenum*) {
        };
    }

    void TearDown() override {
        gl3wProcs = saved_;
    }

protected:
    GL3WProcs procs_{};
    GL3WProcs* saved_ = nullptr;
    static inline GLuint next_ = 1;
    static inline int limit_queries_ = 0;
    static inline int renderbuffers_ = 0;
    static inline int framebuffers_ = 0;
    static inline GLenum framebuffer_status_ = GL_FRAMEBUFFER_COMPLETE;
};

TEST_F(GraphicsResources, SampledTextureNeedsNeitherFramebufferNorRenderbufferCapacity) {
    Texture texture(128, 128);
    EXPECT_EQ(texture.size(), (Volcano::Size{128, 128}));
    EXPECT_EQ(renderbuffers_, 0);
    EXPECT_EQ(framebuffers_, 0);
    const auto id = texture.texture().id();
    const auto queries = limit_queries_;
    texture.resize(128, 128);
    EXPECT_EQ(limit_queries_, queries);
    EXPECT_THROW(texture.resize(129, 128), std::invalid_argument);
    EXPECT_EQ(texture.texture().id(), id);
    EXPECT_EQ(texture.size(), (Volcano::Size{128, 128}));
}

TEST_F(GraphicsResources, TextureMovesTransferOwnershipAndReleaseEachResourceOnce) {
    static std::vector<GLuint> deleted;
    deleted.clear();
    procs_.gl.DeleteTextures = [](GLsizei count, const GLuint* ids) {
        deleted.insert(deleted.end(), ids, ids + count);
    };

    GLuint transferred = 0;
    GLuint replaced = 0;
    {
        Texture source(4, 8);
        transferred = source.texture().id();
        Texture moved(std::move(source));
        EXPECT_EQ(source.texture().id(), 0u);
        EXPECT_EQ(moved.texture().id(), transferred);
        EXPECT_EQ(moved.size(), (Volcano::Size{4, 8}));

        Texture destination(2, 2);
        replaced = destination.texture().id();
        destination = std::move(moved);
        EXPECT_EQ(moved.texture().id(), 0u);
        EXPECT_EQ(destination.texture().id(), transferred);
        EXPECT_EQ(destination.size(), (Volcano::Size{4, 8}));
        EXPECT_EQ(deleted, (std::vector<GLuint>{replaced}));
    }
    EXPECT_EQ(deleted, (std::vector<GLuint>{replaced, transferred}));
}

TEST_F(GraphicsResources, UploadPremultipliedPreservesBytesAndStraightUploadConvertsOnce) {
    static std::array<std::uint8_t, 4> uploaded;
    procs_.gl.PixelStorei = [](GLenum, GLint) {};
    procs_.gl.TexSubImage2D = [](GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const void* bytes) {
        std::copy_n(static_cast<const std::uint8_t*>(bytes), 4, uploaded.begin());
    };
    Texture texture(1, 1);
    const std::array<std::uint8_t, 4> pixels{128, 64, 0, 128};
    texture.uploadPremultiplied(pixels);
    EXPECT_EQ(uploaded, pixels);
    texture.upload(pixels);
    EXPECT_EQ(uploaded, (std::array<std::uint8_t, 4>{64, 32, 0, 128}));
    EXPECT_THROW(texture.uploadPremultiplied({}), std::invalid_argument);
}

TEST_F(GraphicsResources, ViewPreservesAffineShearAndRejectsInvalidMatrices) {
    Mesh mesh({}, {});
    View view;
    Eigen::Affine3f transform = Eigen::Affine3f::Identity();
    transform.linear()(0, 1) = 0.5f;
    view.begin();
    view.add(mesh, {Volcano::Colorf::White, nullptr, true}, transform);
    view.end();
    EXPECT_EQ(view.opaqueItems()[0].model.matrix(), transform.matrix());
    EXPECT_EQ(view.opaqueItems()[0].normal, transform.linear().inverse().transpose());
    for (const auto value : {2.0f, std::numeric_limits<float>::quiet_NaN()}) {
        view.begin();
        transform.matrix()(3, 0) = value;
        view.add(mesh, {}, transform);
        EXPECT_THROW(view.end(), std::invalid_argument);
        EXPECT_FALSE(view.ready());
    }
}

TEST_F(GraphicsResources, FrameBufferResizePreservesItsColorObjectAndRollsBackFailure) {
    FrameBuffer target(8, 8);
    const auto* color = &target.texture();
    const auto id = color->texture().id();
    const auto queries = limit_queries_;
    target.resize(8, 8);
    EXPECT_EQ(limit_queries_, queries);
    EXPECT_TRUE(target.contains(*color));
    Texture other(8, 8);
    EXPECT_FALSE(target.contains(other));
    EXPECT_THROW(target.resize(0, 8), std::invalid_argument);
    EXPECT_THROW(target.resize(65, 8), std::invalid_argument);
    framebuffer_status_ = GL_FRAMEBUFFER_UNSUPPORTED;
    EXPECT_THROW(target.resize(16, 16), std::runtime_error);
    EXPECT_EQ(target.texture().texture().id(), id);
    EXPECT_EQ(target.size(), (Volcano::Size{8, 8}));
    EXPECT_EQ(&target.texture(), color);
    framebuffer_status_ = GL_FRAMEBUFFER_COMPLETE;
    target.resize(16, 16);
    EXPECT_NE(target.texture().texture().id(), id);
    EXPECT_EQ(target.size(), (Volcano::Size{16, 16}));
    EXPECT_EQ(&target.texture(), color);
}

TEST_F(GraphicsResources, GBufferReusesMatchingStorageWithoutQueryingLimits) {
    GBuffer buffer;
    EXPECT_THROW(buffer.resize({0, 0}), std::invalid_argument);
    buffer.resize({8, 8});
    const auto queries = limit_queries_;
    const auto id = buffer.albedo().id();
    buffer.resize({8, 8});
    EXPECT_EQ(limit_queries_, queries);
    EXPECT_EQ(buffer.albedo().id(), id);
    EXPECT_THROW(buffer.resize({-1, 8}), std::invalid_argument);
    EXPECT_EQ(buffer.albedo().id(), id);
}

TEST_F(GraphicsResources, PreparedLightSnapshotsAndPrecomputesUploadValues) {
    static std::array<float, 3> radiance;
    static std::array<float, 3> direction;
    static std::array<float, 2> cone;
    procs_.gl.CreateProgram = []() -> GLuint {
        return next_++;
    };
    procs_.gl.DeleteProgram = [](GLuint) {
    };
    procs_.gl.Uniform1i = [](GLint, GLint) {
    };
    procs_.gl.Uniform1f = [](GLint, GLfloat) {
    };
    procs_.gl.Uniform3f = [](GLint location, GLfloat x, GLfloat y, GLfloat z) {
        if (location == 1) {
            radiance = {x, y, z};
        } else if (location == 2) {
            direction = {x, y, z};
        }
    };
    procs_.gl.Uniform2f = [](GLint, GLfloat x, GLfloat y) {
        cone = {x, y};
    };

    Light light;
    light.type = Light::Type::Spot;
    light.color = {1, 2, 3};
    light.strength = 2;
    light.direction = {0, -4, 0};
    const Light::Prepared prepared(light);
    light.color.setZero();
    light.direction.setZero();
    light.outerAngle = 60;

    gl::Program program;
    Light::Uniforms uniforms;
    uniforms.radiance = 1;
    uniforms.direction = 2;
    prepared.apply(program, uniforms);
    EXPECT_EQ(radiance, (std::array<float, 3>{2, 4, 6}));
    EXPECT_EQ(direction, (std::array<float, 3>{0, -1, 0}));
    EXPECT_FLOAT_EQ(cone[0], std::cos(Volcano::Eigen::radians(15.0f)));
    EXPECT_FLOAT_EQ(cone[1], std::cos(Volcano::Eigen::radians(30.0f)));
}

TEST_F(GraphicsResources, ViewBuildsMatricesAndSortsTransparentItemsStably) {
    Mesh mesh({}, {});
    View view;
    EXPECT_FALSE(view.ready());
#if defined(VOLCANO_DEBUG) && !defined(NDEBUG)
    EXPECT_DEATH(view.opaqueItems(), "[Aa]ssert");
    EXPECT_DEATH(view.transparentItems(), "[Aa]ssert");
    EXPECT_DEATH(view.preparedLights(), "[Aa]ssert");
#endif
    static_assert(std::is_const_v<std::remove_reference_t<decltype(view.items())>>);
    static_assert(std::is_const_v<std::remove_reference_t<decltype(view.lights())>>);

    Volcano::Transform transform;
    transform.setScale({2, 1, 1});
    view.begin();
    view.add(mesh, {Volcano::Colorf::White, nullptr, true}, transform);
    for (int index = 0; index < 3; ++index) {
        Volcano::Transform transparent;
        transparent.setTranslation(Eigen::Vector3f(0, 0, index == 1 ? 0.5f : -0.5f));
        view.add(mesh, {}, transparent, true);
    }
    view.end();
    EXPECT_TRUE(view.ready());
    ASSERT_EQ(view.opaqueItems().size(), 1);
    ASSERT_EQ(view.transparentItems().size(), 3);
    EXPECT_EQ(view.opaqueItems()[0].index, 0);
    EXPECT_FLOAT_EQ(view.opaqueItems()[0].model.linear()(0, 0), 2);
    EXPECT_FLOAT_EQ(view.opaqueItems()[0].normal(0, 0), 0.5f);
    EXPECT_EQ(view.transparentItems()[0].index, 1);
    EXPECT_EQ(view.transparentItems()[1].index, 3);
    EXPECT_EQ(view.transparentItems()[2].index, 2);
    EXPECT_FLOAT_EQ(view.items()[2].transform.translation().z(), 0.5f);

    const auto* storage = view.opaqueItems().data();
    auto copy = view;
    transform.setScale(Eigen::Vector3f::Constant(3));
    EXPECT_FLOAT_EQ(copy.items()[0].transform.linear()(0, 0), 2);
    view.begin();
    EXPECT_FALSE(view.ready());
    view.add(mesh, {Volcano::Colorf::White, nullptr, true}, transform);
    view.end();
    EXPECT_EQ(view.opaqueItems().data(), storage);
    EXPECT_TRUE(view.transparentItems().empty());
    EXPECT_FLOAT_EQ(view.opaqueItems()[0].model.linear()(0, 0), 3);
    EXPECT_NEAR(view.opaqueItems()[0].normal(0, 0), 1.0f / 3.0f, 1e-6f);
    EXPECT_FLOAT_EQ(copy.opaqueItems()[0].model.linear()(0, 0), 2);

    transform.setScale(Eigen::Vector3f::Zero());
    view.begin();
    view.add(mesh, {Volcano::Colorf::White, nullptr, true}, transform);
    EXPECT_THROW(view.end(), std::invalid_argument);
    EXPECT_FALSE(view.ready());
#if defined(VOLCANO_DEBUG) && !defined(NDEBUG)
    EXPECT_DEATH(view.opaqueItems(), "[Aa]ssert");
#endif
    view.begin();
    view.add(mesh, {}, transform);
    EXPECT_NO_THROW(view.end());

    Eigen::Matrix4f camera = Eigen::Matrix4f::Identity();
    camera(2, 2) = std::numeric_limits<float>::max();
    transform.reset();
    transform.setTranslation(Eigen::Vector3f(0, 0, std::numeric_limits<float>::max()));
    view.begin();
    view.setViewMatrix(camera);
    view.add(mesh, {}, transform, true);
    EXPECT_THROW(view.end(), std::invalid_argument);
    EXPECT_FALSE(view.ready());
}

TEST_F(GraphicsResources, ViewGroupsOpaqueStatesWithoutChangingSubmissionOrTransparentOrder) {
    Mesh first_mesh({}, {});
    Mesh second_mesh({}, {});
    const Mesh* lower_mesh = &first_mesh;
    const Mesh* higher_mesh = &second_mesh;
    if (std::less<const Mesh*>{}(higher_mesh, lower_mesh)) {
        std::swap(lower_mesh, higher_mesh);
    }
    Texture first_texture(1, 1);
    Texture second_texture(1, 1);
    const Texture* lower_texture = &first_texture;
    const Texture* higher_texture = &second_texture;
    if (std::less<const Texture*>{}(higher_texture, lower_texture)) {
        std::swap(lower_texture, higher_texture);
    }

    View view;
    view.begin();
    view.add(*higher_mesh, {Volcano::Colorf::Red, higher_texture});
    view.add(*higher_mesh, {Volcano::Colorf::White, lower_texture});
    view.add(*lower_mesh, {Volcano::Colorf::White, higher_texture});
    view.add(*lower_mesh, {Volcano::Colorf::Red, lower_texture});
    view.add(*lower_mesh, {Volcano::Colorf::Blue, lower_texture});
    view.add(*lower_mesh, {Volcano::Colorf::White, lower_texture, true});
    Volcano::Transform far;
    far.setTranslation(Eigen::Vector3f(0, 0, -1));
    Volcano::Transform near;
    near.setTranslation(Eigen::Vector3f(0, 0, 1));
    view.add(*lower_mesh, {Volcano::Colorf::White, higher_texture}, far, true);
    view.add(*lower_mesh, {Volcano::Colorf::White, lower_texture}, near, true);
    view.end();

    constexpr std::array<std::size_t, 6> expected{3, 4, 1, 5, 2, 0};
    ASSERT_EQ(view.opaqueItems().size(), expected.size());
    for (std::size_t index = 0; index < expected.size(); ++index) {
        EXPECT_EQ(view.opaqueItems()[index].index, expected[index]);
    }
    EXPECT_EQ(view.items()[0].material.texture, higher_texture);
    EXPECT_EQ(&view.items()[0].mesh.get(), higher_mesh);
    EXPECT_EQ(view.items()[3].material.color, Volcano::Colorf::Red);
    EXPECT_EQ(view.items()[4].material.color, Volcano::Colorf::Blue);
    ASSERT_EQ(view.transparentItems().size(), 2);
    EXPECT_EQ(view.transparentItems()[0].index, 6);
    EXPECT_EQ(view.transparentItems()[1].index, 7);
    EXPECT_FLOAT_EQ(view.transparentItems()[0].model.translation().z(), -1);
}

TEST_F(GraphicsResources, ViewEnforcesSubmissionOrderAndRetainsSettingsAcrossBegin) {
    Mesh mesh({}, {});
    View view;
#if defined(VOLCANO_DEBUG) && !defined(NDEBUG)
    EXPECT_DEATH(view.add(mesh), "[Aa]ssert");
    EXPECT_DEATH(view.add(Light{}), "[Aa]ssert");
    EXPECT_DEATH(view.end(), "[Aa]ssert");
#endif
    view.setAmbientLight(Eigen::Vector3f::Constant(0.2f));
    view.setClearColor(Volcano::Colorf::Blue);
    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();
    projection(0, 0) = 2;
    view.setProjection(projection);
    view.begin();
    view.add(mesh);
    view.add(Light{});
    view.end();
    EXPECT_TRUE(view.ready());
#if defined(VOLCANO_DEBUG) && !defined(NDEBUG)
    EXPECT_DEATH(view.add(mesh), "[Aa]ssert");
    EXPECT_DEATH(view.add(Light{}), "[Aa]ssert");
    EXPECT_DEATH(view.end(), "[Aa]ssert");
#endif
    EXPECT_TRUE(view.ready());
    projection.setZero();
    EXPECT_FLOAT_EQ(view.projection()(0, 0), 2);

    view.begin();
    EXPECT_TRUE(view.items().empty());
    EXPECT_TRUE(view.lights().empty());
#if defined(VOLCANO_DEBUG) && !defined(NDEBUG)
    EXPECT_DEATH(view.preparedLights(), "[Aa]ssert");
#endif
    view.end();
    EXPECT_TRUE(view.ready());
    EXPECT_TRUE(view.opaqueItems().empty());
    EXPECT_TRUE(view.preparedLights().empty());
    EXPECT_TRUE(view.ambientLight().isApprox(Eigen::Vector3f::Constant(0.2f)));
    EXPECT_EQ(view.clearColor(), Volcano::Colorf::Blue);
    EXPECT_FLOAT_EQ(view.projection()(0, 0), 2);

    view.setViewMatrix(Eigen::Matrix4f::Identity());
    EXPECT_FALSE(view.ready());
#if defined(VOLCANO_DEBUG) && !defined(NDEBUG)
    EXPECT_DEATH(view.end(), "[Aa]ssert");
#endif
    view.begin();
    view.add(mesh);
    view.begin();
    view.end();
    EXPECT_TRUE(view.items().empty());

    view.begin();
    view.setAmbientLight({-1, 0, 0});
    EXPECT_THROW(view.end(), std::invalid_argument);
    view.setAmbientLight(Eigen::Vector3f::Zero());
    EXPECT_NO_THROW(view.end());
    EXPECT_TRUE(view.ready());
}

TEST_F(GraphicsResources, ViewRebuildsLightsAndRecoversAfterFailedEnd) {
    View view;
    view.begin();
    view.add(Light{});
    view.add(Light{});
    view.end();
    ASSERT_EQ(view.preparedLights().size(), 2);
    const auto* storage = view.preparedLights().data();
    view.begin();
    view.add(Light{});
    view.end();
    EXPECT_EQ(view.preparedLights().data(), storage);
    EXPECT_EQ(view.preparedLights().size(), 1);
    Light invalid;
    invalid.strength = -1;
    view.begin();
    view.add(invalid);
    EXPECT_THROW(view.end(), std::invalid_argument);
    EXPECT_FALSE(view.ready());
#if defined(VOLCANO_DEBUG) && !defined(NDEBUG)
    EXPECT_DEATH(view.preparedLights(), "[Aa]ssert");
#endif
    view.begin();
    view.end();
    EXPECT_TRUE(view.preparedLights().empty());
}

} // namespace
