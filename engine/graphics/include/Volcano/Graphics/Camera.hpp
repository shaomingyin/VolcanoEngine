//
//
#ifndef VOLCANO_GRAPHICS_CAMERA_HPP
#define VOLCANO_GRAPHICS_CAMERA_HPP

#include <QList>
#include <QQueue>
#include <QAtomicInt>
#include <QRectF>
#include <QColor>
#include <QVector3D>
#include <QMatrix4x4>
#include <QElapsedTimer>
#include <QQuickFramebufferObject>

#include <Volcano/Game/Component.hpp>
#include <Volcano/Game/DirectionalLight.hpp>
#include <Volcano/Game/Entity.hpp>
#include <Volcano/Game/Light.hpp>
#include <Volcano/Game/Material.hpp>
#include <Volcano/Game/Mesh.hpp>
#include <Volcano/Game/Object.hpp>
#include <Volcano/Game/PointLight.hpp>
#include <Volcano/Game/Resource.hpp>
#include <Volcano/Game/SpotLight.hpp>
#include <Volcano/Game/World.hpp>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Renderer.hpp>
#include <Volcano/Graphics/Mesh.hpp>
#include <Volcano/Graphics/VisibleSet.hpp>

VOLCANO_GRAPHICS_BEGIN

class Camera: public QQuickFramebufferObject {
    Q_OBJECT
    Q_PROPERTY(int fps READ fps NOTIFY fpsChanged)
    Q_PROPERTY(int fpsMax READ fpsMax WRITE setFpsMax NOTIFY fpsMaxChanged)
    Q_PROPERTY(bool clear READ isClear WRITE setClear NOTIFY clearChanged)
    Q_PROPERTY(QColor clearColor READ clearColor WRITE setClearColor NOTIFY clearColorChanged)
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QVector3D direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(QVector3D up READ up WRITE setUp NOTIFY upChanged)
    Q_PROPERTY(bool perpective READ isPerpective WRITE setPerpective NOTIFY perpectiveChanged)
    Q_PROPERTY(qreal fov READ fov WRITE setFov NOTIFY fovChanged)
    Q_PROPERTY(QRectF rect READ rect WRITE setRect NOTIFY rectChanged)
    Q_PROPERTY(float nearPlane READ nearPlane WRITE setNearPlane NOTIFY nearPlaneChanged)
    Q_PROPERTY(float farPlane READ farPlane WRITE setFarPlane NOTIFY farPlaneChanged)
    Q_PROPERTY(Game::World *gameWorld READ gameWorld WRITE setGameWorld NOTIFY gameWorldChanged)

public:
    Camera(QQuickItem *parent = nullptr);
    ~Camera(void) override;

public:
    QQuickFramebufferObject::Renderer *createRenderer(void) const override;
    int fps(void) const;
    int fpsMax(void) const;
    void setFpsMax(int v);
    bool isClear(void) const;
    void setClear(bool v);
    const QColor &clearColor(void) const;
    void setClearColor(const QColor &v);
    const QVector3D &position(void) const;
    void setPosition(const QVector3D &v);
    const QVector3D &direction(void) const;
    void setDirection(const QVector3D &v);
    const QVector3D &up(void) const;
    void setUp(const QVector3D &v);
    bool isPerpective(void) const;
    void setPerpective(bool v);
    qreal fov(void) const;
    void setFov(qreal v);
    const QRectF &rect(void) const;
    void setRect(const QRectF &v);
    float nearPlane(void) const;
    void setNearPlane(float v);
    float farPlane(void) const;
    void setFarPlane(float v);
    Game::World *gameWorld(void);
    void setGameWorld(Game::World *gameWorld);
    const VisibleSet *lockVisibleSet(void);
    void unlockVisibleSet(void);

signals:
    void fpsChanged(int v);
    void fpsMaxChanged(int v);
    void clearChanged(bool v);
    void clearColorChanged(const QColor &v);
    void positionChanged(const QVector3D &v);
    void directionChanged(const QVector3D &v);
    void upChanged(const QVector3D &v);
    void perpectiveChanged(bool v);
    void fovChanged(qreal v);
    void rectChanged(const QRectF &v);
    void nearPlaneChanged(float v);
    void farPlaneChanged(float v);
    void gameWorldChanged(Game::World *gameWorld);

protected:
    void timerEvent(QTimerEvent *event) override;
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *updatePaintNodeData) override;

private slots:
    void addGameObject(Game::Object *gameObject);
    void removeGameObject(Game::Object *gameObject);

private:
    void frame(void);
    void buildVisibleSet(VisibleSet &out);
    void reset(void);
    void addGameEntity(Game::Entity *gameEntity);
    void addGameMesh(Game::Mesh *gameMesh);
    void addGameDirectionalLight(Game::DirectionalLight *gameDirectionalLight);
    void addGamePointLight(Game::PointLight *gamePointLight);
    void addGameSpotLight(Game::SpotLight *gameSpotLight);
    void removeGameEntity(Game::Entity *gameEntity);

private:
    using MeshList = QList<Mesh>;

private:
    int m_frameTimer;
    int m_frameCountTimer;
    int m_frameCount;
    int m_frameCountMax;
    int m_frameCountPerSecond;
    QElapsedTimer m_frameElapsedTimer;
    bool m_isClear;
    QColor m_clearColor;
    QVector3D m_position;
    QVector3D m_direction;
    QVector3D m_up;
    bool m_isPerpective;
    qreal m_fov;
    QRectF m_rect;
    float m_nearPlane;
    float m_farPlane;
    Game::World *m_gameWorld;
    MeshList m_meshList;
    MeshList m_meshListPengine;
    VisibleSet m_vsFlip[2];
    QAtomicInt m_vsState;
    int m_vsRendering;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_CAMERA_HPP
