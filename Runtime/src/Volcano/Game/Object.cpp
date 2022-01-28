//
//
#include <Volcano/Game/Object.hpp>

VOLCANO_GAME_BEGIN

Object::Object(QObject *parent):
    QObject(parent),
    m_isEnabled(true),
    m_context(nullptr),
    m_graphicsService(nullptr),
    m_physicsService(nullptr),
    m_soundService(nullptr)
{
}

Object::~Object(void)
{
}

bool Object::isEnabled(void) const
{
    return m_isEnabled;
}

void Object::setEnabled(bool v)
{
    if (m_isEnabled != v) {
        m_isEnabled = v;
        emit enabledChanged(v);
    }
}

void Object::tick(Duration elapsed)
{
    if (Q_LIKELY(m_isEnabled))
        onTick(elapsed);
}

void Object::draw(void)
{
    if (Q_LIKELY(m_isEnabled)) {
        auto gService = graphicsService();
        if (Q_LIKELY(gService != nullptr && gService->isStarted()))
            onDraw();
    }
}

Context *Object::context(void)
{
    if (Q_LIKELY(m_context != nullptr))
        return m_context;

    m_context = Context::fromObject(this);

    return m_context;
}

Graphics::Service *Object::graphicsService(void)
{
    if (Q_LIKELY(m_graphicsService != nullptr))
        return m_graphicsService;

    auto p = context();
    if (p != nullptr)
        m_graphicsService = p->graphicsService();

    return m_graphicsService;
}

Physics::Service *Object::physicsService(void)
{
    if (Q_LIKELY(m_physicsService != nullptr))
        return m_physicsService;

    auto p = context();
    if (p != nullptr)
        m_physicsService = p->physicsService();

    return m_physicsService;
}

Sound::Service *Object::soundService(void)
{
    if (Q_LIKELY(m_soundService != nullptr))
        return m_soundService;

    auto p = context();
    if (p != nullptr)
        m_soundService = p->soundService();

    return m_soundService;
}

void Object::onTick(Duration elapsed)
{
    Q_UNUSED(elapsed);
}

void Object::onDraw(void)
{
}

VOLCANO_GAME_END
