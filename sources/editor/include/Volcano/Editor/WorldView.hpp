//
//
#ifndef VOLCANO_EDITOR_WORLDVIEW_HPP
#define VOLCANO_EDITOR_WORLDVIEW_HPP

#include <GL/gl3w.h>

#include <QScopedPointer>
#include <QOpenGLWidget>

#include <Volcano/Graphics/Renderer.hpp>
#include <Volcano/Editor/Common.hpp>
#include <Volcano/Editor/DebugDrawRenderer.hpp>

VOLCANO_EDITOR_BEGIN

class WorldView : public QOpenGLWidget {
	Q_OBJECT

public:
	WorldView(QWidget *parent = nullptr);
	~WorldView(void) override;

public:
	bool init(void);

protected:
	void initializeGL(void) override;
	void paintGL(void) override;
	void resizeGL(int w, int h) override;

private:
	union GL3WProcs m_gl3w;
	QScopedPointer<Graphics::Renderer> m_graphicsRenderer;
	QScopedPointer<DebugDrawRenderer> m_ddRenderer;
	dd::ContextHandle m_ddContext;
	bool m_ready;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_WORLDVIEW_HPP
