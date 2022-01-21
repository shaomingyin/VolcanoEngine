//
//
#include <memory>

#include <Volcano/Graphics/OpenGL/Service.hpp>
#include <Volcano/Physics/Bullet/Service.hpp>
#include <Volcano/Sound/OpenAL/Service.hpp>

#include <Volcano/System/Core.hpp>

VOLCANO_SYSTEM_BEGIN

Core::Core(QObject *parent):
    Game::Context(parent),
    m_qmlEngine(nullptr),
    m_qmlComponent(nullptr),
    m_networkAccessManagerFactor(nullptr),
    m_gameManifest(nullptr),
    m_graphicsService(nullptr),
    m_inputService(nullptr),
    m_physicsService(nullptr),
    m_soundService(nullptr)
{
}

Core::~Core(void)
{
    if (m_gameManifest != nullptr)
        delete m_gameManifest;
    if (m_networkAccessManagerFactor != nullptr)
        delete m_networkAccessManagerFactor;
}

bool Core::init(const QUrl &url)
{
    Q_ASSERT(url.isValid());

    Q_ASSERT(m_qmlEngine == nullptr);
    auto engine = std::make_unique<QQmlEngine>(this);
    if (!engine)
        return false;

    attach(engine.get());

    Q_ASSERT(m_networkAccessManagerFactor == nullptr);
    auto namf = std::make_unique<NetworkAccessManagerFactory>();
    if (!namf) {
        qFatal("Failed to create network access manager factory.");
        return false;
    }

    engine->setNetworkAccessManagerFactory(namf.get());
    engine->setOutputWarningsToStandardError(true);

    Q_ASSERT(m_qmlComponent == nullptr);
    auto component = std::make_unique<QQmlComponent>(engine.get(), url, this);
    if (!component)
        return false;

    m_qmlEngine = engine.release();
    m_qmlComponent = component.release();
    m_networkAccessManagerFactor = namf.release();

    if (m_qmlComponent->status() == QQmlComponent::Ready)
        onStatusChanged(QQmlComponent::Ready);
    else
        connect(m_qmlComponent, &QQmlComponent::statusChanged, this, &Core::onStatusChanged);

    return true;
}

TimePoint Core::frame(void)
{
    auto curr = Clock::now();
    if (curr < m_frameLast)
        return curr;

    if (m_qmlComponent == nullptr || m_qmlComponent->status() != QQmlComponent::Ready)
        return curr + m_frameElapsedMin;

    auto diff = curr - m_frameCountLast;
    if (diff >= 1s) {
        m_frameCountPerSecond = m_frameCount;
        m_frameCount = 0;
        m_frameCountLast = curr;
        qDebug() << "fps: " << m_frameCountPerSecond;
    }

    auto frameNext = m_frameLast + m_frameElapsedMin;
    if (curr < frameNext)
        return frameNext;

    auto elapsed = curr - m_frameLast;

    if (m_physicsService != nullptr && m_physicsService->isStarted()) {
        m_physicsService->stepSimulation(elapsed);
        m_physicsService->resetTransform();
    }

    m_gameManifest->tick(elapsed);

    if (m_soundService != nullptr && m_soundService->isStarted()) {
        // TODO
    }

    if (m_graphicsService != nullptr && m_graphicsService->isStarted()) {
        m_graphicsService->beginFrame();
        m_gameManifest->draw();
        m_graphicsService->endFrame();
    }

    m_frameLast = curr;
    m_frameCount += 1;

    return curr + m_frameElapsedMin;
}

QStringList Core::enumateGraphicsServices(void)
{
    return QStringList() << "OpenGL";
}

bool Core::useGraphicsService(const QString &name)
{
    if (m_graphicsService != nullptr) {
        if (m_graphicsService->isStarted()) {
            emit graphicsServiceStopping();
            m_graphicsService->stop();
        }
        delete m_graphicsService;
        m_graphicsService = nullptr;
    }

    qDebug("Using %s graphics service...", qPrintable(name));

    if (name == "OpenGL")
        m_graphicsService = new Graphics::OpenGL::Service(this);

    if (m_graphicsService == nullptr)
        return false;

    if (m_graphicsService->start())
        emit graphicsSerivceStarted();

    return true;
}

Graphics::Service *Core::graphicsService(void)
{
    return m_graphicsService;
}

Input::Service *Core::inputService(void)
{
    return m_inputService;
}

QStringList Core::enumatePhysicsServices(void)
{
    return QStringList() << "Bullet";
}

bool Core::usePhysicsService(const QString &name)
{
    if (m_physicsService != nullptr) {
        if (m_physicsService->isStarted()) {
            emit physicsServiceStopping();
            m_physicsService->stop();
        }
        delete m_physicsService;
        m_physicsService = nullptr;
    }

    qInfo("Using %s physics service.", qPrintable(name));

    if (name == "Bullet")
        m_physicsService = new Physics::Bullet::Service(this);

    if (m_physicsService == nullptr)
        return false;

    if (m_physicsService->start())
        emit physicsServiceStarted();

    return true;
}

Physics::Service *Core::physicsService(void)
{
    return m_physicsService;
}

QStringList Core::enumateSoundServices(void)
{
    return QStringList() << "OpenAL";
}

bool Core::useSoundService(const QString &name)
{
    if (m_soundService != nullptr) {
        if (m_soundService->isStarted()) {
            emit soundServiceStopping();
            m_soundService->stop();
        }
        delete m_soundService;
        m_soundService = nullptr;
    }

    qInfo("Using %s sound service.", qPrintable(name));

    if (name == "OpenAL")
        m_soundService = new Sound::OpenAL::Service(this);

    if (m_soundService == nullptr)
        return false;

    if (m_soundService->start())
        emit soundServiceStarted();

    return true;
}

Sound::Service *Core::soundService(void)
{
    return m_soundService;
}

void Core::onStatusChanged(QQmlComponent::Status status)
{
    qDebug() << __LINE__ << status;
    if (status != QQmlComponent::Ready) {
        qCritical("Failed to load game Qml.");
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

    m_inputService = new Input::Service(this);
    m_inputService->start();

    useGraphicsService("OpenGL");
    usePhysicsService("Bullet");
    useSoundService("OpenAL");

    m_frameLast = Clock::now();
    m_frameCount = 0;
    m_frameCountPerSecond = 0;
    m_frameCountLast = m_frameLast;
    m_frameElapsedMin = std::chrono::microseconds(16666);
}

VOLCANO_SYSTEM_END
