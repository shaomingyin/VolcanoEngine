//
//
#ifndef VOLCANO_LAUNCHER_H
#define VOLCANO_LAUNCHER_H

#include <QUrl>
#include <QOpenGLContext>
#include <QQuickView>
#include <QMainWindow>

#include <Volcano/Common.h>
#include <Volcano/Renderer.h>

VOLCANO_BEGIN

class Launcher: public QMainWindow
{
    Q_OBJECT

public:
    Launcher(void);
    ~Launcher(void) override;

public:
    virtual bool init(const QUrl &url);

private slots:
    void initRendererContext(void);
    void releaseRendererContext(void);
    void beforeRendering(void);

private:
    QQuickView *m_view;
    QWidget *m_viewWidget;
    Renderer::Context *m_renderer;
};

VOLCANO_END

#endif // VOLCANO_LAUNCHER_H
