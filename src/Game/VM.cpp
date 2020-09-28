//
//
#include <Volcano/Game/VM.hpp>

VOLCANO_GAME_BEGIN

VM::VM(QObject *parent):
    QEventLoop(parent),
    m_qmlEngine(nullptr),
    m_qmlComponent(nullptr),
    m_frameTimer(0),
    m_countTimer(0)
{
}

VM::VM(const QUrl &url, QObject *parent):
    QEventLoop(parent),
    m_url(url),
    m_qmlEngine(nullptr),
    m_qmlComponent(nullptr),
    m_frameTimer(0),
    m_countTimer(0)
{
}

VM::~VM(void)
{
    stop();
}

bool VM::start(void)
{
    Q_ASSERT(m_url.isValid());
    Q_ASSERT(m_qmlEngine == nullptr);
    Q_ASSERT(m_qmlComponent == nullptr);

    m_qmlEngine = new QQmlEngine(this);
    if (m_qmlEngine == nullptr)
        return false;

    m_qmlComponent = new QQmlComponent(m_qmlEngine, this);
    if (m_qmlComponent == nullptr)
    {
        delete m_qmlEngine;
        m_qmlEngine = nullptr;
        return false;
    }

    m_qmlComponent->loadUrl(m_url, QQmlComponent::PreferSynchronous);
    if (!m_qmlComponent->isReady())
        connect(m_qmlComponent, &QQmlComponent::statusChanged, this, &VM::statusChanged);
    else
        statusChanged(QQmlComponent::Ready);

    return true;
}

void VM::stop(void)
{
    if (m_countTimer > 0)
    {
        killTimer(m_countTimer);
        m_countTimer = 0;
    }

    if (m_frameTimer > 0)
    {
        killTimer(m_frameTimer);
        m_frameTimer = 0;
    }

    if (m_qmlComponent != nullptr)
    {
        delete m_qmlComponent;
        m_qmlComponent = nullptr;
    }

    if (m_qmlEngine != nullptr)
    {
        delete m_qmlEngine;
        m_qmlEngine = nullptr;
    }
}

void VM::statusChanged(QQmlComponent::Status v)
{
    if (v != QQmlComponent::Ready)
    {
        stop();
        error("ERROR1"); // TODO
        return;
    }

    QObject *object = m_qmlComponent->create();
    if (object == nullptr)
    {
        stop();
        error("ERROR2"); // TODO
        return;
    }

    m_world = qobject_cast<World *>(object);
    if (m_world == nullptr)
    {
        delete object;
        stop();
        error("ERROR3"); // TODO
        return;
    }

    m_frameCount = 0;
    m_frameCountPerSecond = 0;
    m_frameTimer = startTimer(16, Qt::PreciseTimer);
    m_countTimer = startTimer(1000);
    m_elapsedTimer.start();

    ready();
}

void VM::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_frameTimer)
    {
        qint64 elapsed = m_elapsedTimer.restart();
        Q_ASSERT(m_world != nullptr);
        m_world->update(float(elapsed));
        m_frameCount += 1;
        return;
    }

    if (event->timerId() == m_countTimer)
    {
        m_frameCountPerSecond = m_frameCount;
        m_frameCount = 0;
        return;
    }
}

VOLCANO_GAME_END
