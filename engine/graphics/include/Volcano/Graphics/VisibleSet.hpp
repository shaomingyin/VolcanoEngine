//
//
#ifndef VOLCANO_GRAPHICS_VISIBLESET_HPP
#define VOLCANO_GRAPHICS_VISIBLESET_HPP

#include <QPair>
#include <QStack>
#include <QVector>
#include <QRectF>
#include <QColor>
#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Renderable.hpp>

VOLCANO_GRAPHICS_BEGIN

class VisibleSet {
public:
    struct Transform {
        QVector3D translate;
        QVector3D scale;
        QQuaternion rotation;
    };

    using TransformedRenderable = QPair<Transform, Renderable *>;
    using TransformedRenderableVector = QVector<TransformedRenderable>;

public:
    VisibleSet(void);
    virtual ~VisibleSet(void);

public:
    void reset(void);
    void pushTransform(void);
    void popTransform(void);
    void loadIdienty(void);
    void translate(float x, float y, float z);
    void scale(float x, float y, float z);
    void rotate(float angle, float x, float y, float z);
    void add(Renderable *renerable);

public: // readonly for renderer.
    const TransformedRenderableVector &renderables(void) const;

private:
    using TransformStack = QVector<Transform>;

private:
    TransformStack m_transformStack;
    TransformedRenderableVector m_transformedRenderableVector;
    Renderable *m_lastAddedRenderable;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_VISIBLESET_HPP
