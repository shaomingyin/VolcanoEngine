//
//
#ifndef VOLCANO_RENDERER_H
#define VOLCANO_RENDERER_H

#include <QQuickFramebufferObject>

#include <Volcano/Common.h>
#include <Volcano/Node.h>

VOLCANO_BEGIN

class Renderer: public QQuickFramebufferObject::Renderer
{
public:
    Renderer(void);
    ~Renderer(void) override;

public:
    void render(void) override;
    void synchronize(QQuickFramebufferObject *obj) override;

protected:
    virtual bool init(void);

private:
    enum State {
        StateIdle,
        StateInited,
        StateReady,
        StateLoading,
        StateError
    };

    State m_state;
    qreal m_loadingProgress;
};

VOLCANO_END

#endif // VOLCANO_RENDERER_H
