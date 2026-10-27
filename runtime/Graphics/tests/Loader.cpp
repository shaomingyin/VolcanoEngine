#include <array>
#include <cstring>
#include <stdexcept>

#include <gtest/gtest.h>

#include <Volcano/Graphics/Renderer.h>

namespace {

class GraphicsLoader : public ::testing::Test {
protected:
    void SetUp() override {
        saved_ = gl3wProcs;
        gl3wProcs = &previous_;
    }

    void TearDown() override {
        gl3wProcs = saved_;
    }

protected:
    GL3WProcs previous_{};
    GL3WProcs* saved_ = nullptr;
};

TEST_F(GraphicsLoader, ProgramQueriesAndValidatesFragmentOutputLocations) {
    previous_.gl.CreateProgram = []() -> GLuint {
        return 42;
    };
    previous_.gl.DeleteProgram = [](GLuint) {
    };
    previous_.gl.GetFragDataLocation = [](GLuint program, const GLchar* name) -> GLint {
        EXPECT_EQ(program, 42u);
        return std::strcmp(name, "color") == 0 ? 2 : -1;
    };

    gl::Program program;
    EXPECT_EQ(program.fragmentLocation("color"), 2);
    EXPECT_EQ(program.fragmentLocation("color", 2), 2);
    EXPECT_THROW(program.fragmentLocation("color", 0), std::runtime_error);
    EXPECT_EQ(program.fragmentLocation("missing"), -1);
    EXPECT_THROW(program.fragmentLocation("missing", 0), std::runtime_error);
}

TEST_F(GraphicsLoader, ProgramUploadsUniformsByLocationAndNameWithoutBinding) {
    static int uploads = 0;
    static int queries = 0;
    static const GLfloat* matrix_data = nullptr;
    uploads = 0;
    queries = 0;
    const std::array<GLfloat, 32> matrices{};
    matrix_data = matrices.data();

    previous_.gl.CreateProgram = []() -> GLuint {
        return 42;
    };
    previous_.gl.DeleteProgram = [](GLuint) {
    };
    previous_.gl.UseProgram = [](GLuint) {
        ADD_FAILURE() << "Uniform uploads must not change the bound program";
    };
    previous_.gl.GetUniformLocation = [](GLuint program, const GLchar* name) -> GLint {
        ++queries;
        EXPECT_EQ(program, 42u);
        EXPECT_STREQ(name, "value");
        return 3;
    };
    previous_.gl.Uniform1i = [](GLint location, GLint value) {
        ++uploads;
        EXPECT_EQ(location, 3);
        EXPECT_EQ(value, 7);
    };
    previous_.gl.Uniform1ui = [](GLint location, GLuint value) {
        ++uploads;
        EXPECT_EQ(location, 3);
        EXPECT_EQ(value, 7u);
    };
    previous_.gl.Uniform1f = [](GLint location, GLfloat value) {
        ++uploads;
        EXPECT_EQ(location, 3);
        EXPECT_EQ(value, 1.0f);
    };
    previous_.gl.Uniform2f = [](GLint location, GLfloat x, GLfloat y) {
        ++uploads;
        EXPECT_EQ(location, 3);
        EXPECT_EQ(x, 1.0f);
        EXPECT_EQ(y, 2.0f);
    };
    previous_.gl.Uniform3f = [](GLint location, GLfloat x, GLfloat y, GLfloat z) {
        ++uploads;
        EXPECT_EQ(location, 3);
        EXPECT_EQ(x, 1.0f);
        EXPECT_EQ(y, 2.0f);
        EXPECT_EQ(z, 3.0f);
    };
    previous_.gl.Uniform4f = [](GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
        ++uploads;
        EXPECT_EQ(location, 3);
        EXPECT_EQ(x, 1.0f);
        EXPECT_EQ(y, 2.0f);
        EXPECT_EQ(z, 3.0f);
        EXPECT_EQ(w, 4.0f);
    };
    previous_.gl.UniformMatrix3fv = [](GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) {
        ++uploads;
        EXPECT_EQ(location, 3);
        EXPECT_EQ(count, 2);
        EXPECT_EQ(transpose, GL_TRUE);
        EXPECT_EQ(value, matrix_data);
    };
    previous_.gl.UniformMatrix4fv = [](GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) {
        ++uploads;
        EXPECT_EQ(location, 3);
        EXPECT_EQ(count, 2);
        EXPECT_EQ(transpose, GL_TRUE);
        EXPECT_EQ(value, matrix_data);
    };

    gl::Program program;
    program.uniform(3, GLint{7});
    program.uniform(3, GLuint{7});
    program.uniform(3, GLfloat{1.0f});
    program.uniform2f(3, 1.0f, 2.0f);
    program.uniform3f(3, 1.0f, 2.0f, 3.0f);
    program.uniform4f(3, 1.0f, 2.0f, 3.0f, 4.0f);
    program.uniformMatrix3fv(3, matrix_data, GL_TRUE, 2);
    program.uniformMatrix4fv(3, matrix_data, GL_TRUE, 2);
    EXPECT_EQ(uploads, 8);
    EXPECT_EQ(queries, 0);

    program.uniform("value", GLint{7});
    program.uniform("value", GLuint{7});
    program.uniform("value", GLfloat{1.0f});
    program.uniform2f("value", 1.0f, 2.0f);
    program.uniform3f("value", 1.0f, 2.0f, 3.0f);
    program.uniform4f("value", 1.0f, 2.0f, 3.0f, 4.0f);
    program.uniformMatrix3fv("value", matrix_data, GL_TRUE, 2);
    program.uniformMatrix4fv("value", matrix_data, GL_TRUE, 2);
    EXPECT_EQ(uploads, 16);
    EXPECT_EQ(queries, 8);

    previous_.gl.Uniform1i = [](GLint location, GLint value) {
        ++uploads;
        EXPECT_EQ(location, -1);
        EXPECT_EQ(value, 7);
    };
    program.uniform(-1, GLint{7});
    EXPECT_EQ(uploads, 17);
    EXPECT_EQ(queries, 8);
}

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
