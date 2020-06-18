//
//
#ifndef VOLCANO_SCENE_H
#define VOLCANO_SCENE_H

#include <QVector>
#include <QObject>
#include <QVector3D>

#include <Volcano/Common.h>
#include <Volcano/Object.h>

VOLCANO_BEGIN

class VOLCANO_API Scene: public Object
{
    Q_OBJECT
    Q_PROPERTY(QVector3D gravity READ gravity WRITE setGravity NOTIFY gravityChanged)

public:
    class Listener {
    public:
        Listener(Scene *scene);
        virtual ~Listener(void);

    public:
        Scene *scene(void);
        virtual void update(float elapsed) = 0;
        virtual void render(void) = 0;

    private:
        Scene *m_scene;
    };

public:
    Scene(void);
    virtual ~Scene(void);

public:
    virtual void update(float elapsed);
    bool addListener(Listener *p);
    bool removeListener(Listener *p);
    const QVector3D &gravity(void) const;
    void setGravity(const QVector3D &v);

signals:
    void gravityChanged(const QVector3D &v);

private:
    typedef QVector<Listener *> ListenerArray;
    ListenerArray::iterator findListener(Listener *p);

private:
    ListenerArray m_listners;
    QVector3D m_gravity;
};

VOLCANO_INLINE Scene *Scene::Listener::scene(void)
{
    Q_ASSERT(m_scene != nullptr);

    return m_scene;
}

VOLCANO_INLINE const QVector3D &Scene::gravity(void) const
{
    return m_gravity;
}

VOLCANO_INLINE void Scene::setGravity(const QVector3D &v)
{
    if (m_gravity != v)
    {
        m_gravity = v;
        gravityChanged(v);
    }
}

VOLCANO_END

#endif // VOLCANO_SCENE_H
