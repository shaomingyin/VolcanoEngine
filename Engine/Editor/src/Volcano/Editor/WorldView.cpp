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
    if (!m_graphicsView.init(width(), height())) {
        qCritical("Failed to create graphics view.");
        return;
    }

    // TODO
}

void WorldView::paintGL(void)
{
    // TODO update graphics view...
    // m_viewalbe[clientWorld?].buildVisibleSet(m_graphicsView, m_graphicsCamera);

    m_graphicsView.render();
}

void WorldView::resizeGL(int w, int h)
{
    m_graphicsView.resize(w, h);
}

VOLCANO_EDITOR_END
