//
//
#include <Volcano/ScopeGuard.hpp>
#include <Volcano/Graphics/Renderer.hpp>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer(const Napi::CallbackInfo &info):
    Node::Object<Renderer>(info),
    m_clearEnabled(true),
    m_viewport(0, 0, 0, 0)
{
}

Renderer::~Renderer(void)
{
}

void Renderer::registerClass(Napi::Env env)
{
    registerConstructor(env, "Renderer", {
        InstanceAccessor<&Renderer::clearEnabled, &Renderer::setClearEnabled>("clearEnabled"),
        InstanceAccessor<&Renderer::clearColor, &Renderer::setClearColor>("clearColor"),
        InstanceAccessor<&Renderer::viewport, &Renderer::setViewport>("viewport")
    });
}

bool Renderer::init(int width, int height)
{
    m_width = width;
    m_height = height;

    return true;
}

void Renderer::beginFrame(void)
{
}

void Renderer::endFrame(void)
{
    glViewport(0, 0, m_width, m_height);
    glClear(GL_COLOR_BUFFER_BIT);

    // m_program.use();
    // setup view matrix uniform.
}

Renderer::MatrixMode Renderer::matrixMode(void) const
{
    return m_matrixMode;
}

void Renderer::setMatrixMode(MatrixMode mode)
{
    m_matrixMode = mode;
}

void Renderer::loadIdentity(void)
{
#if 0
    m_matrix[m_matrixMode].setToIdentity();
    m_translate = QVector3D(0.0f, 0.0f, 0.0f);
    m_scale = QVector3D(0.0f, 0.0f, 0.0f);
    m_rotation = QQuaternion(0.0f, 0.0f, 0.0f, 0.0f);
#endif
}

void Renderer::translate(const Eigen::Vector3f &r)
{
    //m_translate += r;
}

void Renderer::scale(const Eigen::Vector3f &r)
{
    //m_scale *= r;
}

void Renderer::rotation(float angle, const Eigen::Vector3f &axis)
{
    //m_rotation *= Eigen::Quaternionf(Eigen::AngleAxisf(angle, axis));
}

void Renderer::add(DirectionalLight &directionalLight)
{

}

void Renderer::add(PointLight &pointLight)
{

}

void Renderer::add(SpotLight &spotLight)
{

}

Napi::Value Renderer::clearEnabled(const Napi::CallbackInfo &info)
{
    return Napi::Value::From(info.Env(), m_clearEnabled);
}

void Renderer::setClearEnabled(const Napi::CallbackInfo &info, const Napi::Value &value)
{
    m_clearEnabled = value.ToBoolean();
}

Napi::Value Renderer::clearColor(const Napi::CallbackInfo &info)
{
    auto value = Napi::Array::New(info.Env());

    value[0u] = m_clearColor[0];
    value[1u] = m_clearColor[1];
    value[2u] = m_clearColor[2];

    return value;
}

void Renderer::setClearColor(const Napi::CallbackInfo &info, const Napi::Value &value)
{
    auto object = value.ToObject();

    m_clearColor[0] = object.Get(0u).ToNumber().FloatValue();
    m_clearColor[1] = object.Get(1u).ToNumber().FloatValue();
    m_clearColor[2] = object.Get(2u).ToNumber().FloatValue();
}

Napi::Value Renderer::viewport(const Napi::CallbackInfo &info)
{
    auto value = Napi::Array::New(info.Env());

    value[0u] = m_viewport[0];
    value[1u] = m_viewport[1];
    value[2u] = m_viewport[2];
    value[3u] = m_viewport[3];

    return value;
}

void Renderer::setViewport(const Napi::CallbackInfo &info, const Napi::Value &value)
{
    auto object = value.ToObject();

    m_viewport[0] = object.Get(0u).ToNumber().Int32Value();
    m_viewport[1] = object.Get(1u).ToNumber().Int32Value();
    m_viewport[2] = object.Get(2u).ToNumber().Int32Value();
    m_viewport[3] = object.Get(3u).ToNumber().Int32Value();
}

VOLCANO_GRAPHICS_END
