//
//
#include <Volcano/Game/Play.hpp>

VOLCANO_GAME_BEGIN

Play::Play(QObject *parent):
    QObject(parent),
    m_context(nullptr)
{
}

Play::~Play(void)
{
}

bool Play::init(Context *context)
{
    if (context == nullptr)
        return false;

    m_context = context;

    return true;
}

void Play::shutdown(void)
{
    m_context = nullptr;
}

void Play::update(float elapsed)
{
    Q_UNUSED(elapsed);
}

VOLCANO_GAME_END
