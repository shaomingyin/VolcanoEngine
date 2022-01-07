//
//
#include <memory>

#include <QOpenGLContext>
#include <QQuickOpenGLUtils>
#include <QQuickWindow>

#include <Volcano/Graphics/Renderer.hpp>
#include <Volcano/Graphics/CameraView.hpp>

VOLCANO_GRAPHICS_BEGIN

// CameraRenderer

class CameraRenderer: public QQuickFramebufferObject::Renderer {
public:
    CameraRenderer(void);
    ~CameraRenderer(void) override;

public:
    bool init(int width, int height);
    void render(void) override;
    void synchronize(QQuickFramebufferObject *item) override;

private:
    Volcano::Graphics::Renderer m_renderer;
};

CameraRenderer::CameraRenderer(void)
{
}

CameraRenderer::~CameraRenderer(void)
{
}

bool CameraRenderer::init(int width, int height)
{
    return m_renderer.init(width, height);
}

void CameraRenderer::render(void)
{
    m_renderer.render();
    QQuickOpenGLUtils::resetOpenGLState();
}

void CameraRenderer::synchronize(QQuickFramebufferObject *item)
{
    m_renderer.reset();
    m_renderer.resize(item->width(), item->height());
    auto cameraView = static_cast<CameraView *>(item);
    if (Q_LIKELY(cameraView->isActivate())) {
        cameraView->buildView(&m_renderer);
    } else {
        m_renderer.enableClear();
        m_renderer.setClearColor(QColor::fromRgb(80, 80, 80));
    }
}

// CameraView

CameraView::CameraView(QQuickItem *parent):
    QQuickFramebufferObject(parent),
    m_isActivate(true),
    m_gameActorSet(nullptr),
    m_frameTimer(0),
    m_frameCount(0),
    m_frameCountPerSecond(0),
    m_frameElapsedMin(-1),
    m_isBackgroundEnabled(true),
    m_backgroundColor(QColor::fromRgb(0, 0, 0))
{
    m_frameCountTimer = startTimer(1000);
    setFpsMax(60);
}

CameraView::~CameraView(void)
{
}

CameraView::Renderer *CameraView::createRenderer(void) const
{
    auto renderer = std::make_unique<CameraRenderer>();
    if (!renderer || !renderer->init(width(), height()))
        return nullptr;
    return renderer.release();
}

bool CameraView::isActivate(void) const
{
    return m_isActivate;
}

void CameraView::setActivate(bool v)
{
    if (m_isActivate != v) {
        m_isActivate = v;
        emit activateChanged(v);
    }
}

int CameraView::fps(void) const
{
    return m_frameCountPerSecond;
}

int CameraView::fpsMax(void) const
{
    return (1000 / m_frameElapsedMin);
}

void CameraView::setFpsMax(int v)
{
    if (v < 1)
        return;

    quint64 frameElapsedMin = 1000 / v;
    if (frameElapsedMin < 1)
        return;
    if (m_frameElapsedMin == frameElapsedMin)
        return;

    if (m_frameTimer > 0)
        killTimer(m_frameTimer);

    m_frameTimer = startTimer(frameElapsedMin, Qt::PreciseTimer);
    m_frameCount = 0;
    m_frameCountPerSecond = 0;
    m_elapsedTimer.restart();

    m_frameElapsedMin = frameElapsedMin;
    emit fpsMaxChanged(v);
}

Camera *CameraView::camera(void)
{
    return &m_camera;
}

bool CameraView::isBackgroundEnabled(void) const
{
    return m_isBackgroundEnabled;
}

void CameraView::setBackgroundEnabled(bool v)
{
    if (m_isBackgroundEnabled != v) {
        m_isBackgroundEnabled = v;
        emit backgroundEnabledChanged(v);
    }
}

const QColor CameraView::backgroundColor(void) const
{
    return m_backgroundColor;
}

void CameraView::setBackgroundColor(const QColor &v)
{
    if (m_backgroundColor != v) {
        m_backgroundColor = v;
        emit backgroundColorChanged(v);
    }
}

Game::ActorSet *CameraView::gameActorSet(void)
{
    return m_gameActorSet;
}

void CameraView::setGameActorSet(Game::ActorSet *p)
{
    if (m_gameActorSet != p) {
        m_gameActorSet = p;
        m_world.setGameActorSet(p);
        emit gameActorSetChanged(p);
    }
}

void CameraView::buildView(View *view)
{
    Q_ASSERT(view != nullptr);

    view->lookAt(m_camera.position(), m_camera.direction(), m_camera.up());

    if (m_isBackgroundEnabled) {
        view->enableClear();
        view->setClearColor(m_backgroundColor);
    } else
        view->disableClear();

    m_world.buildView(view, m_camera);
}

void CameraView::timerEvent(QTimerEvent *p)
{
    if (Q_LIKELY(p->timerId() == m_frameTimer)) {
        auto elapsed = m_elapsedTimer.restart();
        frame(std::chrono::milliseconds(elapsed));
        m_frameCount += 1;
        return;
    }

    if (p->timerId() == m_frameCountTimer) {
        m_frameCountPerSecond = m_frameCount;
        m_frameCount = 0;
        emit fpsChanged(m_frameCountPerSecond);
        return;
    }
}

void CameraView::frame(Duration elapsed)
{
    // TODO

    update();
}

VOLCANO_GRAPHICS_END
