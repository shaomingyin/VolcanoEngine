#include <limits>
#include <stdexcept>
#include <string>
#include <string_view>

#include <Volcano/Graphics/Pass.h>
#include <Volcano/Graphics/Mesh.h>

VOLCANO_GRAPHICS_BEGIN

void Pass::ObjectUniforms::resolve(const gl::Program& program) {
    program.attributeLocation("aPosition", GL_FLOAT_VEC3, Mesh::PositionAttribute);
    program.attributeLocation("aNormal", GL_FLOAT_VEC3, Mesh::NormalAttribute);
    program.attributeLocation("aTexcoord", GL_FLOAT_VEC2, Mesh::TexcoordAttribute);

    projection = program.uniformLocation("uProjection", GL_FLOAT_MAT4);
    view = program.uniformLocation("uView", GL_FLOAT_MAT4);
    model = program.uniformLocation("uModel", GL_FLOAT_MAT4);
    normalMatrix = program.uniformLocation("uNormalMatrix", GL_FLOAT_MAT3);
    material.resolve(program);
}

void Pass::LightingUniforms::resolve(const gl::Program& program, std::size_t maxLights) {
    if (maxLights == 0 || maxLights > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
        throw std::invalid_argument("Light capacity must be a positive GLSL int");
    }
    ambient = program.uniformLocation("uAmbient", GL_FLOAT_VEC3);
    count = program.uniformLocation("uLightCount", GL_INT);
    lights.resize(maxLights);
    for (std::size_t i = 0; i < lights.size(); ++i) {
        lights[i].resolve(program, i);
    }
}

void Pass::attachShader(GLenum stage, std::string_view version, std::string_view source,
    std::string_view definitions) {
    std::string result = "#version " + std::string(version) + "\n";
    result += definitions;
    result += '\n';
    result += source;
    program_.attachSource(stage, result);
}

VOLCANO_GRAPHICS_END
