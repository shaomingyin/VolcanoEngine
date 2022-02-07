//
//
#include <memory>

#include <Volcano/System/Core.hpp>

VOLCANO_SYSTEM_BEGIN

Core::Core(QObject *parent):
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

Core::~Core(void)
{
    if (m_gameManifest != nullptr)
        delete m_gameManifest;
}

bool Core::init(const QUrl &url)
{
    Q_ASSERT(url.isValid());

    Q_ASSERT(m_qmlEngine == nullptr);
    auto qmlEng = std::make_unique<QQmlEngine>(this);
    if (!qmlEng)
        return false;

    attach(qmlEng.get());

    qmlEng->setOutputWarningsToStandardError(true);
    //qmlEng->rootContext()->setContextProperty("v", &m_globals);
    qmlEng->rootContext()->setContextObject(this);

    qInfo("Loading index Qml...");
    Q_ASSERT(m_qmlComponent == nullptr);
    auto qmlComponent = std::make_unique<QQmlComponent>(qmlEng.get(), url, this);
    if (!qmlComponent)
        return false;

    m_shouldQuit = false;
    m_qmlEngine = qmlEng.release();
    m_qmlComponent = qmlComponent.release();

    if (m_qmlComponent->isLoading())
        connect(m_qmlComponent, &QQmlComponent::statusChanged, this, &Engine::onStatusChanged);
    else
        onStatusChanged(m_qmlComponent->status());

    return true;
}

bool Core::shouldQuit(void) const
{
    return m_shouldQuit;
}

TimePoint Core::frame(void)
{
    auto curr = Clock::now();
    if (Q_UNLIKELY(curr < m_frameLast))
        return curr;

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

    auto elapsed = curr - m_frameLast;

    if (Q_LIKELY(m_physicsService != nullptr && m_physicsService->isStarted())) {
        m_physicsService->stepSimulation(elapsed);
        m_physicsService->resetTransform();
    }

    m_gameManifest->tick(elapsed);

    if (Q_LIKELY(m_graphicsService != nullptr && m_graphicsService->isStarted())) {
        m_graphicsService->beginFrame();
        m_gameManifest->draw();
        m_graphicsService->endFrame();
    }

    if (Q_LIKELY(m_soundService != nullptr && m_soundService->isStarted())) {
        // TODO
    }

    m_frameLast = curr;
    m_frameCount += 1;

    return curr + m_frameElapsedMin;
}

bool Core::registerService(QMetaObject *p)
{
    Q_ASSERT(p != nullptr);

    if (m_serviceMetaObjectList.contains(p))
        return false;

    if (!p->inherits(&Service::staticMetaObject))
        return false;

    m_serviceMetaObjectList.append(p);

    return true;
}

bool Core::unregisterService(QMetaObject *p)
{
    Q_ASSERT(p != nullptr);

    if (!m_serviceMetaObjectList.contains(p))
        return false;

    if
}

Input::Service *Core::inputService(void)
{
    return m_inputService;
}

QStringList Core::graphicsServiceList(void)
{
    return QStringList() << "OpenGL";
}

Graphics::Service *Core::graphicsService(void)
{
    return m_graphicsService;
}

QString Core::graphicsServiceName(void) const
{
    return "OpenGL";
}

bool Core::setGraphicsService(const QString &name)
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

QStringList Core::physicsServiceList(void)
{
    return QStringList() << "Bullet";
}

Physics::Service *Core::physicsService(void)
{
    return m_physicsService;
}

QString Core::physicsServiceName(void) const
{
    return "Bullet";
}

bool Core::setPhysicsService(const QString &name)
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

QStringList Core::soundServiceList(void)
{
    return QStringList() << "OpenAL";
}

Sound::Service *Core::soundService(void)
{
    return m_soundService;
}

QString Core::soundServiceName(void) const
{
    return "OpenAL";
}

bool Core::setSoundService(const QString &name)
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

void Core::onStatusChanged(QQmlComponent::Status status)
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

    m_inputService = new Input::Service(this);
    m_inputService->start();

    m_frameLast = Clock::now();
    m_frameCount = 0;
    m_frameCountPerSecond = 0;
    m_frameCountLast = m_frameLast;
    m_frameElapsedMin = std::chrono::microseconds(16666);
}

QVersionNumber Core::version(void) const
{
    return QVersionNumber(VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, VOLCANO_VERSION_PATCH);
}

QuaternionUtils *Core::quaternionUtils(void)
{
    return &m_quaternionUtils;
}

void Core::quit(void)
{
    m_shouldQuit = true;
}

VOLCANO_SYSTEM_END
