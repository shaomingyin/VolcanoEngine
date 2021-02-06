//
//
#include <QSettings>
#include <QScopeGuard>

#include <Volcano/Launcher/MainWindow.hpp>

VOLCANO_LAUNCHER_BEGIN

MainWindow::MainWindow(void):
    m_graphicsMemory(nullptr),
    m_graphicsRenderer(nullptr)
{
}

MainWindow::~MainWindow(void)
{
    if (m_graphicsRenderer != nullptr)
    {
        // TODO
    }

    if (m_graphicsMemory != nullptr)
    {

    }
}

bool MainWindow::init(void)
{
    return true;
}

void MainWindow::initializeGL(void)
{
    if (m_graphicsRenderer != nullptr)
    {
        return;
    }

    m_graphicsRenderer = new Graphics::Renderer(this);
    if (m_graphicsRenderer == nullptr)
    {
        return;
    }

    auto graphicsRendererGuard = qScopeGuard([&]() {
        delete m_graphicsRenderer;
        m_graphicsRenderer = nullptr;
    });

    if (!m_graphicsRenderer->init(this))
    {
        return;
    }

    if (m_graphicsMemory != nullptr)
    {
        return;
    }

    m_graphicsMemory = new Graphics::Memory();
    if (m_graphicsMemory == nullptr)
    {
        return;
    }

    auto graphicsMemoryGuard = qScopeGuard([&]() {
        delete m_graphicsMemory;
        m_graphicsMemory = nullptr;
    });

    graphicsRendererGuard.dismiss();
    graphicsMemoryGuard.dismiss();
}

void MainWindow::paintGL(void)
{
    if (m_graphicsRenderer != nullptr)
    {
        m_graphicsRenderer->beginFrame();

        m_graphicsRenderer->endFrame();
    }
}

void MainWindow::resizeGL(int w, int h)
{
}

VOLCANO_LAUNCHER_END
