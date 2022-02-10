//
//
#include <memory>

#include <Volcano/System/Engine.hpp>

VOLCANO_SYSTEM_BEGIN

Engine::Engine(QObject *parent):
    Game::Context(parent),
    m_shouldQuit(false),
    m_qmlEngine(nullptr),
    m_qmlComponent(nullptr),
    m_gameManifest(nullptr),
    m_graphicsService(nullptr),
    m_inputService(nullptr),
    m_physicsService(nullptr),
    m_soundService(nullptr)
{
}

Engine::~Engine(void)
{
    if (m_gameManifest != nullptr)
        delete m_gameManifest;
}

Duration Engine::elapsed(void)
{
    return m_frameElapsed;
}

QVersionNumber Engine::version(void) const
{
    return QVersionNumber(VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, VOLCANO_VERSION_PATCH);
}

bool Engine::init(const QUrl &url)
{
    Q_ASSERT(url.isValid());
    Q_ASSERT(m_inputService != nullptr);
    Q_ASSERT(m_graphicsService != nullptr);
    Q_ASSERT(m_physicsService != nullptr);
    Q_ASSERT(m_soundService != nullptr);

    Q_ASSERT(m_qmlEngine == nullptr);
    auto qmlEng = std::make_unique<QQmlEngine>(this);
    if (!qmlEng)
        return false;

    attach(qmlEng.get());

    qmlEng->setOutputWarningsToStandardError(true);
    qmlEng->rootContext()->setContextObject(this);

    qInfo("Loading index Qml...");
    Q_ASSERT(m_qmlComponent == nullptr);
    auto qmlComponent = std::make_unique<QQmlComponent>(qmlEng.get(), url, this);
    if (!qmlComponent)
        return false;

    m_shouldQuit = false;
    m_qmlEngine = qmlEng.release();
    m_qmlComponent = qmlComponent.release();

    m_frameElapsed = 0s;

    if (m_qmlComponent->isLoading())
        connect(m_qmlComponent, &QQmlComponent::statusChanged, this, &Engine::onStatusChanged);
    else
        onStatusChanged(m_qmlComponent->status());

    return true;
}

bool Engine::shouldQuit(void) const
{
    return m_shouldQuit;
}

TimePoint Engine::frame(void)
{
    Q_ASSERT(m_inputService != nullptr);
    Q_ASSERT(m_graphicsService != nullptr);
    Q_ASSERT(m_physicsService != nullptr);
    Q_ASSERT(m_soundService != nullptr);

    auto curr = Clock::now();

    if (Q_UNLIKELY(curr < m_frameLast))
        return curr;

    if (Q_UNLIKELY(!m_qmlComponent->isReady()))
        return curr + m_frameElapsedMin;

    if (Q_UNLIKELY(m_qmlComponent == nullptr || !m_qmlComponent->isReady()))
        return curr + m_frameElapsedMin;

    auto diff = curr - m_frameCountLast;
    if (Q_UNLIKELY(diff >= 1s)) {
        m_frameCountPerSecond = m_frameCount;
        m_frameCount = 0;
        m_frameCountLast = curr;
        qDebug() << "fps: " << m_frameCountPerSecond;
    }

    auto frameNext = m_frameLast + m_frameElapsedMin;
    if (Q_UNLIKELY(curr < frameNext))
        return frameNext;

    m_frameElapsed = curr - m_frameLast;

    if (Q_LIKELY(m_physicsService != nullptr)) {
        m_physicsService->stepSimulation(m_frameElapsed);
        m_physicsService->resetTransform();
    }

    m_gameManifest->updateState();

    if (Q_LIKELY(m_graphicsService != nullptr)) {
        m_graphicsService->beginFrame();
        m_gameManifest->updateGraphics();
        m_graphicsService->endFrame();
    }

    if (Q_LIKELY(m_soundService != nullptr)) {
        // TODO
    }

    m_frameLast = curr;
    m_frameCount += 1;

    return curr + m_frameElapsedMin;
}

Input::Service *Engine::inputService(void)
{
    return m_inputService;
}

void Engine::setInputService(Input::Service *p)
{
    Q_ASSERT(p != nullptr);
    Q_ASSERT(m_inputService == nullptr);

    m_inputService = p;
}

Graphics::Service *Engine::graphicsService(void)
{
    return m_graphicsService;
}

void Engine::setGraphicsService(Graphics::Service *p)
{
    Q_ASSERT(p != nullptr);
    Q_ASSERT(m_graphicsService == nullptr);

    m_graphicsService = p;
}

Physics::Service *Engine::physicsService(void)
{
    return m_physicsService;
}

void Engine::setPhysicsService(Physics::Service *p)
{
    Q_ASSERT(p != nullptr);
    Q_ASSERT(m_physicsService == nullptr);

    m_physicsService = p;
}

Sound::Service *Engine::soundService(void)
{
    return m_soundService;
}

void Engine::setSoundService(Sound::Service *p)
{
    Q_ASSERT(p != nullptr);
    Q_ASSERT(m_soundService == nullptr);

    m_soundService = p;
}

void Engine::onStatusChanged(QQmlComponent::Status status)
{
    if (status != QQmlComponent::Ready) {
        if (status == QQmlComponent::Error) {
            auto errList = m_qmlComponent->errors();
            for (auto err: errList)
                qWarning() << err;
        }
        return;
    }

    auto rootObject = m_qmlComponent->create();
    if (rootObject == nullptr) {
        qCritical("Invalid game Qml.");
        return;
    }

    m_gameManifest = qobject_cast<Game::Manifest *>(rootObject);
    if (!m_gameManifest) {
        qCritical("Invalid game manifest.");
        return;
    }

    m_frameLast = Clock::now();
    m_frameCount = 0;
    m_frameCountPerSecond = 0;
    m_frameCountLast = m_frameLast;
    m_frameElapsedMin = std::chrono::microseconds(16666);
}

void Engine::quit(void)
{
    m_shouldQuit = true;
}

QmlQuaternionUtils *Engine::qmlQuaternionUtils(void)
{
    return &m_qmlQuaternionUtils;
}

VOLCANO_SYSTEM_END
