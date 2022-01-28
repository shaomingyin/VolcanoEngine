//
//
#include <memory>

#include <Volcano/Physics/Bullet/Service.hpp>

VOLCANO_PHYSICS_BULLET_BEGIN

Service::Service(QObject *parent):
    Volcano::Physics::Service(parent),
    m_btCollisionConfiguration(nullptr),
    m_btDispatcher(nullptr),
    m_btOverlappingPairCache(nullptr),
    m_btSolver(nullptr),
    m_btWorld(nullptr),
    m_gravity(0.0f, -9.8f, 0.0f)
{
}

Service::~Service(void)
{
    if (m_btWorld != nullptr)
        onStop();
}

void Service::stepSimulation(Duration elapsed)
{
    if (Q_LIKELY(m_btWorld != nullptr)) {
        auto ms = std::chrono::duration_cast<std::chrono::microseconds>(elapsed);
        m_btWorld->stepSimulation(float(ms.count()) / 1000000.0f);
        // TODO
    }
}

const QVector3D &Service::gravity(void) const
{
    return m_gravity;
}

void Service::setGravity(const QVector3D &v)
{
    if (qFuzzyCompare(m_gravity, v))
        return;

    m_gravity = v;

    if (m_btWorld != nullptr)
        m_btWorld->setGravity(btVector3(m_gravity[0], m_gravity[1], m_gravity[2]));
}

Box *Service::createBox(float length, float width, float height)
{
    return nullptr;
}

void Service::updateBox(Box *p)
{
    Q_ASSERT(p != nullptr);
}

Cylinder *Service::createCylinder(float radius, float height)
{
    return nullptr;
}

void Service::updateCylinder(Cylinder *p)
{
    Q_ASSERT(p != nullptr);
}

Plane *Service::createPlane(float x, float y, float z, float d)
{
    return nullptr;
}

void Service::updatePlane(Plane *p)
{
    Q_ASSERT(p != nullptr);
}

Sphere *Service::createSphere(float radius)
{
    return nullptr;
}

void Service::updateSphere(Sphere *p)
{
    Q_ASSERT(p != nullptr);
}

bool Service::onStart(void)
{
    qInfo("Initializing bullet physics...");

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

    //emit dynamicChanged(true);

    return true;
}

void Service::onStop(void)
{
    qInfo("Shuting down bullet physics...");

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

    //emit dynamicChanged(false);
}

VOLCANO_PHYSICS_BULLET_END
