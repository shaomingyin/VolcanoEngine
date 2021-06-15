//
//
#include <Volcano/Graphics/WorldRenderer.hpp>
#include <Volcano/Graphics/WorldView.hpp>

VOLCANO_GRAPHICS_BEGIN

WorldView::WorldView(QQuickItem *parent):
    QQuickFramebufferObject(parent),
    m_worldRenderer(nullptr),
    m_camera(nullptr),
    m_frameTimer(-1),
    m_frameCountTimer(-1)
{
    setFpsMax(60);
}

WorldView::~WorldView(void)
{
}

WorldView::Renderer *WorldView::createRenderer(void) const
{
    m_worldRenderer = new WorldRenderer();

    return m_worldRenderer;
}

Camera *WorldView::camera(void)
{
    return m_camera;
}

void WorldView::setCamera(Camera *camera)
{
    if (m_camera == camera)
        return;

    m_camera = camera;

    if (m_camera == nullptr) {
        if (m_frameCountTimer >= 0) {
            killTimer(m_frameCountTimer);
            m_frameCountTimer = -1;
        }
    } else
        m_frameCountTimer = startTimer(1000, Qt::PreciseTimer);

    emit cameraChanged(camera);
}

int WorldView::fps(void) const
{
    return m_frameCountPerSecond;
}

int WorldView::fpsMax(void) const
{
    return m_frameCountMax;
}

void WorldView::setFpsMax(int v)
{
    if (m_frameCountMax == v)
        return;

    if (m_frameTimer >= 0) {
        killTimer(m_frameTimer);
        m_frameTimer = -1;
    }

    m_frameCountMax = v;
    if (m_frameCountMax < 1)
        m_frameCount = 1;

    m_frameCount = 0;
    m_frameCountPerSecond = 0;
    m_frameTimer = startTimer(1000 / v, Qt::PreciseTimer);

    emit fpsMaxChanged(v);
}

void WorldView::timerEvent(QTimerEvent *event)
{
    if (Q_LIKELY(event->timerId() == m_frameTimer)) {
        update();
        m_frameCount += 1;
        return;
    }

    if (event->timerId() == m_frameCountTimer) {
        m_frameCountPerSecond = m_frameCount;
        m_frameCount = 0;
        emit fpsChanged(m_frameCountPerSecond);
    }
}

VOLCANO_GRAPHICS_END
