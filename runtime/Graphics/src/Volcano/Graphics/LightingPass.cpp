#include <stdexcept>
#include <string>

#include <Volcano/Graphics/GBuffer.h>
#include <Volcano/Graphics/LightingPass.h>
#include <Volcano/Graphics/View.h>

VOLCANO_GRAPHICS_BEGIN

namespace {

constexpr const char* VertexSource = R"GLSL(
void main() {
    vec2 point = vec2((gl_VertexID << 1) & 2, gl_VertexID & 2);
    gl_Position = vec4(point * 2.0 - 1.0, 0.0, 1.0);
}
)GLSL";

constexpr const char* FragmentSource = R"GLSL(
uniform sampler2D uAlbedo;
uniform sampler2D uNormal;
uniform sampler2D uPosition;
uniform sampler2D uDepth;
layout(location = COLOR_OUTPUT) out vec4 color;
vec3 evaluateLighting(vec3 position, vec3 normal);
void main() {
    ivec2 pixel = ivec2(gl_FragCoord.xy);
    float depth = texelFetch(uDepth, pixel, 0).r;
    if (depth == 1.0) {
        discard;
    }

    color = texelFetch(uAlbedo, pixel, 0);
    vec4 normal = texelFetch(uNormal, pixel, 0);
    if (normal.w > 0.5) {
        color.rgb *= evaluateLighting(texelFetch(uPosition, pixel, 0).xyz, normal.xyz);
    }

    // Carry opaque depth to the output without requiring matching depth formats for a blit.
    gl_FragDepth = depth;
}
)GLSL";

}

LightingPass::LightingPass(std::size_t maxLights) {
    attachShader(GL_VERTEX_SHADER, "330 core", VertexSource);
    attachShader(GL_FRAGMENT_SHADER, "330 core", FragmentSource,
        "#define COLOR_OUTPUT " + std::to_string(ColorOutput) + "\n" + Light::shaderSource(maxLights));
    program_.link();
    program_.fragmentLocation("color", ColorOutput);
    uniforms_.resolve(program_, maxLights);
    albedo_sampler_ = program_.uniformLocation("uAlbedo", GL_SAMPLER_2D);
    normal_sampler_ = program_.uniformLocation("uNormal", GL_SAMPLER_2D);
    position_sampler_ = program_.uniformLocation("uPosition", GL_SAMPLER_2D);
    depth_sampler_ = program_.uniformLocation("uDepth", GL_SAMPLER_2D);
}

void LightingPass::render(const View& view, const GBuffer& geometry) {
    const auto& lights = view.preparedLights();
    if (geometry.size().empty()) {
        throw std::invalid_argument("LightingPass requires an allocated GBuffer");
    }

    GLint framebuffer = 0;
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &framebuffer);
    if (static_cast<GLuint>(framebuffer) == geometry.framebuffer().id()) {
        throw std::invalid_argument("LightingPass output must differ from its GBuffer input");
    }

    GLint viewport[4]{};
    glGetIntegerv(GL_VIEWPORT, viewport);
    if (viewport[0] != 0 || viewport[1] != 0 ||
        viewport[2] != geometry.size().width || viewport[3] != geometry.size().height) {
        throw std::invalid_argument("LightingPass viewport must match its GBuffer dimensions");
    }

    if (lights.size() > uniforms_.lights.size()) {
        throw std::invalid_argument("View exceeds LightingPass light capacity");
    }
    program_.use();
    const auto& ambient = view.ambientLight();
    program_.uniform3f(uniforms_.ambient, ambient.x(), ambient.y(), ambient.z());
    program_.uniform(uniforms_.count, static_cast<GLint>(lights.size()));
    for (std::size_t i = 0; i < lights.size(); ++i) {
        lights[i].apply(program_, uniforms_.lights[i]);
    }

    program_.uniform(albedo_sampler_, static_cast<GLint>(AlbedoTexture));
    program_.uniform(normal_sampler_, static_cast<GLint>(NormalTexture));
    program_.uniform(position_sampler_, static_cast<GLint>(PositionTexture));
    program_.uniform(depth_sampler_, static_cast<GLint>(DepthTexture));

    geometry.albedo().bind(AlbedoTexture);
    geometry.normal().bind(NormalTexture);
    geometry.position().bind(PositionTexture);
    geometry.depth().bind(DepthTexture);
    for (GLuint unit : {AlbedoTexture, NormalTexture,
                       PositionTexture, DepthTexture}) {
        glBindSampler(unit, 0);
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    vertex_array_.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    gl::VertexArray::unbind();
    gl::Program::unuse();
}

VOLCANO_GRAPHICS_END
