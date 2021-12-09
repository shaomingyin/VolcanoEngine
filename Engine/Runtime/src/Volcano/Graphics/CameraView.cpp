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
    if (!m_renderer.init(width, height))
        return false;
    return true;
}

void CameraRenderer::render(void)
{
    m_renderer.render();
    QQuickOpenGLUtils::resetOpenGLState();
}

void CameraRenderer::synchronize(QQuickFramebufferObject *item)
{
    m_renderer.reset();
    m_renderer.resize(QSize(item->width(), item->height()));

    auto cameraView = static_cast<CameraView *>(item);
    auto camera = cameraView->camera();
    m_renderer.lookAt(camera->position(), camera->direction(), camera->up());
    if (cameraView->isBackgroundEnabled()) {
        m_renderer.enableClear();
        m_renderer.setClearColor(cameraView->backgroundColor());
    } else
        m_renderer.disableClear();
    cameraView->buildVisibleSet(&m_renderer);
}

CameraView::CameraView(QQuickItem *parent):
    QQuickFramebufferObject(parent),
    m_isResized(false),
    m_gameWorld(nullptr),
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

Game::WorldBase *CameraView::gameWorld(void)
{
    return m_gameWorld;
}

void CameraView::setGameWorld(Game::WorldBase *p)
{
    if (m_gameWorld == p)
        return;

    if (m_gameWorld != nullptr) {

    }

    m_gameWorld = p;

    if (m_gameWorld != nullptr) {

    }

    emit gameWorldChanged(p);
}

void CameraView::buildVisibleSet(VisibleSet *p)
{
    Q_ASSERT(p != nullptr);
}

void CameraView::timerEvent(QTimerEvent *p)
{
    if (Q_LIKELY(p->timerId() == m_frameTimer)) {
        auto elapsed = m_elapsedTimer.restart();
        frame(float(elapsed) / 1000.0f);
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

void CameraView::frame(quint64 elapsed)
{
//    if (Q_LIKELY(m_world != nullptr))
//        m_world->update(elapsed);

    update();
}

VOLCANO_GRAPHICS_END
