//
//
#include <Volcano/Scene.h>

VOLCANO_BEGIN

Scene::Listener::Listener(Scene *scene):
    m_scene(scene)
{
}

Scene::Listener::~Listener(void)
{
}

Scene::Scene(void)
{
}

Scene::~Scene(void)
{
}

void Scene::update(float elapsed)
{
    for (auto it(m_listners.begin()); it != m_listners.end(); ++it)
        (*it)->update(elapsed);
}

bool Scene::addListener(Listener *p)
{
    Q_ASSERT(p != nullptr);

    auto it(findListener(p));
    if (it != m_listners.end())
        return false;

    m_listners.push_back(p);

    // TODO attach notify

    return true;
}

bool Scene::removeListener(Listener *p)
{
    Q_ASSERT(p != nullptr);

    auto it(findListener(p));
    if (it == m_listners.end())
        return false;

    // TODO detach notify

    m_listners.erase(it);

    return true;
}

Scene::ListenerArray::iterator Scene::findListener(Listener *p)
{
    Q_ASSERT(p != nullptr);

    Scene::ListenerArray::iterator it;
    for (it = m_listners.begin(); it != m_listners.end(); ++it) {
        if ((*it) == p)
            return it;
    }

    return m_listners.end();
}

VOLCANO_END
