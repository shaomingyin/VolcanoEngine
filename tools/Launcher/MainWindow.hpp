//
//
#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QUrl>
#include <QThread>
#include <QOpenGLWindow>

#include <Volcano/Game/VM.hpp>
#include <Volcano/Graphics/OpenGL/Renderer.hpp>
#include <Volcano/Graphics/OpenGL/Target.hpp>

class MainWindow: public QOpenGLWindow
{
    Q_OBJECT

public:
    MainWindow(QWindow *parent = nullptr);
    ~MainWindow(void) override;

public:
    bool init(const QUrl &url);

protected:
    void initializeGL(void) override;
    void paintGL(void) override;
    void resizeGL(int w, int h) override;
    void run(void);

private:
    QUrl m_url;
    QThread *m_thread;
    Volcano::Game::VM *m_vm;
    Volcano::Graphics::OpenGL::Renderer *m_renderer;
    Volcano::Graphics::OpenGL::Target *m_target;
};

#endif // MAINWINDOW_HPP
