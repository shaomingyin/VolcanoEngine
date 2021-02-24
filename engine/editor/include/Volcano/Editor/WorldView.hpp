//
//
#ifndef VOLCANO_EDITOR_WORLDVIEW_HPP
#define VOLCANO_EDITOR_WORLDVIEW_HPP

#include <QOpenGLWidget>

#include <Volcano/UI/Graphics/Memory.hpp>
#include <Volcano/UI/Graphics/Renderer.hpp>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class WorldView: public QOpenGLWidget {
    Q_OBJECT

public:
    WorldView(QWidget *parent = nullptr);
    ~WorldView(void) override;

protected:
    void initializeGL(void) override;
    void paintGL(void) override;
    void resizeGL(int w, int h) override;

private:
    UI::Graphics::Memory *m_graphicsMemory;
    UI::Graphics::Renderer *m_graphicsRenderer;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_WORLDVIEW_HPP
