//
//
#ifndef VOLCANO_VIEWPORT_HPP
#define VOLCANO_VIEWPORT_HPP

#include <QQuickFramebufferObject>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class Viewport: public QQuickFramebufferObject
{
    Q_OBJECT

public:
    Viewport(QQuickItem *parent = nullptr);
    ~Viewport(void) override;

public:
    Renderer *createRenderer(void) const override;
};

VOLCANO_END

#endif // VOLCANO_VIEWPORT_HPP
