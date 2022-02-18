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

Object::Object(Context *context, QObject *parent):
    QObject(parent),
    m_isEnabled(true),
    m_context(context),
    m_graphicsService(nullptr),
    m_physicsService(nullptr),
    m_soundService(nullptr)
{
    Q_ASSERT(m_context != nullptr);
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

void Object::updateState(void)
{
    if (Q_LIKELY(m_isEnabled))
        tick();
}

void Object::updateGraphics(void)
{
    if (Q_LIKELY(m_isEnabled)) {
        auto gService = graphicsService();
        if (Q_LIKELY(gService != nullptr))
            draw();
    }
}

Context *Object::context(void)
{
    if (Q_LIKELY(m_context != nullptr))
        return m_context;

    m_context = Context::fromObject(this);

    return m_context;
}

Duration Object::elapsed(void)
{
    auto p = context();
    if (Q_LIKELY(p != nullptr))
        return p->elapsed();

    return 0s;
}

Input::Service *Object::inputService(void)
{
    if (Q_LIKELY(m_inputService != nullptr))
        return m_inputService;

    auto p = context();
    Q_ASSERT(p != nullptr);
    m_inputService = p->inputService();
    Q_ASSERT(m_inputService != nullptr);

    return m_inputService;
}

Graphics::Service *Object::graphicsService(void)
{
    if (Q_LIKELY(m_graphicsService != nullptr))
        return m_graphicsService;

    auto p = context();
    Q_ASSERT(p != nullptr);
    m_graphicsService = p->graphicsService();
    Q_ASSERT(m_graphicsService != nullptr);

    return m_graphicsService;
}

Physics::Service *Object::physicsService(void)
{
    if (Q_LIKELY(m_physicsService != nullptr))
        return m_physicsService;

    auto p = context();
    Q_ASSERT(p != nullptr);
    m_physicsService = p->physicsService();
    Q_ASSERT(m_physicsService != nullptr);

    return m_physicsService;
}

Sound::Service *Object::soundService(void)
{
    if (Q_LIKELY(m_soundService != nullptr))
        return m_soundService;

    auto p = context();
    Q_ASSERT(p != nullptr);
    m_soundService = p->soundService();
    Q_ASSERT(m_soundService != nullptr);

    return m_soundService;
}

void Object::tick(void)
{
}

void Object::draw(void)
{
}

VOLCANO_GAME_END
