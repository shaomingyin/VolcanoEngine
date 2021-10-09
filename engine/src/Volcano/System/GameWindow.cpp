//
//
#include <memory>

#include <QQuickOpenGLUtils>

#include <Volcano/System/GameWindow.hpp>

VOLCANO_SYSTEM_BEGIN

GameWindow::GameWindow(void):
    m_graphicsRenderer(nullptr),
    m_gameWorld(nullptr)
{
    connect(this, &QQuickWindow::sceneGraphInitialized, this, &GameWindow::onSceneGraphInitialized, Qt::DirectConnection);
    connect(this, &QQuickWindow::beforeRendering, this, &GameWindow::onBeforeRendering, Qt::DirectConnection);
}

GameWindow::~GameWindow(void)
{
}

Game::World *GameWindow::gameWorld(void)
{
    return m_gameWorld;
}

void GameWindow::setGameWorld(Game::World *p)
{
    if (m_gameWorld != p) {
        m_gameWorld = p;
        emit gameWorldChanged(p);
    }
}

void GameWindow::onSceneGraphInitialized(void)
{
    if (m_graphicsRenderer != nullptr)
        return;

    auto graphicsRenderer = std::make_unique<Graphics::Renderer>(this);
    if (!graphicsRenderer || !graphicsRenderer->init())
        return;

    graphicsRenderer->resize(width(), height());

    m_graphicsRenderer = graphicsRenderer.release();
}

void GameWindow::onBeforeRendering(void)
{
    if (m_graphicsRenderer != nullptr && m_gameWorld != nullptr) {
        renderGameWorld();
        QQuickOpenGLUtils::resetOpenGLState();
    }
}

void GameWindow::renderGameWorld(void)
{
    Q_ASSERT(m_graphicsRenderer != nullptr);
    Q_ASSERT(m_gameWorld != nullptr);

    // TODO update renderer size.

    m_graphicsRenderer->beginFrame();

    // TODO build visible set from game world
    // and add it to graphics renderer.

    m_graphicsRenderer->endFrame();
}

VOLCANO_SYSTEM_END
