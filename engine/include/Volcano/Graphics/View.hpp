//
//
#ifndef VOLCANO_GRAPHICS_VIEW_HPP
#define VOLCANO_GRAPHICS_VIEW_HPP

#include <QSize>
#include <QStack>
#include <QVector>
#include <QRectF>
#include <QColor>
#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QAtomicInt>
#include <QObject>

#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class View: public QObject {
    Q_OBJECT

public:
    View(QObject *parent = nullptr);
    ~View(void) override;

public:
    const QSize size(void) const;
    void setSize(const QSize &v);
    bool begin(void);
    void end(void);
    void beforeRender(void);
    void render(void);

public:
    void setClear(bool v);
    void setClearColor(const QColor &v);
    void lookAt(const QVector3D &position,  const QVector3D &direction, const QVector3D &up);
    void setProjection(const QMatrix4x4 &v);
    void pushTransform(void);
    void popTransform(void);
    void resetTransform(void);
    void translate(const QVector3D &v);
    void translateTo(const QVector3D &v);
    void scale(const QVector3D &v);
    void scaleTo(const QVector3D &v);
    void rotate(float angle, const QVector3D &axis);
    void rotateTo(float angle, const QVector3D &axis);

private:
    struct Transform {
        QVector3D translate;
        QVector3D scale;
        QQuaternion rotation;

        Transform(void);
        void reset(void);
    };

    using TransformStack = QVector<Transform>;

    struct Def {
        bool isClear;
        QColor clearColor;
        QVector3D position;
        QVector3D direction;
        QVector3D up;
        QMatrix4x4 projection;
        TransformStack transformStack;
    };

    void renderDef(Def &d);

private:
    OpenGLFunctions *m_gl;
    QSize m_size;
    QAtomicInt m_state;
    int m_rendering;
    Def m_def[2];
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_VIEW_HPP
