#include <Volcano/Graphics/GeometryPass.h>

#include <Volcano/Graphics/Frame.h>
#include <Volcano/Graphics/Material.h>
#include <Volcano/Graphics/Mesh.h>

VOLCANO_GRAPHICS_BEGIN

namespace {

constexpr const char* GeometryVertexSource = R"GLSL(
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexcoord;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

out vec3 vPosition;
out vec3 vNormal;

void main() {
    vec4 position = uModel * vec4(aPosition, 1.0);
    vPosition = position.xyz;
    vNormal = normalize(transpose(inverse(mat3(uModel))) * aNormal);
    gl_Position = uProjection * uView * position;
}
)GLSL";

constexpr const char* GeometryFragmentSource = R"GLSL(
#version 330 core

layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec4 gNormal;
layout(location = 2) out vec4 gAlbedoSpecular;

uniform vec4 uAlbedo;
uniform float uSpecular;

in vec3 vPosition;
in vec3 vNormal;

void main() {
    gPosition = vec4(vPosition, 1.0);
    gNormal = vec4(normalize(vNormal), 1.0);
    gAlbedoSpecular = vec4(uAlbedo.rgb, uSpecular);
}
)GLSL";

}

GeometryPass::GeometryPass(GBuffer& gbuffer)
    : gbuffer_(gbuffer) {
    program_.attachSource(GL_VERTEX_SHADER, GeometryVertexSource);
    program_.attachSource(GL_FRAGMENT_SHADER, GeometryFragmentSource);
    program_.link();
}

void GeometryPass::resize(int, int) noexcept {
}

void GeometryPass::execute(const Frame& frame) {
    if (gbuffer_.width() <= 0 || gbuffer_.height() <= 0) {
        return;
    }

    gbuffer_.bindForGeometry();
    glViewport(0, 0, gbuffer_.width(), gbuffer_.height());
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    const auto& clear_color = frame.get(Frame::Vector4f::ClearColor);
    glClearColor(clear_color.x() / 255.0f, clear_color.y() / 255.0f, clear_color.z() / 255.0f, clear_color.w() / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program_.use();
    program_.uniformMatrix4fv("uProjection", frame.get(Frame::Matrix4f::Projection).data());
    program_.uniformMatrix4fv("uView", frame.get(Frame::Affine3f::View).matrix().data());

    for (const auto& item : frame.items()) {
        if (item.mesh == nullptr || item.mesh->empty()) {
            continue;
        }

        const auto model = item.transform.toAffine();
        program_.uniformMatrix4fv("uModel", model.matrix().data());

        if (item.material != nullptr) {
            const auto& albedo = item.material->albedo();
            program_.uniform4f("uAlbedo", albedo.x(), albedo.y(), albedo.z(), albedo.w());
            program_.uniform("uSpecular", item.material->specular());
        } else {
            program_.uniform4f("uAlbedo", 1.0f, 1.0f, 1.0f, 1.0f);
            program_.uniform("uSpecular", 0.5f);
        }

        item.mesh->vertexArray().bind();
        glDrawElements(GL_TRIANGLES, item.mesh->count(), GL_UNSIGNED_INT, nullptr);
    }

    gl::VertexArray::unbind();
    gl::Program::unuse();
}

VOLCANO_GRAPHICS_END
