//
//
#ifndef VOLCANO_GRAPHICS_VIEW_HPP
#define VOLCANO_GRAPHICS_VIEW_HPP

#include <QStack>
#include <QColor>
#include <QMatrix4x4>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Mesh.hpp>
#include <Volcano/Graphics/Transform.hpp>

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
    void rotateTo(const QQuaternion &v);
    void push(bool copyTop = false);
    void pop(void);

protected:
    Transform &currentTransform(void);

    bool m_isClearEnabled;
    QColor m_clearColor;
    QVector3D m_position;
    QVector3D m_direction;
    QVector3D m_up;
    float m_fov;
    float m_ratio;
    float m_nearPlane;
    float m_farPlane;

private:
    struct TransformMesh {
        Transform transform;
        Buffer *vertexBuffer;
    };

    using TransformStack = QStack<Transform>;

    TransformStack m_transformStack;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_VIEW_HPP
