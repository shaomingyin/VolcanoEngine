//
//
#ifndef VOLCANO_WORLD_HPP
#define VOLCANO_WORLD_HPP

#include <QVector3D>

#include <Volcano/Common.hpp>
#include <Volcano/Camera.hpp>
#include <Volcano/Node.hpp>
#include <Volcano/WorldAmbient.hpp>

VOLCANO_BEGIN

class World: public Node
{
    Q_OBJECT
    Q_PROPERTY(QVector3D gravity READ gravity WRITE setGravity NOTIFY gravityChanged)
    Q_PROPERTY(Camera *viewport READ viewport WRITE setViewport NOTIFY viewportChanged)
    Q_PROPERTY(Node *focus READ focus WRITE setFocus NOTIFY focusChanged)
    Q_PROPERTY(WorldAmbient *ambient READ ambient)

public:
    World(Node *parent = nullptr);
    ~World(void) override;

public:
    const QVector3D &gravity(void) const;
    void setGravity(const QVector3D &v);
    Camera *viewport(void);
    void setViewport(Camera *p);
    Node *focus(void);
    void setFocus(Node *p);
    WorldAmbient *ambient(void);

signals:
    void gravityChanged(const QVector3D &v);
    void viewportChanged(Camera *p);
    void focusChanged(Node *p);

protected:
    void onUpdate(float elapsed) override;
    void onDraw(OpenGL::View &view) override;

private:
    QVector3D m_gravity;
    Camera *m_viewport;
    Node *m_focus;
    WorldAmbient m_ambient;
};

VOLCANO_INLINE const QVector3D &World::gravity(void) const
{
    return m_gravity;
}

VOLCANO_INLINE Camera *World::viewport(void)
{
    return m_viewport;
}

VOLCANO_INLINE Node *World::focus(void)
{
    return m_focus;
}

VOLCANO_INLINE WorldAmbient *World::ambient(void)
{
    return &m_ambient;
}

VOLCANO_END

#endif // VOLCANO_WORLD_HPP
