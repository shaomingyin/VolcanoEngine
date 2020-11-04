//
//
#ifndef VOLCANO_LAUNCHER_MAINWINDOW_HPP
#define VOLCANO_LAUNCHER_MAINWINDOW_HPP

#include <QUrl>
#include <QThread>
#include <QOpenGLWindow>

#include <Volcano/Game/VM.hpp>
#include <Volcano/Graphics/Renderer.hpp>
#include <Volcano/Launcher/Common.hpp>

VOLCANO_LAUNCHER_BEGIN

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
    Volcano::Game::VM m_vm;
    Volcano::Graphics::Renderer m_renderer;
};

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_MAINWINDOW_HPP
