//
//
#include <memory>

#include <Volcano/Game/DynamicWorld.hpp>

VOLCANO_GAME_BEGIN

DynamicWorld::DynamicWorld(QObject *parent):
    World(parent),
    m_btCollisionConfiguration(nullptr),
    m_btDispatcher(nullptr),
    m_btOverlappingPairCache(nullptr),
    m_btSolver(nullptr),
    m_btWorld(nullptr),
    m_gravity(0.0f, -9.8f, 0.0f)
{
    init();
}

DynamicWorld::~DynamicWorld(void)
{
    if (m_btWorld != nullptr)
        release();
}

void DynamicWorld::tick(Duration elapsed)
{
    if (Q_LIKELY(m_btWorld != nullptr)) {
        auto ms = std::chrono::duration_cast<std::chrono::microseconds>(elapsed);
        m_btWorld->stepSimulation(float(ms.count()) / 1000000.0f);
        // TODO
    }

    World::tick(elapsed);
}

const QVector3D &DynamicWorld::gravity(void) const
{
    return m_gravity;
}

void DynamicWorld::setGravity(const QVector3D &v)
{
    if (qFuzzyCompare(m_gravity, v))
        return;

    m_gravity = v;

    if (m_btWorld != nullptr)
        m_btWorld->setGravity(btVector3(m_gravity[0], m_gravity[1], m_gravity[2]));

    emit gravityChanged(v);
}

void DynamicWorld::handleActorAdded(Actor *actor)
{
    World::handleActorAdded(actor);
}

void DynamicWorld::handleActorRemoved(Actor *actor)
{
    World::handleActorRemoved(actor);
}

bool DynamicWorld::init(void)
{
    Q_ASSERT(m_btWorld == nullptr);

    auto btCollisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
    if (!btCollisionConfiguration)
        return false;

    auto btDispatcher = std::make_unique<btCollisionDispatcher>(btCollisionConfiguration.get());
    if (!btDispatcher)
        return false;

    auto btOverlappingPairCache = std::make_unique<btDbvtBroadphase>();
    if (!btOverlappingPairCache)
        return false;

    auto btSolver = std::make_unique<btSequentialImpulseConstraintSolver>();
    if (!btSolver)
        return false;

    auto btWorld = std::make_unique<btDiscreteDynamicsWorld>(
                btDispatcher.get(),
                btOverlappingPairCache.get(),
                btSolver.get(),
                btCollisionConfiguration.get());

    if (!btWorld)
        return false;

    btWorld->setGravity(btVector3(m_gravity.x(), m_gravity.y(), m_gravity.z()));

    m_btCollisionConfiguration = btCollisionConfiguration.release();
    m_btDispatcher = btDispatcher.release();
    m_btOverlappingPairCache = btOverlappingPairCache.release();
    m_btSolver = btSolver.release();
    m_btWorld = btWorld.release();

    return true;
}

void DynamicWorld::release(void)
{
    Q_ASSERT(m_btWorld != nullptr);

    delete m_btWorld;
    delete m_btSolver;
    delete m_btOverlappingPairCache;
    delete m_btDispatcher;
    delete m_btCollisionConfiguration;

    m_btWorld = nullptr;
    m_btSolver = nullptr;
    m_btOverlappingPairCache = nullptr;
    m_btDispatcher = nullptr;
    m_btCollisionConfiguration = nullptr;
}

VOLCANO_GAME_END
