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
#include <QObject>

#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class View: public QObject {
    Q_OBJECT

public:
    struct Transform {
        QVector3D translate;
        QVector3D scale;
        QQuaternion rotation;
    };

public:
    View(QObject *parent = nullptr);
    ~View(void) override;

public:
    bool init(const QSize &v);
    const QSize size(void) const;
    void setSize(const QSize &v);
    void reset(void);
    void render(void);

private:
    OpenGLFunctions *m_gl;
    QSize m_size;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_VIEW_HPP
