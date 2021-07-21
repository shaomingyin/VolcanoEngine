//
//
#include <QThread>

#include <Volcano/Game/Resource.hpp>

VOLCANO_GAME_BEGIN

Resource::Resource(QObject *parent):
    Component(parent),
    m_status(None),
    m_progress(1.0f)
{
}

Resource::~Resource(void)
{
}

const QUrl &Resource::source(void) const
{
    return m_source;
}

void Resource::setSource(const QUrl &v)
{
    if (m_source == v)
        return;

    m_source = v;
    onSourceChanged(v);

    emit sourceChanged(v);
}

Resource::Status Resource::status(void) const
{
    return m_status;
}

qreal Resource::progress(void) const
{
    return m_progress;
}

void Resource::setStatus(Status v)
{
    if (QThread::currentThread() == thread()) {
        if (m_status != v) {
            m_status = v;
            emit statusChanged(v);
        }
        return;
    }
}

void Resource::setProgress(qreal v)
{
    qreal tmp = qBound(0.0f, v, 1.0f);
    if (!qFuzzyCompare(m_progress, tmp)) {
        m_progress = tmp;
        emit progressChanged(tmp);
    }
}

VOLCANO_GAME_END
