//
//
#include <Volcano/Game/ContextBase.hpp>

VOLCANO_GAME_BEGIN

ContextBase::ContextBase(void)
{
}

ContextBase::~ContextBase(void)
{
}

bool ContextBase::init(void)
{
    return true;
}

void ContextBase::frame(float elapsed)
{
}

std::string ContextBase::name(void)
{
    return "VolcanoBaseGame";
}

VOLCANO_GAME_END
