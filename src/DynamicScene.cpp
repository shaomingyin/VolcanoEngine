//
//
#include <Volcano/Scene.h>

VOLCANO_BEGIN

class SceneItem {
public:
    SceneItem(void);
    ~SceneItem(vodi);

public:

private:
    QVariantL
};

Scene::Scene(void)
{
}

Scene::~Scene(void)
{
}

void Scene::update(float elapsed)
{
    if (m_btWorld)
    {
        m_btWorld->stepSimulation(elapsed);
    }
}

void Scene::buildView(Camera &cam, View &view)
{
    view.reset();

    view.setViewport(cam.viewport());
    view.setViewMatrix(cam.viewMatrix());
    view.setProjectMatrix(cam.projectMatrix());

    // TODO
}

void Scene::setPhysicsEnabled(bool v)
{
    if (v)
    {
        if (m_btWorld)
            return;

        m_btcc.reset(new btDefaultCollisionConfiguration());
        m_btcd.reset(new btCollisionDispatcher(m_btcc.data()));
        m_btbi.reset(new btDbvtBroadphase());
        m_btsics.reset(new btSequentialImpulseConstraintSolver);
        m_btWorld.reset(new btDiscreteDynamicsWorld(m_btcd.data(), m_btbi.data(), m_btsics.data(), m_btcc.data()));
        m_btWorld->setGravity(btVector3(m_gravity.x(), m_gravity.y(), m_gravity.z()));
    }
    else
    {
        if (!m_btWorld)
            return;

        m_btWorld.reset();
        m_btsics.reset();
        m_btbi.reset();
        m_btcd.reset();
        m_btcc.reset();
    }

    physicsChanged(v);
}

VOLCANO_END
