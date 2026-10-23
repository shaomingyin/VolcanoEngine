#include <gtest/gtest.h>
#include <Volcano/Graphics/Renderer.h>
#include <cstring>

namespace {

class GraphicsLoader : public ::testing::Test {
protected:
    GL3WProcs previous_{};
    GL3WProcs* saved_ = nullptr;

    void SetUp() override {
        saved_ = gl3wProcs;
        gl3wProcs = &previous_;
    }

    void TearDown() override {
        gl3wProcs = saved_;
    }
};

TEST_F(GraphicsLoader, RejectsMissingResolverWithoutChangingDispatch) {
    EXPECT_THROW(Volcano::Graphics::Renderer(nullptr), std::invalid_argument);
    EXPECT_EQ(gl3wProcs, &previous_);
}

TEST_F(GraphicsLoader, RejectsMissingFunctionsWithoutChangingDispatch) {
    EXPECT_THROW(Volcano::Graphics::Renderer([](const char*) -> GL3WglProc {
        return nullptr;
    }), std::runtime_error);
    EXPECT_EQ(gl3wProcs, &previous_);
}

void APIENTRY version32(GLenum name, GLint* value) {
    *value = name == GL_MAJOR_VERSION ? 3 : 2;
}

TEST_F(GraphicsLoader, RejectsOldContextWithoutChangingDispatch) {
    EXPECT_THROW(Volcano::Graphics::Renderer([](const char* name) -> GL3WglProc {
        return std::strcmp(name, "glGetIntegerv") == 0 ? reinterpret_cast<GL3WglProc>(version32) : nullptr;
    }), std::runtime_error);
    EXPECT_EQ(gl3wProcs, &previous_);
}

}
