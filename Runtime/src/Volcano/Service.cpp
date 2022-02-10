//
//
#include <Volcano/Service.hpp>

VOLCANO_BEGIN

Service::Service(QObject *parent):
    QObject(parent)
{
}

Service::~Service(void)
{
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

VOLCANO_END
