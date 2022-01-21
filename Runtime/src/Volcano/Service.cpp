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

    resetTransform();
    m_isStarted = onStart();

    return m_isStarted;
}

void Service::stop(void)
{
    Q_ASSERT(m_isStarted);

    onStop();

    m_isStarted = false;
}

bool Service::isStarted(void) const
{
    return m_isStarted;
}

void Service::translate(const QVector3D &v)
{
    m_transformStack.top().setTranslate(v);
}

void Service::translateTo(const QVector3D &v)
{
    m_transformStack.top().setTranslateOffset(v);
}

void Service::scale(const QVector3D &v)
{
    m_transformStack.top().setScale(v);
}

void Service::scaleTo(const QVector3D &v)
{
    m_transformStack.top().setScaleOffset(v);
}

void Service::rotate(const QQuaternion &v)
{
    m_transformStack.top().setRotation(v);
}

void Service::rotateTo(const QQuaternion &v)
{
    m_transformStack.top().setRotationOffset(v);
}

void Service::resetTransform(void)
{
    m_transformStack.resize(1);
    m_transformStack.first().reset();
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

const Transform &Service::transform(void)
{
    return m_transformStack.top();
}

bool Service::onStart(void)
{
    return true;
}

void Service::onStop(void)
{
}

VOLCANO_END
