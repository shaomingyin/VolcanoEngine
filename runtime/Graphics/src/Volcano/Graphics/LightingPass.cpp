#include <Volcano/Graphics/LightingPass.h>

#include <algorithm>
#include <string>

#include <Volcano/Graphics/Frame.h>

VOLCANO_GRAPHICS_BEGIN

namespace {

constexpr const char* LightingVertexSource = R"GLSL(
#version 330 core

out vec2 vTexcoord;

void main() {
    const vec2 positions[3] = vec2[3](
        vec2(-1.0, -1.0),
        vec2( 3.0, -1.0),
        vec2(-1.0,  3.0)
    );

    vec2 position = positions[gl_VertexID];
    vTexcoord = position * 0.5 + 0.5;
    gl_Position = vec4(position, 0.0, 1.0);
}
)GLSL";

constexpr const char* LightingFragmentSource = R"GLSL(
#version 330 core

const int MaxLights = 64;

struct Light {
    int type;
    vec3 position;
    vec3 direction;
    vec3 color;
    float strength;
    float range;
    float innerAngle;
    float outerAngle;
};

uniform sampler2D uPosition;
uniform sampler2D uNormal;
uniform sampler2D uAlbedoSpecular;
uniform sampler2D uDepth;
uniform vec3 uCameraPosition;
uniform vec3 uAmbientLight;
uniform int uLightCount;
uniform Light uLights[MaxLights];

in vec2 vTexcoord;
out vec4 color;

void main() {
    vec3 position = texture(uPosition, vTexcoord).xyz;
    vec3 normal = texture(uNormal, vTexcoord).xyz;
    vec4 albedoSpecular = texture(uAlbedoSpecular, vTexcoord);

    if (length(normal) < 0.1) {
        color = vec4(0.0);
        return;
    }

    normal = normalize(normal);

    vec3 albedo = albedoSpecular.rgb;
    vec3 viewDirection = normalize(uCameraPosition - position);
    vec3 result = albedo * uAmbientLight;

    for (int i = 0; i < uLightCount; ++i) {
        Light light = uLights[i];
        vec3 lightDirection;
        float attenuation = 1.0;

        if (light.type == 0) {
            lightDirection = normalize(-light.direction);
        } else {
            vec3 difference = light.position - position;
            float distanceToLight = length(difference);
            lightDirection = normalize(difference);
            attenuation = max(0.0, 1.0 - distanceToLight / max(light.range, 0.001));

            if (light.type == 2) {
                float angle = dot(normalize(light.direction), -lightDirection);
                float inner = cos(radians(light.innerAngle));
                float outer = cos(radians(light.outerAngle));
                attenuation *= clamp((angle - outer) / max(inner - outer, 0.001), 0.0, 1.0);
            }
        }

        float diffuse = max(dot(normal, lightDirection), 0.0);
        vec3 halfDirection = normalize(lightDirection + viewDirection);
        float specular = pow(max(dot(normal, halfDirection), 0.0), 32.0) * albedoSpecular.a;
        vec3 radiance = light.color * light.strength * attenuation;
        result += (albedo * diffuse + vec3(specular)) * radiance;
    }

    color = vec4(result, 1.0);
}
)GLSL";

void setLightUniform(const gl::Program& program, int index, const Frame::Light& light) noexcept {
    const std::string prefix = "uLights[" + std::to_string(index) + "]";
    glUniform1i(glGetUniformLocation(program.id(), (prefix + ".type").c_str()), static_cast<GLint>(light.type));
    glUniform3fv(glGetUniformLocation(program.id(), (prefix + ".position").c_str()), 1, light.position.data());
    glUniform3fv(glGetUniformLocation(program.id(), (prefix + ".direction").c_str()), 1, light.direction.data());
    glUniform3fv(glGetUniformLocation(program.id(), (prefix + ".color").c_str()), 1, light.color.data());
    glUniform1f(glGetUniformLocation(program.id(), (prefix + ".strength").c_str()), light.strength);
    glUniform1f(glGetUniformLocation(program.id(), (prefix + ".range").c_str()), light.range);
    glUniform1f(glGetUniformLocation(program.id(), (prefix + ".innerAngle").c_str()), light.inner_angle);
    glUniform1f(glGetUniformLocation(program.id(), (prefix + ".outerAngle").c_str()), light.outer_angle);
}

}

LightingPass::LightingPass(GBuffer& gbuffer)
    : gbuffer_(gbuffer) {
    program_.attachSource(GL_VERTEX_SHADER, LightingVertexSource);
    program_.attachSource(GL_FRAGMENT_SHADER, LightingFragmentSource);
    program_.link();
}

void LightingPass::resize(int, int) noexcept {
}

void LightingPass::execute(const Frame& frame) {
    if (gbuffer_.width() <= 0 || gbuffer_.height() <= 0) {
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, gbuffer_.width(), gbuffer_.height());
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    const auto& clear_color = frame.get(Frame::Vector4f::ClearColor);
    glClearColor(clear_color.x() / 255.0f, clear_color.y() / 255.0f, clear_color.z() / 255.0f, clear_color.w() / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    program_.use();
    program_.uniform("uPosition", 0);
    program_.uniform("uNormal", 1);
    program_.uniform("uAlbedoSpecular", 2);
    program_.uniform("uDepth", 3);

    const auto& camera_position = frame.get(Frame::Vector3f::CameraPosition);
    const auto& ambient_light = frame.get(Frame::Vector3f::AmbientLight);
    program_.uniform3f("uCameraPosition", camera_position.x(), camera_position.y(), camera_position.z());
    program_.uniform3f("uAmbientLight", ambient_light.x(), ambient_light.y(), ambient_light.z());

    const auto light_count = std::min(frame.lights().size(), static_cast<size_t>(64));
    program_.uniform("uLightCount", static_cast<GLint>(light_count));

    gbuffer_.bindForLighting();
    for (size_t index = 0; index < light_count; ++index) {
        setLightUniform(program_, static_cast<int>(index), frame.lights()[index]);
    }

    vertex_array_.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    gl::VertexArray::unbind();

    glDepthMask(GL_TRUE);
    gl::Program::unuse();
}

VOLCANO_GRAPHICS_END
