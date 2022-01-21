//
//
#include <QMutex>
#include <QMutexLocker>

#include <Volcano/RunnableEvent.hpp>

VOLCANO_BEGIN

RunnableEvent::RunnableEvent(QRunnable *p):
    QEvent(typeId()),
    m_runnable(p)
{
}

RunnableEvent::~RunnableEvent(void)
{
    if (m_runnable != nullptr && m_runnable->autoDelete())
        delete m_runnable;
}

void RunnableEvent::run(void)
{
    if (m_runnable != nullptr) {
        m_runnable->run();
        if (m_runnable->autoDelete()) {
            delete m_runnable;
            m_runnable = nullptr;
        }
    }
}

QEvent::Type RunnableEvent::typeId(void)
{
    static QMutex mutex;
    static int type = -1;

    QMutexLocker locker(&mutex);

    if (type < 0)
        type = QEvent::registerEventType();

    return QEvent::Type(type);
}

VOLCANO_END
