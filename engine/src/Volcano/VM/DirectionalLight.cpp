//
//
#include <Volcano/VM/DirectionalLight.hpp>

VOLCANO_VM_BEGIN

DirectionalLight::DirectionalLight(void):
    m_direction(0.0f, -1.0f, 0.0f)
{
}

DirectionalLight::~DirectionalLight(void)
{
}

void DirectionalLight::applyToView(Graphics::View &view)
{
    Light::applyToView(view);

    // TODO
}

VOLCANO_VM_END
