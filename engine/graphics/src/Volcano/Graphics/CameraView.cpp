//
//
#include <memory>

#include <QPointer>
#include <QRunnable>
#include <QMutexLocker>

#include <Volcano/Graphics/CameraRenderer.hpp>
#include <Volcano/Graphics/CameraView.hpp>

VOLCANO_GRAPHICS_BEGIN

CameraView::CameraView(QQuickItem *parent):
    QQuickFramebufferObject(parent),
    m_frameTimer(-1),
    m_frameCountTimer(-1),
    m_isClear(true),
    m_clearColor(Qt::black),
    m_viewChain(nullptr)
{
    setFpsMax(60);
}

CameraView::~CameraView(void)
{
}

QQuickFramebufferObject::Renderer *CameraView::createRenderer(void) const
{
    m_viewChain = nullptr;

    auto cameraRenderer = std::make_unique<CameraRenderer>(const_cast<CameraView &>(*this));
    if (!cameraRenderer || !cameraRenderer->init())
        return nullptr;

    m_viewChain = &cameraRenderer->viewChain();

    return cameraRenderer.release();
}

int CameraView::fps(void) const
{
    return m_frameCountPerSecond;
}

int CameraView::fpsMax(void) const
{
    return m_frameCountMax;
}

void CameraView::setFpsMax(int v)
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

    m_frameElapsedTimer.restart();

    if (m_frameCountTimer >= 0) {
        killTimer(m_frameCountTimer);
        m_frameCountTimer = -1;
    }

    m_frameCountTimer = startTimer(1000, Qt::PreciseTimer);

    emit fpsMaxChanged(v);
}

bool CameraView::isClear(void) const
{
    return m_isClear;
}

void CameraView::setClear(bool v)
{
    if (m_isClear != v) {
        m_isClear = v;
        emit clearChanged(v);
    }
}

const QColor &CameraView::clearColor(void) const
{
    return m_clearColor;
}

void CameraView::setClearColor(const QColor &v)
{
    if (m_clearColor != v) {
        m_clearColor = v;
        emit clearColorChanged(v);
    }
}

Camera *CameraView::camera(void)
{
    return &m_camera;
}

void CameraView::timerEvent(QTimerEvent *event)
{
    if (Q_UNLIKELY(!isVisible()))
        return;

    if (Q_LIKELY(event->timerId() == m_frameTimer)) {
        frame();
        m_frameCount += 1;
        return;
    }

    if (event->timerId() == m_frameCountTimer) {
        m_frameCountPerSecond = m_frameCount;
        m_frameCount = 0;
        emit fpsChanged(m_frameCountPerSecond);
        return;
    }
}

void CameraView::frame(void)
{
    if (Q_UNLIKELY(!isVisible() || size().isEmpty()))
        return;

    if (Q_LIKELY(m_viewChain != nullptr)) {
        auto view = m_viewChain->acquire();
        if (Q_LIKELY(view != nullptr)) {
            m_camera.buildView(view);
            m_viewChain->commit(view);
        }
    }

    // TODO sync with rendering.

    update();
}

VOLCANO_GRAPHICS_END
