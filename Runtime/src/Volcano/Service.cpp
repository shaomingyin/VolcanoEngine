//
//
#include <Volcano/Service.hpp>

VOLCANO_BEGIN

Service::Service(QObject *parent):
    QObject(parent),
    m_isStarted(false)
{
}

Service::~Service(void)
{
}

bool Service::start(void)
{
    Q_ASSERT(!m_isStarted);

    m_transformStack.clear();
    m_transformStack.emplaceBack();

    m_isStarted = onStart();

    return m_isStarted;
}

void Service::stop(void)
{
    Q_ASSERT(m_isStarted);

    onStop();

    m_transformStack.clear();
    m_isStarted = false;
}

bool Service::isStarted(void) const
{
    return m_isStarted;
}

void Service::pushTransform(bool copyTop)
{
    if (!copyTop)
        m_transformStack.emplaceBack();
    else
        m_transformStack.push(m_transformStack.top());
}

void Service::popTransform(void)
{
    Q_ASSERT(m_transformStack.size() > 1);
    m_transformStack.pop();
}

void Service::resetTransform(void)
{
    m_transformStack.top().reset();
}

const Transform::Data &Service::transform(void) const
{
    return m_transformStack.top();
}

void Service::setTransform(const Transform::Data &v)
{
    m_transformStack.top() = v;
}

bool Service::onStart(void)
{
    return true;
}

void Service::onStop(void)
{
}

VOLCANO_END
