//
//
#ifndef VOLCANO_GRAPHICS_VIEW_HPP
#define VOLCANO_GRAPHICS_VIEW_HPP

#include <QSize>
#include <QColor>
#include <QVector3D>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/VisibleSet.hpp>

VOLCANO_GRAPHICS_BEGIN

class View: public VisibleSet {
public:
    View(void);
    virtual ~View(void);

public:
    bool init(int width, int height);
    void reset(void) override;
    void resize(const QSize &v);
    void resize(int width, int height);
    void render(void);

public:
    void enableClear(void);
    void disableClear(void);
    void setClearColor(const QColor &v);
    void lookAt(const QVector3D &position, const QVector3D &direction, const QVector3D &up);

private:
    QSize m_size;
    OpenGLFunctions *m_gl;
    bool m_isClearEnabled;
    QColor m_clearColor;
    QVector3D m_position;
    QVector3D m_direction;
    QVector3D m_up;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_VIEW_HPP
