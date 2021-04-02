//
//
#include <Volcano/Game/Context.hpp>

VOLCANO_GAME_BEGIN

Context::Context(void):
    m_world(nullptr)
{
}

Context::~Context(void)
{
    if (m_world != nullptr)
        delete m_world;
}

bool Context::init(rttr::type screenType, rttr::type worldType, rttr::type cameraType)
{
    VOLCANO_ASSERT(m_screen == nullptr);
    VOLCANO_ASSERT(m_world == nullptr);
    VOLCANO_ASSERT(m_camera == nullptr);

    m_world = createWorld();
    if (m_world == nullptr)
        return false;

    return true;
}

void Context::update(float elapsed)
{
    if (m_world != nullptr)
        m_world->update(elapsed);
}

std::string Context::name(void)
{
    return "VolcanoGame";
}

VOLCANO_GAME_END
