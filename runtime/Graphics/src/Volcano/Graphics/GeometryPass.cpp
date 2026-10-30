#include <Volcano/ScopeGuard.h>
#include <Volcano/Graphics/GBuffer.h>
#include <Volcano/Graphics/GeometryPass.h>
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
layout(location = ALBEDO_ATTACHMENT) out vec4 albedo;
layout(location = NORMAL_ATTACHMENT) out vec4 normal;
layout(location = POSITION_ATTACHMENT) out vec4 position;
void main() {
    albedo = vec4(uColor.rgb * uColor.a, uColor.a);
    if (uTextured) {
        albedo *= texture(uTexture, vTexcoord);
    }
    if (albedo.a == 0.0) {
        discard;
    }

    normal = vec4(vNormal, uLit ? 1.0 : 0.0);
    position = vec4(vPosition, 1.0);
}
)GLSL";

}

GeometryPass::GeometryPass() {
    attachShader(GL_VERTEX_SHADER, "330 core", VertexSource, Mesh::shaderDefinitions());
    attachShader(GL_FRAGMENT_SHADER, "330 core", FragmentSource, GBuffer::shaderDefinitions());
    program_.link();
    uniforms_.resolve(program_);
    program_.fragmentLocation("albedo", GBuffer::AlbedoAttachment);
    program_.fragmentLocation("normal", GBuffer::NormalAttachment);
    program_.fragmentLocation("position", GBuffer::PositionAttachment);
}

void GeometryPass::render(const View& view) {
    const auto& items = view.opaqueItems();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    program_.use();
    auto unbind = scopeGuard([]() noexcept {
        gl::Program::unuse();
    });
    program_.uniformMatrix4fv(uniforms_.projection, view.projection().data());
    program_.uniformMatrix4fv(uniforms_.view, view.viewMatrix().data());

    for (const auto& prepared : items) {
        const auto& item = view.items()[prepared.index];
        if (item.mesh.get().empty()) {
            continue;
        }

        program_.uniformMatrix4fv(uniforms_.model, prepared.model.data());
        program_.uniformMatrix3fv(uniforms_.normalMatrix, prepared.normal.data());
        item.material.apply(program_, uniforms_.material);
        item.mesh.get().draw();
    }
}

VOLCANO_GRAPHICS_END
