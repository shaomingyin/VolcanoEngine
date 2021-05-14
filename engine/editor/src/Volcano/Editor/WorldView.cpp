//
//
#include <Volcano/Editor/WorldView.hpp>

#include <QSurfaceFormat>
#include <QOpenGLContext>

VOLCANO_EDITOR_BEGIN

WorldView::WorldView(QWidget *parent):
	QOpenGLWidget(parent),
	m_ddContext(nullptr),
	m_ready(false)
{
	QSurfaceFormat fmt;

	fmt.setVersion(3, 3);
	fmt.setProfile(QSurfaceFormat::OpenGLContextProfile::CoreProfile);
	fmt.setSwapBehavior(QSurfaceFormat::SwapBehavior::DoubleBuffer);

	fmt.setRedBufferSize(8);
	fmt.setGreenBufferSize(8);
	fmt.setBlueBufferSize(8);
	fmt.setAlphaBufferSize(8);

	fmt.setDepthBufferSize(24);

	setFormat(fmt);
}

WorldView::~WorldView(void)
{
	if (dd::isInitialized(m_ddContext))
		dd::shutdown(m_ddContext);
}

bool WorldView::init(void)
{
	QScopedPointer<Graphics::Renderer> graphicsRenderer(new Graphics::Renderer());
	if (!graphicsRenderer) {
		spdlog::error("Failed to create graphics renderer.");
		return false;
	}

	auto ret = gl3wInit(&m_gl3w, [](const char *procName) {
		auto context = QOpenGLContext::currentContext();
		Q_ASSERT(context != nullptr);
		return reinterpret_cast<GL3WProc>(context->getProcAddress(procName));
	});

	if (ret != GL3W_OK) {
		spdlog::error("Failed to init gl3w.");
		return false;
	}

	QScopedPointer<DebugDrawRenderer> ddRenderer(new DebugDrawRenderer(this));
	if (!ddRenderer) {
		spdlog::error("Failed to create debug draw renderer.");
		return false;
	}

	m_graphicsRenderer.reset(graphicsRenderer.take());
	m_ddRenderer.reset(ddRenderer.take());

	return true;
}

void WorldView::initializeGL(void)
{
	Q_ASSERT(m_graphicsRenderer);
	Q_ASSERT(m_ddRenderer);

	gl3wProcs = &m_gl3w;

	if (!m_graphicsRenderer->init(width(), height())) {
		spdlog::error("Failed to init graphics renderer.");
		return;
	}

	if (!m_ddRenderer->init(width(), height())) {
		spdlog::error("Failed to init debug draw renderer.");
		return;
	}

	if (!dd::initialize(&m_ddContext, m_ddRenderer.get())) {
		spdlog::error("Failed to init debug draw.");
		return;
	}
}

void WorldView::paintGL(void)
{
	Q_ASSERT(m_graphicsRenderer);
	Q_ASSERT(m_ddRenderer);

	gl3wProcs = &m_gl3w;
}

void WorldView::resizeGL(int w, int h)
{
	Q_ASSERT(m_graphicsRenderer);
	Q_ASSERT(m_ddRenderer);

	gl3wProcs = &m_gl3w;

	m_ddRenderer->resize(w, h);
}

VOLCANO_EDITOR_END
