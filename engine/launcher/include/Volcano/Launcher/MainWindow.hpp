//
//
#ifndef VOLCANO_LAUNCHER_MAINWINDOW_HPP
#define VOLCANO_LAUNCHER_MAINWINDOW_HPP

#include <QOpenGLWindow>

#include <Volcano/Graphics/Memory.hpp>
#include <Volcano/Graphics/Renderer.hpp>
#include <Volcano/Launcher/Common.hpp>

VOLCANO_LAUNCHER_BEGIN

class MainWindow: public QOpenGLWindow
{
    Q_OBJECT

public:
    MainWindow(void);
    ~MainWindow(void) override;

public:
    bool init(void);

protected:
    void initializeGL(void) override;
    void paintGL(void) override;
    void resizeGL(int w, int h) override;

private:
    Graphics::Memory *m_graphicsMemory;
    Graphics::Renderer *m_graphicsRenderer;
};

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_MAINWINDOW_HPP
