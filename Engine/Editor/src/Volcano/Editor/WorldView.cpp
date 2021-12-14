//
//

#include <Volcano/Editor/WorldView.hpp>

VOLCANO_EDITOR_BEGIN

WorldView::WorldView(QWidget *parent):
    QOpenGLWidget(parent)
{
}

WorldView::~WorldView(void)
{
}

void WorldView::initializeGL(void)
{
    if (!m_graphicsRenderer.init(width(), height())) {
        qCritical("Failed to create graphics renderer.");
        return;
    }

    // TODO
}

void WorldView::paintGL(void)
{
    m_graphicsRenderer.reset();
    m_graphicsWorld.buildView(&m_graphicsRenderer, m_graphicsCamera);
    m_graphicsRenderer.render();
}

void WorldView::resizeGL(int w, int h)
{
    m_graphicsRenderer.resize(w, h);
}

VOLCANO_EDITOR_END
