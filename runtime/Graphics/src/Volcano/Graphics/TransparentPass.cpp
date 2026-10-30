#include <stdexcept>
#include <string>

#include <Volcano/ScopeGuard.h>
#include <Volcano/Graphics/TransparentPass.h>
#include <Volcano/Graphics/Mesh.h>
#include <Volcano/Graphics/View.h>

VOLCANO_GRAPHICS_BEGIN

namespace {

constexpr const char* VertexSource = R"GLSL(
layout(location = POSITION_ATTRIBUTE) in vec3 aPosition;
layout(location = NORMAL_ATTRIBUTE) in vec3 aNormal;
layout(location = TEXCOORD_ATTRIBUTE) in vec2 aTexcoord;
uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;
uniform mat3 uNormalMatrix;
out vec2 vTexcoord;
out vec3 vPosition;
out vec3 vNormal;
void main() {
    vTexcoord = aTexcoord;
    vec4 worldPosition = uModel * vec4(aPosition, 1.0);
    vPosition = worldPosition.xyz;
    vNormal = uNormalMatrix * aNormal;
    gl_Position = uProjection * uView * worldPosition;
}
)GLSL";

constexpr const char* FragmentSource = R"GLSL(
uniform vec4 uColor;
uniform sampler2D uTexture;
uniform bool uTextured;
uniform bool uLit;
in vec2 vTexcoord;
in vec3 vPosition;
in vec3 vNormal;
layout(location = COLOR_OUTPUT) out vec4 color;
vec3 evaluateLighting(vec3 position, vec3 normal);
void main() {
    color = vec4(uColor.rgb * uColor.a, uColor.a);
    if (uTextured) {
        color *= texture(uTexture, vTexcoord);
    }
    if (color.a == 0.0) {
        discard;
    }
    if (uLit) {
        color.rgb *= evaluateLighting(vPosition, vNormal);
    }
}
)GLSL";

}

TransparentPass::TransparentPass(std::size_t maxLights) {
    attachShader(GL_VERTEX_SHADER, "330 core", VertexSource, Mesh::shaderDefinitions());
    attachShader(GL_FRAGMENT_SHADER, "330 core", FragmentSource,
        "#define COLOR_OUTPUT " + std::to_string(ColorOutput) + "\n" + Light::shaderSource(maxLights));
    program_.link();
    program_.fragmentLocation("color", ColorOutput);
    object_uniforms_.resolve(program_);
    lighting_uniforms_.resolve(program_, maxLights);
}

void TransparentPass::render(const View& view) {
    const auto& items = view.transparentItems();
    const auto& lights = view.preparedLights();
    if (lights.size() > lighting_uniforms_.lights.size()) {
        throw std::invalid_argument("View exceeds TransparentPass light capacity");
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    program_.use();
    auto unbind = scopeGuard([]() noexcept {
        gl::Program::unuse();
    });
    program_.uniformMatrix4fv(object_uniforms_.projection, view.projection().data());
    program_.uniformMatrix4fv(object_uniforms_.view, view.viewMatrix().data());
    const auto& ambient = view.ambientLight();
    program_.uniform3f(lighting_uniforms_.ambient, ambient.x(), ambient.y(), ambient.z());
    program_.uniform(lighting_uniforms_.count, static_cast<GLint>(lights.size()));
    for (std::size_t i = 0; i < lights.size(); ++i) {
        lights[i].apply(program_, lighting_uniforms_.lights[i]);
    }

    for (const auto& prepared : items) {
        const auto& item = view.items()[prepared.index];
        if (item.mesh.get().empty()) {
            continue;
        }

        program_.uniformMatrix4fv(object_uniforms_.model, prepared.model.data());
        program_.uniformMatrix3fv(object_uniforms_.normalMatrix, prepared.normal.data());
        item.material.apply(program_, object_uniforms_.material);
        item.mesh.get().draw();
    }
}

VOLCANO_GRAPHICS_END
