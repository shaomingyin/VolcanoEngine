//
//
#include <memory>

#include <QResizeEvent>

#include <Volcano/Editor/Viewport.hpp>

VOLCANO_EDITOR_BEGIN

Viewport::Viewport(QWidget *parent):
    QOpenGLWidget(parent),
    m_gameWorld(nullptr),
    m_glFunctions(nullptr),
    m_graphicsView(nullptr)
{
}

Viewport::~Viewport(void)
{
}

Game::World *Viewport::gameWorld(void)
{
    return m_gameWorld;
}

void Viewport::setGameWorld(Game::World *gameWorld)
{
    if (m_gameWorld ==  gameWorld)
        return;

    m_graphicsCamera.setGameWorld(gameWorld);

    m_gameWorld = gameWorld;
    emit gameWorldChanged(gameWorld);
}

void Viewport::initializeGL(void)
{
    m_glFunctions = Graphics::glFunctions();
    if (m_glFunctions == nullptr)
        return;

    if (m_graphicsView != nullptr) {
        delete m_graphicsView;
        m_graphicsView = nullptr;
    }

    auto graphicaView = std::make_unique<Graphics::View>(this);
    if (!graphicaView || !graphicaView->init(size()))
        return;

    m_graphicsView = graphicaView.release();

    m_glFunctions->glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
}

void Viewport::paintGL(void)
{
    if (m_glFunctions == nullptr)
        return;

    m_glFunctions->glViewport(0, 0, width(), height());
    m_glFunctions->glClear(GL_COLOR_BUFFER_BIT);

    if (m_graphicsView != nullptr) {
        m_graphicsView->reset();
        m_graphicsCamera.buildView(m_graphicsView);
        m_graphicsView->render();
    }
}

void Viewport::resizeGL(int w, int h)
{
    if (m_graphicsView != nullptr)
        m_graphicsView->setSize(size());
}

VOLCANO_EDITOR_END
