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

VOLCANO_GRAPHICS_BEGIN

class VisibleSet {
public:
    struct Transform {
        QVector3D translate;
        QVector3D scale;
        QQuaternion rotation;
    };

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

private:
    using TransformStack = QVector<Transform>;

private:
    TransformStack m_transformStack;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_VISIBLESET_HPP
