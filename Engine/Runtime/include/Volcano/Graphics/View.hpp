//
//
#ifndef VOLCANO_GRAPHICS_VIEW_HPP
#define VOLCANO_GRAPHICS_VIEW_HPP

#include <QList>
#include <QStack>
#include <QColor>
#include <QMatrix4x4>

#include <Volcano/Transform.hpp>

#include <Volcano/Game/Rotation.hpp>
#include <Volcano/Game/Box.hpp>
#include <Volcano/Game/Cylinder.hpp>
#include <Volcano/Game/Sphere.hpp>
#include <Volcano/Game/Plane.hpp>
#include <Volcano/Game/Light.hpp>
#include <Volcano/Game/DirectionalLight.hpp>
#include <Volcano/Game/PointLight.hpp>
#include <Volcano/Game/SpotLight.hpp>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Mesh.hpp>

VOLCANO_GRAPHICS_BEGIN

class View {
public:
    View(void);
    virtual ~View(void);

public:
    virtual void reset(void);
    void enableClear(void);
    void disableClear(void);
    void setClearColor(const QColor &v);
    void lookAt(const QVector3D &position, const QVector3D &direction, const QVector3D &up);
    void perspective(float fov, float ratio, float nearPlane, float farPlane);
    void translate(const QVector3D &v);
    void translateTo(const QVector3D &v);
    void scale(const QVector3D &v);
    void scaleTo(const QVector3D &v);
    void rotate(const QQuaternion &v);
    void rotate(const Rotation &v);
    void rotateTo(const QQuaternion &v);
    void rotateTo(const Rotation &v);
    void push(bool copyTop = false);
    void pop(void);

public:
    void add(const Game::Box::Data &v);
    void add(const Game::Cylinder::Data &v);
    void add(const Game::Sphere::Data &v);
    void add(const Game::Plane::Data &v);
    void add(const Game::Light *p);
    void add(const Game::DirectionalLight *p);
    void add(const Game::PointLight *p);
    void add(const Game::SpotLight *p);
    void add(const Mesh *p);

protected:
    struct TransformMesh {
        Transform transform;
        Buffer *vertexBuffer;
    };

    struct Box {
        float length;
        float width;
        float height;
    };

    struct DirectionalLight {
        QColor color;
        QVector3D direction;
    };

    struct PointLight {
        QColor color;
        float radius;
    };

    struct SpotLight {
        QColor color;
        QVector3D direction;
        float radius;
        // shape
    };

    using TransformStack = QStack<Transform>;
    using DirectionalLightList = QList<DirectionalLight>;
    using PointLightList = QList<PointLight>;
    using SpotLightList = QList<SpotLight>;

    const Transform &transform(void);

    bool m_isClearEnabled;
    QColor m_clearColor;
    QVector3D m_position;
    QVector3D m_direction;
    QVector3D m_up;
    float m_fov;
    float m_ratio;
    float m_nearPlane;
    float m_farPlane;
    QColor m_ambientLight;
    DirectionalLightList m_directionalLightList;
    PointLightList m_pointLightList;
    SpotLightList m_spotLightList;

private:
    TransformStack m_transformStack;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_VIEW_HPP
