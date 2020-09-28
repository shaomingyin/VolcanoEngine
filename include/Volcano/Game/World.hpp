//
//
#ifndef VOLCANO_GAME_WORLD_HPP
#define VOLCANO_GAME_WORLD_HPP

#include <QObject>
#include <QQmlListProperty>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Camera.hpp>
#include <Volcano/Game/Scene.hpp>

VOLCANO_GAME_BEGIN

typedef QList<Scene *> SceneList;
typedef QList<Camera *> CameraList;

class VOLCANO_API World: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(Camera *camera READ camera WRITE setCamera NOTIFY cameraChanged)
    Q_PROPERTY(QQmlListProperty<Volcano::Game::Scene> scenes READ scenes)
    Q_PROPERTY(QQmlListProperty<Volcano::Game::Camera> cameras READ cameras)

public:
    World(QObject *parent = nullptr);
    ~World(void) override;

public:
    const QString &name(void) const;
    void setName(const QString &v);
    Camera *camera(void);
    void setCamera(Camera *p);
    QQmlListProperty<Scene> scenes(void);
    QQmlListProperty<Camera> cameras(void);
    void update(float elapsed);

signals:
    void nameChanged(const QString &name);
    void cameraChanged(Camera *p);

private:
    static void appendScene(QQmlListProperty<Scene> *p, Scene *n);
    static int sceneCount(QQmlListProperty<Scene> *p);
    static Scene *sceneAt(QQmlListProperty<Scene> *p, int i);
    static void clearScene(QQmlListProperty<Scene> *p);
    static void replaceScene(QQmlListProperty<Scene> *p, int i, Scene *n);
    static void removeLastScene(QQmlListProperty<Scene> *p);
    static void appendCamera(QQmlListProperty<Camera> *p, Camera *n);
    static int cameraCount(QQmlListProperty<Camera> *p);
    static Camera *cameraAt(QQmlListProperty<Camera> *p, int i);
    static void clearCamera(QQmlListProperty<Volcano::Game::Camera> *p);
    static void replaceCamera(QQmlListProperty<Volcano::Game::Camera> *p, int i, Camera *n);
    static void removeLastCamera(QQmlListProperty<Volcano::Game::Camera> *p);

private:
    QString m_name;
    Camera *m_camera;
    SceneList m_sceneList;
    CameraList m_cameraList;
};

VOLCANO_INLINE const QString &World::name(void) const
{
    return m_name;
}

VOLCANO_INLINE void World::setName(const QString &v)
{
    if (m_name != v)
    {
        m_name = v;
        nameChanged(v);
    }
}

VOLCANO_INLINE Camera *World::camera(void)
{
    return m_camera;
}

VOLCANO_INLINE void World::setCamera(Camera *p)
{
    if (m_camera != p)
    {
        m_camera = p;
        cameraChanged(p);
    }
}

VOLCANO_GAME_END

#endif // VOLCANO_GAME_WORLD_HPP
