//
//
#ifndef VOLCANO_OPENGL_RENDERER_HPP
#define VOLCANO_OPENGL_RENDERER_HPP

#include <QSizeF>
#include <QMatrix4x4>
#include <QObject>

#include <Volcano/OpenGL/Common.hpp>

VOLCANO_OPENGL_BEGIN

class Renderer: public QObject {
    Q_OBJECT

public:
    Renderer(void);
    ~Renderer(void) override;

public:
    bool init(const QSize &size);
    const QSizeF &size(void);
    void setSize(const QSizeF &v);
    void beginFrame(void);
    void endFrame(void);

signals:
    void sizeChanged(const QSizeF &v);

private:
    Functions *m_fns;
    QSizeF m_size;
};

VOLCANO_OPENGL_END

#endif // VOLCANO_OPENGL_RENDERER_HPP
